#pragma once
//---------------------------------------------------------------------------
#include "optimization/EvaluationContext.hpp"
#include <string>
#include <string_view>
#include <memory>
#include <vector>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// Forward declared for AST's visitor.
class ASTNodeVisitor;
//---------------------------------------------------------------------------
/// The Node of AST (abstract syntax tree): classes that represent statements, expressions, and function and subclasses for different kinds of statements and expressions.
class ASTNode {
    public:
    /// The types of AST Nodes.
    enum class Type {
        LiteralPrimaryExpression,
        IdentifierPrimaryExpression,
        UnaryExpression,
        BinaryExpression,
        AssignmentStatement,
        ReturnStatement,
        Function
    };
    /// Constructor.
    explicit ASTNode(Type type);
    /// Destructor.
    virtual ~ASTNode() = default;
    /// Get the type.
    [[nodiscard]] Type GetType() const;
    /// Accept function for the visitor.
    virtual void Accept(ASTNodeVisitor&) = 0;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext&) const = 0;

    private:
    /// The AST node type.
    const Type type;
};
//---------------------------------------------------------------------------
/// The AST Node for expression.
class ExpressionAST : public ASTNode {
    public:
    /// Constructor.
    explicit ExpressionAST(Type type);
    /// Destructor.
    ~ExpressionAST() override = default;
};
//---------------------------------------------------------------------------
/// The AST Node for statement.
class StatementAST : public ASTNode {
    public:
    /// Constructor.
    explicit StatementAST(Type type);
    /// Destructor.
    ~StatementAST() override = default;
};
//---------------------------------------------------------------------------
/// The AST Node for function.
class FunctionAST : public ASTNode {
    public:
    /// Constructor.
    explicit FunctionAST(std::vector<std::unique_ptr<StatementAST>> children);
    /// Get children.
    const std::vector<std::unique_ptr<StatementAST>>& GetChildren() const;
    /// Eliminate the last child.
    void EliminateLastChild();
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;

    private:
    /// The statements as children.
    std::vector<std::unique_ptr<StatementAST>> children;
};
//---------------------------------------------------------------------------
/// Grammar:
/// primary-expression = identifier
///                    | literal
///                    | "(" additive-expression ")".
class LiteralPrimaryExpressionAST : public ExpressionAST {
    public:
    /// Constructor.
    explicit LiteralPrimaryExpressionAST(int64_t value);
    /// Get the literal value.
    [[nodiscard]] int64_t GetValue() const;
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;

    private:
    /// The literal value.
    const int64_t value;
};
//---------------------------------------------------------------------------
/// Grammar:
/// primary-expression = identifier
///                    | literal
///                    | "(" additive-expression ")".
class IdentifierPrimaryExpressionAST : public ExpressionAST {
    public:
    /// Constructor.
    explicit IdentifierPrimaryExpressionAST(std::string_view name);
    /// Get the identifier name.
    [[nodiscard]] std::string_view GetName() const;
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;

    private:
    /// The identifier name.
    const std::string_view name;
};
//---------------------------------------------------------------------------
/// Grammar:
/// unary-expression = [ "+" | "-" ] primary-expression.
class UnaryExpressionAST : public ExpressionAST {
    public:
    /// The Unary Operator, aka the sign: +, -.
    enum UnaryOperator {
        POSTIVE,
        NEGATIVE
    };
    /// Constructor
    explicit UnaryExpressionAST(UnaryOperator op, std::unique_ptr<ASTNode> child);
    /// Get the operator type.
    [[nodiscard]] UnaryOperator GetUnaryOperatorType() const;
    /// Get child.
    const std::unique_ptr<ASTNode>& GetChild() const;
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;

    private:
    /// The Unary Operator.
    const UnaryOperator op;
    /// The child: primary-expression.
    const std::unique_ptr<ASTNode> child;
};
//---------------------------------------------------------------------------
/// Grammar:
/// additive-expression = multiplicative-expression [ ( "+" | "-" ) additive-expression ].
/// multiplicative-expression = unary-expression [ ( "*" | "/" ) multiplicative-expression ].
class BinaryExpressionAST : public ExpressionAST {
    public:
    /// The Binary Operator: +, -, *, /.
    enum BinaryOperator {
        PLUS,
        MINUS,
        MUL,
        DIV
    };
    /// Constructor
    explicit BinaryExpressionAST(BinaryOperator op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);
    /// Get the operator type.
    [[nodiscard]] BinaryOperator GetBinaryOperatorType() const;
    /// Get the left child.
    const std::unique_ptr<ASTNode>& GetLeftChild() const;
    /// Get the right child.
    const std::unique_ptr<ASTNode>& GetRightChild() const;
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;

    private:
    /// The Binary Operator
    const BinaryOperator op;
    /// The left child.
    const std::unique_ptr<ASTNode> left;
    /// The right child.
    const std::unique_ptr<ASTNode> right;
};
//---------------------------------------------------------------------------
/// Grammar:
/// statement = assignment-expression           <- defined as `AssignmentStatement`.
///           | "RETURN" additive-expression.   <- defined as `ReturnStatement`.
///
/// assignment-expression = identifier ":=" additive-expression.
class AssignmentStatementAST : public StatementAST {
    public:
    /// Constructor
    explicit AssignmentStatementAST(std::unique_ptr<IdentifierPrimaryExpressionAST> identifier, std::unique_ptr<ASTNode> expression);
    /// Get the identifier.
    const std::unique_ptr<IdentifierPrimaryExpressionAST>& GetIdentifier() const;
    /// Get the expression.
    const std::unique_ptr<ASTNode>& GetExpression() const;
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;
    /// Optimize with constant replacement.
    void SetToConstantLiteral(int64_t value);

    private:
    /// The identifier.
    const std::unique_ptr<IdentifierPrimaryExpressionAST> identifier;
    /// The expression: non-const, possible be replace in the optimization pass: constant propagation.
    std::unique_ptr<ASTNode> expression;
};

//---------------------------------------------------------------------------
/// Grammar:
/// statement = assignment-expression           <- defined as `AssignmentStatement`.
///           | "RETURN" additive-expression.   <- defined as `ReturnStatement`.
class ReturnStatementAST : public StatementAST {
    public:
    /// Constructor
    explicit ReturnStatementAST(std::unique_ptr<ASTNode> expression);
    /// Get the expression.
    const std::unique_ptr<ASTNode>& GetExpression() const;
    /// Accept function for the visitor.
    void Accept(ASTNodeVisitor& v) override;
    /// Evaluate the node.
    virtual int64_t Evaluate(EvaluationContext& ec) const override;
    /// Optimize with constant replacement.
    void SetToConstantLiteral(int64_t value);

    private:
    /// The expression: non-const, possible be replace in the optimization pass: constant propagation.
    std::unique_ptr<ASTNode> expression;
};
//---------------------------------------------------------------------------
}  // namespace pljit
//---------------------------------------------------------------------------
