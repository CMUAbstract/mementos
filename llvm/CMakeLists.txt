include_directories("include")

find_package(LLVM)

# Define add_llvm_* macro's.

# Workarounds for "Loadable modules not supported error"
# https://www.marc.info/?l=llvm-dev&m=142196366215513&w=2
set(LLVM_ENABLE_PLUGINS ON)
set(LLVM_PLUGIN_EXT ".so")
set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lib)

include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_subdirectory(lib/Mementos)
