##===--------------------------------------------------------------------------
##                   ROCm Device Libraries
##
## This file is distributed under the University of Illinois Open Source
## License. See LICENSE.TXT for details.
##===--------------------------------------------------------------------------

file(GLOB source_templates
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.ll.in
)

set(irif_lib_file ${CMAKE_CURRENT_BINARY_DIR}/irif.bc)

# find_program(LLVM_LINK_EXECUTABLE NAMES llvm-link PATHS ${LLVM_CONFIG_DIR} NO_DEFAULT_PATH)
# if(NOT LLVM_LINK_EXECUTABLE)
#   message(FATAL_ERROR "llvm-link not found")
# endif()
# add_executable(llvm-link IMPORTED)
# set_target_properties(llvm-link PROPERTIES IMPORTED_LOCATION ${LLVM_LINK_EXECUTABLE})

set(TARGET_TRIPLE ${AMDGPU_TARGET_TRIPLE})
if(TARGET_TRIPLE MATCHES "amdgcn.*")
  set(TARGET_DATALAYOUT "e-p:64:64-p1:64:64-p2:32:32-p3:32:32-p4:64:64-p5:32:32-p6:32:32-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-v2048:2048-n32:64-S32-A5")
elseif(TARGET_TRIPLE MATCHES "spirv64.*")
  # Two COMMANDs in one execute_process form a pipeline and run concurrently,
  # so clang can open empty.c before touch has created it. Create the file
  # first, in its own step.
  file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/empty.c)
  execute_process(
    COMMAND ${CLANG_ROOT_PATH_BIN}/clang -target ${AMDGPU_TARGET_TRIPLE}
    empty.c -emit-llvm -S -o -
    OUTPUT_VARIABLE code_output
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND_ERROR_IS_FATAL ANY)

  string(REGEX MATCH "target datalayout = \"[^ ]*\"" datalayout_stmt
    ${code_output})
  string(REGEX REPLACE "target datalayout = \"([^ ]*)\"" "\\1"
    TARGET_DATALAYOUT ${datalayout_stmt})
else()
  message(FATAL_ERROR "Don't know datalayout for '${TARGET_TRIPLE}' triple.")
endif()

foreach(template IN LISTS source_templates)
  get_filename_component(source_name ${template} NAME_WLE)
  set(source ${CMAKE_CURRENT_BINARY_DIR}/${source_name})
  configure_file(${template} ${source} @ONLY)
  list(APPEND sources ${source})
endforeach()

add_custom_command(OUTPUT ${irif_lib_file}
  COMMAND $<TARGET_FILE:llvm-link> ${sources} -o ${irif_lib_file}
  DEPENDS ${sources})

add_custom_target(irif DEPENDS ${irif_lib_file}
                       SOURCES ${sources})
set_target_properties(irif PROPERTIES
  OUTPUT_NAME ${irif_lib_file}
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  ARCHIVE_OUTPUT_NAME "irif")

if(NOT ROCM_DEVICELIB_STANDALONE_BUILD)
  add_dependencies(irif llvm-link)
endif()
