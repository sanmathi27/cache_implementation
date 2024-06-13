# If we don't need RTTI or EH, there's no reason to export anything
# from the Ass2 plugin.
if( NOT LLVM_REQUIRES_RTTI )
  if( NOT LLVM_REQUIRES_EH )
    set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Ass2.exports)
  endif()
endif()

if(WIN32 OR CYGWIN OR ZOS)
  set(LLVM_LINK_COMPONENTS Core Support)
endif()



add_llvm_library( LLVMAss2 MODULE BUILDTREE_ONLY
  Ass2.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
  target_link_libraries(LLVMAss2 PRIVATE ${llvm_libs})

  # Specify that it should be built as a shared library
set_target_properties(LLVMAss2 PROPERTIES
POSITION_INDEPENDENT_CODE ON
)