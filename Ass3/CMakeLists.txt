# If we don't need RTTI or EH, there's no reason to export anything
# from the Ass3 plugin.
if( NOT LLVM_REQUIRES_RTTI )
  if( NOT LLVM_REQUIRES_EH )
    set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Ass3.exports)
  endif()
endif()

if(WIN32 OR CYGWIN OR ZOS)
  set(LLVM_LINK_COMPONENTS Core Support)
endif()



add_llvm_library( LLVMAss3 MODULE BUILDTREE_ONLY
  Ass3.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
  target_link_libraries(LLVMAss3 PRIVATE ${llvm_libs})

  # Specify that it should be built as a shared library
set_target_properties(LLVMAss3 PROPERTIES
POSITION_INDEPENDENT_CODE ON
)