#pragma once
//---------------------------------------------------------------------------
#include "util/SourceCodeReference.hpp"
#include <vector>
#include <memory>
#include <string>
#include <string_view>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// Forward declaration of Visitor.
class ParseTreeNodeVisitor;
//---------------------------------------------------------------------------
/// The Node of the PL Parse Tree.
class ParseTreeNode {
    public:
    /// All possible types of Nodes of the PL Parse Tree.
    /// To distinguish to Token, not use all upper case.
    enum class Type {
        Identifier,
        Literal,
        GenericToken,
        FunctionDefinition,
        ParameterDeclarations,
        VariableDeclarations,
        ConstantDeclarations,
        DeclaratorList,
        InitDeclaratorList,
        InitDeclarator,
        CompoundStatement,
        StatementList,
        Statement,
        AssignmentExpression,
        AdditiveExpression,
        MultiplicativeExpression,
        UnaryExpression,
        PrimaryExpression
    };

    /// Constructor.
    ParseTreeNode(Type type, const SourceCodeReference& source_code_reference);
    /// Destructor.
    virtual ~ParseTreeNode() = default;
    /// Get the type of the Parse Tree Node.
    [[nodiscard]] Type GetType() const;
    /// Get the type as string of the Parse Tree Node.
    [[nodiscard]] const std::string GetTypeAsString() const;
    /// Get the source code reference.
    const SourceCodeReference& GetSourceCodeReference() const;
    /// Accept function for the visitor.
    virtual void Accept(ParseTreeNodeVisitor&) = 0;

    private:
    /// The type of the Parse Tree Node.
    const Type type;
    protected:
    /// The source code reference.
    const SourceCodeReference source_code_reference;
};
//---------------------------------------------------------------------------
class LiteralParseTreeNode : public ParseTreeNode {
    public:
    /// Constructor.
    LiteralParseTreeNode(const SourceCodeReference& source_code_reference, int64_t value);
    /// Get the literal value.
    [[nodiscard]] int64_t GetValue() const;
    /// Accept function for the visitor.
    void Accept(ParseTreeNodeVisitor& v) override;

    private:
    const int64_t value;
};
//---------------------------------------------------------------------------
class IdentifierParseTreeNode : public ParseTreeNode {
    public:
    /// Constructor.
    explicit IdentifierParseTreeNode(const SourceCodeReference& source_code_reference);
    /// Get the identifier name.
    [[nodiscard]] std::string_view GetName() const;
    /// Accept function for the visitor.
    void Accept(ParseTreeNodeVisitor& v) override;

    private:
    const std::string_view name;
};
//---------------------------------------------------------------------------
class GenericTokenParseTreeNode : public ParseTreeNode {
    public:
    /// Constructor.
    explicit GenericTokenParseTreeNode(const SourceCodeReference& source_code_reference);
    /// Accept function for the visitor.
    void Accept(ParseTreeNodeVisitor& v) override;
};
//---------------------------------------------------------------------------
class OperatorAlternationParseTreeNode : public GenericTokenParseTreeNode {
    public:
    enum OperatorType {
        Plus,
        Minus,
        Multiply,
        Divide
    };
    /// Constructor.
    OperatorAlternationParseTreeNode(const SourceCodeReference& source_code_reference, OperatorType operator_type);
    /// Get the operator type.
    [[nodiscard]] OperatorType GetOperatorType() const;

    private:
    const OperatorType operator_type;
};
//---------------------------------------------------------------------------
class NonTerminalParseTreeNode : public ParseTreeNode {
    public:
    /// Constructor.
    NonTerminalParseTreeNode(const SourceCodeReference& source_code_reference, ParseTreeNode::Type type, std::vector<std::unique_ptr<ParseTreeNode>> children);
    /// Get children.
    const std::vector<std::unique_ptr<ParseTreeNode>>& GetChildren() const;
    /// Accept function for the visitor.
    void Accept(ParseTreeNodeVisitor& v) override;

    private:
    /// The children of the Non-Terminal node.
    const std::vector<std::unique_ptr<ParseTreeNode>> children;
};
//---------------------------------------------------------------------------
} //namespace pljit
//---------------------------------------------------------------------------
