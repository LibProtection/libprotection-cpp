project(UrlLanguageProvider)
add_library(${PROJECT_NAME} STATIC Url.cpp)
target_link_libraries(${PROJECT_NAME} WebUtils)