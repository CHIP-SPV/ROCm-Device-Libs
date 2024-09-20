##===--------------------------------------------------------------------------
##                   ROCm Device Libraries
##
## This file is distributed under the University of Illinois Open Source
## License. See LICENSE.TXT for details.
##===--------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13.4)

include(AddLLVM)

if (ROCM_DEVICELIB_STANDALONE_BUILD)
  add_definitions(${LLVM_DEFINITIONS})
  include_directories(${LLVM_INCLUDE_DIR})
  include_directories(${LLVM_CONFIG_INCLUDE_DIR})
  include_directories(${LLVM_MAIN_INCLUDE_DIR})

  include_directories(${LLVM_INCLUDE_DIRS})
  add_definitions(${LLVM_DEFINITIONS})
  link_directories("${LLVM_LIBRARY_DIR}")
endif()

add_executable(prepare-builtins prepare-builtins.cpp)
target_compile_options(prepare-builtins PRIVATE -I/${LLVM_INCLUDE_DIRS})
set_target_properties(prepare-builtins PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED Yes
  CXX_EXTENSIONS No)

set(LLVM_COMPONENTS_FOUND TRUE)
set(LLVM_COMPONENTS support core bitreader bitwriter)

llvm_map_components_to_libnames(llvm_libs support core bitreader bitwriter)

# Find LLVM libraries
foreach(lib ${llvm_libs})
  find_library(${lib}_LIBRARY
    NAMES lib${lib}.a lib${lib}.so
    PATHS ${LLVM_LIBRARY_DIR}
    NO_DEFAULT_PATH
  )
  if(${lib}_LIBRARY)
    message(STATUS "Found LLVM library: ${${lib}_LIBRARY}")
    list(APPEND FOUND_LLVM_LIBS ${${lib}_LIBRARY})
  else()
    message(STATUS "Could not find LLVM library: ${lib}")
    set(LLVM_COMPONENTS_FOUND FALSE)
  endif()
  unset(${lib}_LIBRARY CACHE)
endforeach()

if(LLVM_COMPONENTS_FOUND)
  message(STATUS "Found LLVM Components: ${FOUND_LLVM_LIBS}")
  llvm_update_compile_flags(prepare-builtins)
  target_link_libraries(prepare-builtins PRIVATE ${llvm_libs})
else()
  # Find libLLVM
  find_library(LLVM_LIBRARY
    NAMES libLLVM.so libLLVM.a
    PATHS ${LLVM_LIBRARY_DIR}
    NO_DEFAULT_PATH
    REQUIRED
  )
  message(STATUS "Found LLVM Library: ${LLVM_LIBRARY}")
  llvm_config(prepare-builtins USE_SHARED support core bitreader bitwriter)
endif()

