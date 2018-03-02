project(UrlLanguageProvider)
add_library(${PROJECT_NAME} Url.cpp)
target_link_libraries(${PROJECT_NAME} WebUtils)