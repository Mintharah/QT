#include "cluster.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <cmath>

// QNX: prefer slog2 for production logging; falls back gracefully if unavailable
#ifdef __QNXNTO__
#  include <sys/slog2.h>
#  define SLOG_ERR(msg)  slog2f(nullptr, 0, SLOG2_ERROR,   "[backend] %s", msg)
#  define SLOG_INFO(msg) slog2f(nullptr, 0, SLOG2_INFO,    "[backend] %s", msg)
#else
#  define SLOG_ERR(msg)  qWarning()  << "[backend]" << msg
#  define SLOG_INFO(msg) qDebug()    << "[backend]" << msg
#endif

// QNX-safe replacement for std::clamp (C++17 stdlib incomplete on some QCC targets)
template<typename T>
static inline T qnx_clamp(T val, T lo, T hi)
{
    return val < lo ? lo : (val > hi ? hi : val);
}

VehicleBackend::VehicleBackend(const QString &csvPath, QObject *parent)
    : QObject(parent), m_csvPath(csvPath)
{
    loadCsv();

    m_timer = new QTimer(this);
    m_timer->setInterval(100);   // one row per 100 ms — match original recording rate
    connect(m_timer, &QTimer::timeout, this, &VehicleBackend::tick);
    m_timer->start();
}

// ── Load entire CSV into memory ───────────────────────────────────────────
void VehicleBackend::loadCsv()
{
    QFile file(m_csvPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // QNX: QFile::errorString() is reliable; also emit slog2 for system log
        QString err = QString("Cannot open: %1 — %2")
                          .arg(m_csvPath, file.errorString());
#ifdef __QNXNTO__
        slog2f(nullptr, 0, SLOG2_ERROR, "[backend] %s",
               err.toLocal8Bit().constData());
#else
        qWarning() << "[backend]" << err;
#endif
        return;
    }

    QTextStream in(&file);
    // QNX: explicitly set UTF-8; default codec can differ by locale
    in.setEncoding(QStringConverter::Utf8);

    m_rows.clear();
    m_rowIndex = 0;

    if (!in.atEnd()) {
        m_headers = in.readLine().trimmed().split(',');
        qDebug() << "[backend] Headers:" << m_headers;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
            m_rows.append(line.split(','));
    }

    qDebug() << "[backend] Loaded" << m_rows.size() << "rows";
}

// ── Advance one row per timer tick ────────────────────────────────────────
void VehicleBackend::tick()
{
    if (m_rows.isEmpty()) return;

    if (m_rowIndex >= m_rows.size()) {
        qDebug() << "[backend] End of file — looping";
        m_rowIndex = 0;
    }

    applyRow(m_rows.at(m_rowIndex));
    ++m_rowIndex;
}

// ── Parse a row and push to properties ───────────────────────────────────
void VehicleBackend::applyRow(const QStringList &row)
{
    if (row.size() < m_headers.size()) {
        qWarning() << "[backend] Short row at index" << m_rowIndex << "— skipping";
        return;
    }

    auto get = [&](const QString &col) -> float {
        int idx = m_headers.indexOf(col);
        if (idx < 0 || idx >= row.size()) return 0.f;
        bool ok = false;
        float val = row.at(idx).toFloat(&ok);
        // QNX: toFloat() can silently return 0 on locale mismatch; guard it
        return ok ? val : 0.f;
    };

    const float v        = get("voltage_V");
    const float i        = get("current_A");
    // Use qnx_clamp instead of std::clamp for QCC C++17 stdlib safety
    const float rawPower = qnx_clamp((v * i / MAX_POWER) * 100.f, 0.f, 100.f);
    m_powerSmoothed      = m_powerSmoothed + POWER_SMOOTH * (rawPower - m_powerSmoothed);

    const float battPct  = qnx_clamp(
        (v - VOLT_MIN) / (VOLT_MAX - VOLT_MIN) * 100.f, 0.f, 100.f);

    setSpeed(get("speed"));
    setVoltage(v);
    setCurrent(i);
    setPower(m_powerSmoothed);
    setTemp(get("temp_C"));
    setVibTotal(get("vib_total"));
    setBattery(battPct);

    evaluateWarnings();
}

// ── Warning logic ─────────────────────────────────────────────────────────
void VehicleBackend::evaluateWarnings()
{
    bool sw   = m_speed    >= SPEED_WARN;
    bool tw   = m_temp     >= TEMP_WARN;
    bool vw   = m_vibTotal >= VIB_WARN;
    bool volw = m_voltage  <= VOLT_WARN;
    bool crit = m_speed    >= SPEED_CRIT
                || m_temp     >= TEMP_WARN  + 20.f
                || m_vibTotal >= VIB_WARN   * 2.f;

    if (sw   != m_speedWarning)   { m_speedWarning   = sw;   emit speedWarningChanged();   }
    if (tw   != m_tempWarning)    { m_tempWarning     = tw;   emit tempWarningChanged();    }
    if (vw   != m_vibWarning)     { m_vibWarning      = vw;   emit vibWarningChanged();     }
    if (volw != m_voltageWarning) { m_voltageWarning  = volw; emit voltageWarningChanged(); }
    if (crit != m_criticalAlert)  { m_criticalAlert   = crit; emit criticalAlertChanged();  }
}

// ── Public slot: restart playback from row 0 ─────────────────────────────
void VehicleBackend::reloadData()
{
    m_rowIndex = 0;
    loadCsv();
}

// ── Setters ───────────────────────────────────────────────────────────────
void VehicleBackend::setSpeed(float v)    { if (m_speed    != v) { m_speed    = v; emit speedChanged();    } }
void VehicleBackend::setPower(float v)    { if (m_power    != v) { m_power    = v; emit powerChanged();    } }
void VehicleBackend::setBattery(float v)  { if (m_battery  != v) { m_battery  = v; emit batteryChanged();  } }
void VehicleBackend::setTemp(float v)     { if (m_temp     != v) { m_temp     = v; emit tempChanged();     } }
void VehicleBackend::setVibTotal(float v) { if (m_vibTotal != v) { m_vibTotal = v; emit vibTotalChanged(); } }
void VehicleBackend::setCurrent(float v)  { if (m_current  != v) { m_current  = v; emit currentChanged();  } }
void VehicleBackend::setVoltage(float v)  { if (m_voltage  != v) { m_voltage  = v; emit voltageChanged();  } }