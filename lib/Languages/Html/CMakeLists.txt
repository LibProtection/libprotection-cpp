project(HtmlLanguageProvider)

set(namespace html)
set(grammar_lexer HTMLLexer)
antlr4_add_grammar(${namespace} ${grammar_lexer})

add_library(${PROJECT_NAME} Html.cpp)
add_dependencies(${PROJECT_NAME} ${namespace})
target_include_directories(${PROJECT_NAME} PUBLIC ${antlr4cpp_include_dirs_${namespace}})
target_link_libraries(${PROJECT_NAME} ${namespace} UrlLanguageProvider WebUtils)
