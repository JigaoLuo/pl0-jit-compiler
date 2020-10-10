set(PLJIT_SOURCES
    # add your *.cpp files here
    util/SourceCodeManagement.cpp
    util/SourceCodeReference.cpp
    lexer/Token.cpp
    lexer/Lexer.cpp
    parser/ParseTreeNode.cpp
    parser/Parser.cpp
    parser/ParseTreeNodeVisitorDot.cpp
    ast/ASTNode.cpp
    ast/SymbolTable.cpp
    ast/SemanticAnalyzer.cpp
    ast/ASTNodeVisitorDot.cpp
    optimization/EvaluationContext.cpp
    optimization/DeadCodeElimination.cpp
    optimization/ConstantPropagation.cpp
    jit/JIT.cpp
    )

include("${CMAKE_SOURCE_DIR}/pljit/include/local.cmake")
include_directories(${CMAKE_SOURCE_DIR}/pljit/include)

add_library(pljit_core ${PLJIT_SOURCES} ${PLJIT_INCLUDES})
target_include_directories(pljit_core PUBLIC ${CMAKE_SOURCE_DIR})

add_clang_tidy_target(lint_pljit_core ${PLJIT_SOURCES})
add_dependencies(lint lint_pljit_core)

add_executable(pljit main.cpp)
target_link_libraries(pljit PUBLIC pljit_core Threads::Threads)
