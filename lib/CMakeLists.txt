project(protection)

add_subdirectory(Support)
add_subdirectory(Languages)

add_library(${PROJECT_NAME} SHARED
        Range.cpp
        LanguageService.cpp
        LibProtectionEngine.cpp
        Languages/RegexLanguageProvider.cpp
        Languages/Token.cpp
        Languages/AntrlLanguageProvider.cpp
        Languages/RegexRule.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC FilePathLanguageProvider UrlLanguageProvider SqlLanguageProvider HtmlLanguageProvider JavaScriptLanguageProvider)

if(WIN32)
  set(LIBPROTECTION_LIB "${PROJECT_BINARY_DIR}/\$\(Configuration\)/protection.dll" PARENT_SCOPE)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PUBLIC antlr4_static)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC antlr4_shared)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt-header-only)

if(WIN32)
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION lib)
else()
  install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
endif()

