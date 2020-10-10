#pragma once
//---------------------------------------------------------------------------
#include "parser/ParseTreeNode.hpp"
#include "ast/SymbolTable.hpp"
#include "ast/ASTNode.hpp"
#include <vector>
#include <unordered_map>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// (The class name: SemanticAnalyzer is not specified.)
/// The task of this class is mainly the semantic analysis:
///     - to traverse the existing parse tree as an input provided by parser and lexer.
///     - to find and report semantic errors in the program, like using an undeclared identifier.
///       Those errors cannot be prevented by only enforcing the grammar with a symbol table,  which is why the parser will not find them.
///     - to generate an AST as an output.
class SemanticAnalyzer {
    public:
    /// The publicly exposed function to traverse the parse tree and to generate an AST.
    /// This function is implemented by calling other private functions that take a node from the parse tree,
    /// recursively call each other with the child nodes, and return the generated AST nodes.
    std::unique_ptr<FunctionAST> AnalyzeParseTree(std::unique_ptr<NonTerminalParseTreeNode> parse_tree_root);

    /// Get the symbol table.
    const SymbolTable& GetSymbolTable() const;

    private:
    /// A symbol table keeps track of all identifiers.
    SymbolTable symbol_table;

    /// The function for analyze a statement of a parse tree and the generation of an AST node.
    /// @return unique pointer to AssignmentStatementAST or ReturnStatementAST. nullptr_t if failure (using undeclared identifiers or assignment to constants).
    std::unique_ptr<StatementAST> AnalyzeStatement(NonTerminalParseTreeNode* node);
    /// The function for analyze an additive expression of a parse tree and the generation of an AST node.
    std::unique_ptr<ExpressionAST> AnalyzeAdditiveExpression(NonTerminalParseTreeNode* node);
    /// The function for analyze a multiplicative expression of a parse tree and the generation of an AST node.
    std::unique_ptr<ExpressionAST> AnalyzeMultiplicativeExpression(NonTerminalParseTreeNode* node);
    /// The function for analyze declarations (parameter-declarations, variable-declarations actually) of a parse tree and the generation of an AST node.
    /// @return True for success, false for failure (Duplicate identifier name found).
    bool AnalyzeDeclarations(const NonTerminalParseTreeNode* const node, Symbol::Type type);
    /// The function for analyze constant declarations of a parse tree and the generation of an AST node.
    /// @return True for success, false for failure (Duplicate identifier name found).
    bool AnalyzeConstDeclarations(const NonTerminalParseTreeNode* const node);
};
//---------------------------------------------------------------------------
} //namespace pljit
//---------------------------------------------------------------------------
