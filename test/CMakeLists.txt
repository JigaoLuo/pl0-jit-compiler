set(TEST_SOURCES
    # add your *.cpp files here
    Tester.cpp
    TestLexer.cpp
    TestParser.cpp
    TestParserDot.cpp
    TestAST.cpp
    TestASTDot.cpp
    TestOptimizationEvaluation.cpp
    TestOptimizationDeadCodeElimination.cpp
    TestOptimizationConstantPropagation.cpp
    TestJIT.cpp
    )

include("${CMAKE_SOURCE_DIR}/pljit/include/local.cmake")
include_directories(${CMAKE_SOURCE_DIR}/pljit/include)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(tester PUBLIC pljit_core GTest::GTest Threads::Threads)
