//---------------------------------------------------------------------------
#include "parser/ParseTreeNode.hpp"
#include "parser/ParseTreeNodeVisitor.hpp"
#include <vector>
#include <memory>
#include <string>
#include <string_view>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
ParseTreeNode::ParseTreeNode(Type type, const SourceCodeReference& source_code_reference) : type(type), source_code_reference(source_code_reference) {}
//---------------------------------------------------------------------------
ParseTreeNode::Type ParseTreeNode::GetType() const { return type; }
//---------------------------------------------------------------------------
const std::string ParseTreeNode::GetTypeAsString() const {
    switch (type) {
        case ParseTreeNode::Type::FunctionDefinition:
            return "FunctionDefinition";
        case ParseTreeNode::Type::ParameterDeclarations:
            return "ParameterDeclarations";
        case ParseTreeNode::Type::VariableDeclarations:
            return "VariableDeclarations";
        case ParseTreeNode::Type::ConstantDeclarations:
            return "ConstantDeclarations";
        case ParseTreeNode::Type::DeclaratorList:
            return "DeclaratorList";
        case ParseTreeNode::Type::InitDeclaratorList:
            return "InitDeclaratorList";
        case ParseTreeNode::Type::InitDeclarator:
            return "InitDeclarator";
        case ParseTreeNode::Type::CompoundStatement:
            return "CompoundStatement";
        case ParseTreeNode::Type::Statement:
            return "Statement";
        case ParseTreeNode::Type::StatementList:
            return "StatementList";
        case ParseTreeNode::Type::AssignmentExpression:
            return "AssignmentExpression";
        case ParseTreeNode::Type::AdditiveExpression:
            return "AdditiveExpression";
        case ParseTreeNode::Type::MultiplicativeExpression:
            return "MultiplicativeExpression";
        case ParseTreeNode::Type::UnaryExpression:
            return "UnaryExpression";
        case ParseTreeNode::Type::PrimaryExpression:
            return "PrimaryExpression";
        case ParseTreeNode::Type::Identifier:
            return "Identifier";
        case ParseTreeNode::Type::Literal:
            return "Literal";
        case ParseTreeNode::Type::GenericToken:
            return "GenericToken";
        default:
            __builtin_unreachable();
    }
}
//---------------------------------------------------------------------------
const SourceCodeReference& ParseTreeNode::GetSourceCodeReference() const { return source_code_reference;  }
//---------------------------------------------------------------------------
LiteralParseTreeNode::LiteralParseTreeNode(const SourceCodeReference& source_code_reference, int64_t value) : ParseTreeNode(ParseTreeNode::Type::Literal, source_code_reference), value(value) {}
//---------------------------------------------------------------------------
int64_t LiteralParseTreeNode::GetValue() const { return value; }
//---------------------------------------------------------------------------
void LiteralParseTreeNode::Accept(ParseTreeNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
IdentifierParseTreeNode::IdentifierParseTreeNode(const SourceCodeReference& source_code_reference) : ParseTreeNode(ParseTreeNode::Type::Identifier, source_code_reference), name(source_code_reference.GetSourceCode()) {}
//---------------------------------------------------------------------------
std::string_view IdentifierParseTreeNode::GetName() const { return name; }
//---------------------------------------------------------------------------
void IdentifierParseTreeNode::Accept(ParseTreeNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
GenericTokenParseTreeNode::GenericTokenParseTreeNode(const SourceCodeReference& source_code_reference) : ParseTreeNode(ParseTreeNode::Type::GenericToken, source_code_reference) {}
//---------------------------------------------------------------------------
void GenericTokenParseTreeNode::Accept(ParseTreeNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
OperatorAlternationParseTreeNode::OperatorAlternationParseTreeNode(const SourceCodeReference& source_code_reference, OperatorType operator_type) : GenericTokenParseTreeNode(source_code_reference), operator_type(operator_type) {}
//---------------------------------------------------------------------------
OperatorAlternationParseTreeNode::OperatorType OperatorAlternationParseTreeNode::GetOperatorType() const { return operator_type; }
//---------------------------------------------------------------------------
NonTerminalParseTreeNode::NonTerminalParseTreeNode(const SourceCodeReference& source_code_reference, ParseTreeNode::Type type, std::vector<std::unique_ptr<ParseTreeNode>> children) : ParseTreeNode(type, source_code_reference), children(std::move(children)) {}
//---------------------------------------------------------------------------
const std::vector<std::unique_ptr<ParseTreeNode>>& NonTerminalParseTreeNode::GetChildren() const { return children; }
//---------------------------------------------------------------------------
void NonTerminalParseTreeNode::Accept(ParseTreeNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
