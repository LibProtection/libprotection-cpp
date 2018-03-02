addTest("Protection:Formats" formats OBJECT_LIBS ${catch} LIBS protection)

if(WIN32)
  add_custom_command(TARGET formats.test POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy "${LIBPROTECTION_LIB}"
					 "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)")
endif()
