# PL/0 JIT Compiler

A tiny [PL/0](https://en.wikipedia.org/wiki/PL/0) JIT compiler's frontend.

## Dependency:

- [CMake](https://cmake.org/)
- [Clang-tidy](https://clang.llvm.org/extra/clang-tidy/)

## Build

A configuration file is provided to build this project with CMake. In the project directory run:

```bash
$ mkdir -p build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
```

## Milestone

### Milestone 1: Source Code Stuff
- [SourceCodeManagement.hpp](pljit/include/util/SourceCodeManagement.hpp)
- [SourceCodeManagement.cpp](pljit/util/SourceCodeManagement.cpp)
- [SourceCodeReference.hpp](pljit/include/util/SourceCodeReference.hpp)
- [SourceCodeReference.cpp](pljit/util/SourceCodeReference.cpp)
    
### Milestone 2: Lexer
- [Defer.hpp](pljit/include/util/Defer.hpp)
- [Token.hpp](pljit/include/lexer/Token.hpp)
- [Token.cpp](pljit/lexer/Token.cpp)
- [Lexer.hpp](pljit/include/lexer/Lexer.hpp)
- [Lexer.cpp](pljit/lexer/Lexer.cpp)
- [TestLexer.cpp](test/TestLexer.cpp)
    
### Milestone 3: Parser (a parse tree)
- [ParseTreeNode.hpp](pljit/include/parser/ParseTreeNode.hpp)
- [ParseTreeNode.cpp](pljit/parser/ParseTreeNode.cpp)
- [Parser.hpp](pljit/include/parser/Parser.hpp)
- [Parser.cpp](pljit/parser/Parser.cpp)
- [ParseTreeVisitor.hpp](pljit/include/parser/ParseTreeNodeVisitor.hpp)
- [ParseTreeVisitorDot.hpp](pljit/include/parser/ParseTreeNodeVisitorDot.hpp)
- [ParseTreeVisitorDot.cpp](pljit/parser/ParseTreeNodeVisitorDot.cpp)
- [TestParser.cpp](test/TestParser.cpp)
- [TestParserDot.cpp](test/TestParserDot.cpp)

### Milestone 4: AST
- [ASTNode.hpp](pljit/include/ast/ASTNode.hpp)
- [ASTNode.cpp](pljit/ast/ASTNode.cpp)
- [SymbolTable.hpp](pljit/include/ast/SymbolTable.hpp)
- [SymbolTable.cpp](pljit/ast/SymbolTable.cpp)
- [SemanticAnalyzer.hpp](pljit/include/ast/SemanticAnalyzer.hpp)
- [SemanticAnalyzer.cpp](pljit/ast/SemanticAnalyzer.cpp)
- [ASTNodeVisitor.hpp](pljit/include/ast/ASTNodeVisitor.hpp)
- [ASTNodeVisitorDot.hpp](pljit/include/ast/ASTNodeVisitorDot.hpp)
- [ASTNodeVisitorDot.cpp](pljit/ast/ASTNodeVisitorDot.cpp)
- [TestAST.cpp](test/TestAST.cpp)
- [TestASTDot.cpp](test/TestASTDot.cpp)
    
### Milestone 5: Optimization Passes
- [EvaluationContext.hpp](pljit/include/optimization/EvaluationContext.hpp)
- [EvaluationContext.cpp](pljit/optimization/EvaluationContext.cpp)
- [OptimizationPass.hpp](pljit/include/optimization/OptimizationPass.hpp)
- [DeadCodeElimination.hpp](pljit/include/optimization/DeadCodeElimination.hpp)
- [DeadCodeElimination.cpp](pljit/optimization/DeadCodeElimination.cpp)
- [ConstantPropagation.hpp](pljit/include/optimization/ConstantPropagation.hpp)
- [ConstantPropagation.cpp](pljit/optimization/ConstantPropagation.cpp)
- [TestOptimizationEvaluation.cpp](test/TestOptimizationEvaluation.cpp)
- [TestOptimizationDeadCodeElimination.cpp](test/TestOptimizationDeadCodeElimination.cpp)
- [TestOptimizationConstantPropagation.cpp](test/TestOptimizationConstantPropagation.cpp)

### Milestone 6: JIT
- [JITFunction.hpp](pljit/include/jit/JITFunction.hpp)
- [JITFunction.cpp](pljit/jit/JITFunction.cpp)
- [JIT.hpp](pljit/include/jit/JIT.hpp)
- [JIT.cpp](pljit/jit/JIT.cpp)
- [TestJIT.cpp](test/TestJIT.cpp)
