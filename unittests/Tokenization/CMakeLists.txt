addTest("Protection:Tokenization:FilePath" filepath OBJECT_LIBS ${catch} LIBS protection)
addTest("Protection:Tokenization:Url" url OBJECT_LIBS ${catch} LIBS protection)
addTest("Protection:Tokenization:Sql" sql OBJECT_LIBS ${catch} LIBS protection)
addTest("Protection:Tokenization:Html" html OBJECT_LIBS ${catch} LIBS protection)
addTest("Protection:Tokenization:JavaScript" javascript OBJECT_LIBS ${catch} LIBS protection)

if(WIN32)
  add_custom_command(TARGET filepath.test POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy "${LIBPROTECTION_LIB}"
					 "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)")
endif()

