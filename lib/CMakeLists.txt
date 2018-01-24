project(protection)

add_subdirectory(Support)
add_subdirectory(Languages)

add_library(${PROJECT_NAME} SHARED
        Range.cpp
        LanguageService.cpp
        Languages/RegexLanguageProvider.cpp
        Languages/Token.cpp
        Languages/AntrlLanguageProvider.cpp
        Languages/RegexRule.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC FilePath Url Sql Html JavaScript antlr4-runtime)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt-header-only)
