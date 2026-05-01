file(REMOVE_RECURSE
  "Calculator/Main.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appCalculator_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
