#include "jit/JIT.hpp"
#include "parser/Parser.hpp"
#include "ast/SemanticAnalyzer.hpp"
#include "optimization/DeadCodeElimination.hpp"
#include "optimization/ConstantPropagation.hpp"
#include <cassert>
#include <mutex>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
FunctionHandle::FunctionHandle(JIT* jit, size_t index) : jit(jit), index(index) {}
//---------------------------------------------------------------------------
FunctionHandle JIT::RegisterFunction(const std::string& code) {
    std::scoped_lock lock(register_mutex);
    const size_t index = codes.size();
    assert(flags.size() == codes.size());
    assert(asts.size() == codes.size());
    assert(mutexes.size() == codes.size());
    assert(ecs.size() == codes.size());

    codes.emplace_back(std::make_unique<SourceCodeManagement>(code));
    mutexes.emplace_back(std::make_unique<std::mutex>());
    flags.emplace_back(false);
    asts.emplace_back(nullptr);
    ecs.emplace_back(nullptr);

    return FunctionHandle(this, index);
}
//---------------------------------------------------------------------------
int FunctionHandle::Compile() {
    Parser parser(*jit->codes[index]);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    if(!parse_tree) { return 1; }
    SemanticAnalyzer semantic_analyzer;
    jit->asts[index] = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    if (!jit->asts[index]) { return 2; }
    jit->ecs[index] = std::make_unique<EvaluationContext>(semantic_analyzer.GetSymbolTable());
    OptimizeDeadCode opc;
    opc.Optimize(*jit->asts[index]);
    ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
    cp.Optimize(*jit->asts[index]);
    return 0;
}
} // namespace pljit
//---------------------------------------------------------------------------