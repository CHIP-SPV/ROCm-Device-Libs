##===--------------------------------------------------------------------------
##                   ROCm Device Libraries
##
## This file is distributed under the University of Illinois Open Source
## License. See LICENSE.TXT for details.
##===--------------------------------------------------------------------------

file(GLOB sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.ll
)

set(irif_lib_file ${CMAKE_CURRENT_BINARY_DIR}/irif.bc)

# find_program(LLVM_LINK_EXECUTABLE NAMES llvm-link PATHS ${LLVM_CONFIG_DIR} NO_DEFAULT_PATH)
# if(NOT LLVM_LINK_EXECUTABLE)
#   message(FATAL_ERROR "llvm-link not found")
# endif()
# add_executable(llvm-link IMPORTED)
# set_target_properties(llvm-link PROPERTIES IMPORTED_LOCATION ${LLVM_LINK_EXECUTABLE})

# The textual IR sources on purpose omit 'target triple' and 'target
# datalayout' statements so we can retarget them to the desired
# target. This command sets the desired target triple and populates
# the 'target datalayout' for it.
foreach(source IN LISTS sources)
  get_filename_component(source_name ${source} NAME_WLE)
  set(bitcode_source ${CMAKE_CURRENT_BINARY_DIR}/${source_name}.bc)
  add_custom_command(
    OUTPUT ${bitcode_source}
    COMMAND ${CLANG_ROOT_PATH_BIN}/opt -mtriple ${AMDGPU_TARGET_TRIPLE}
    ${source} -o ${bitcode_source}
    DEPENDS ${source})
  list(APPEND bitcode_sources ${bitcode_source})
endforeach()

add_custom_command(OUTPUT ${irif_lib_file}
  COMMAND $<TARGET_FILE:llvm-link> ${bitcode_sources} -o ${irif_lib_file}
  DEPENDS ${bitcode_sources})

add_custom_target(irif DEPENDS ${irif_lib_file}
                       SOURCES ${bitcode_sources})
set_target_properties(irif PROPERTIES
  OUTPUT_NAME ${irif_lib_file}
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  ARCHIVE_OUTPUT_NAME "irif")

if(NOT ROCM_DEVICELIB_STANDALONE_BUILD)
  add_dependencies(irif llvm-link)
endif()
