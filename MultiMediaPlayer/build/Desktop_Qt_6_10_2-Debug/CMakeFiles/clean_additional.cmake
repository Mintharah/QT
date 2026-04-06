# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appMultiMediaPlayer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appMultiMediaPlayer_autogen.dir/ParseCache.txt"
  "appMultiMediaPlayer_autogen"
  )
endif()
