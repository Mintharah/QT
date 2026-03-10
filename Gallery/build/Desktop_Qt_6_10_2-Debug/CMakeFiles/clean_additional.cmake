# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Spongebob_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Spongebob_autogen.dir/ParseCache.txt"
  "Spongebob_autogen"
  )
endif()
