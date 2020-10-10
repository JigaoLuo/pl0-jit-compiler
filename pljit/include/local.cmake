# ---------------------------------------------------------------------------
# Include Files
# ---------------------------------------------------------------------------

set(
        PLJIT_INCLUDES
        include/util/SourceCodeManagement.hpp
        include/util/SourceCodeReference.hpp
        include/util/Defer.hpp
        include/lexer/Token.hpp
        include/lexer/Lexer.hpp
        include/parser/ParseTreeNode.hpp
        include/parser/Parser.hpp
        include/parser/ParseTreeNodeVisitor.hpp
        include/parser/ParseTreeNodeVisitorDot.hpp
        include/ast/ASTNode.hpp
        include/ast/SymbolTable.hpp
        include/ast/SemanticAnalyzer.hpp
        include/ast/ASTNodeVisitorDot.hpp
        include/optimization/EvaluationContext.hpp
        include/optimization/OptimizationPass.hpp
        include/optimization/DeadCodeElimination.hpp
        include/optimization/ConstantPropagation.hpp
        include/jit/JIT.hpp
)
