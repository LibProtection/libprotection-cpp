project(JavaScriptLanguageProvider)

set(namespace javascript)
set(lexer_grammar JavaScriptCpp)
antlr4_add_grammar(${namespace} ${lexer_grammar})

add_library(${PROJECT_NAME} STATIC JavaScript.cpp)
add_dependencies(${PROJECT_NAME} ${namespace})
target_include_directories(${PROJECT_NAME} PUBLIC ${antlr4cpp_include_dirs_${namespace}})
target_link_libraries(${PROJECT_NAME} ${namespace} WebUtils)
