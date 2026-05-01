# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appHVAC_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appHVAC_autogen.dir/ParseCache.txt"
  "appHVAC_autogen"
  )
endif()
