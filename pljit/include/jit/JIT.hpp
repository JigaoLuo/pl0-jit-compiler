#pragma once
//---------------------------------------------------------------------------
#include "ast/ASTNode.hpp"
#include "optimization/EvaluationContext.hpp"
#include <mutex>
#include <iostream>
#include <optional>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
class FunctionHandle;
//---------------------------------------------------------------------------
/// JIT-class: handles registering functions and their source code
class JIT {
    friend class FunctionHandle;
    private:
    /// The Register mutex.
    std::mutex register_mutex;
    /// The codes.
    std::vector<std::unique_ptr<SourceCodeManagement>> codes;

    /// The function's mutexes.
    std::vector<std::unique_ptr<std::mutex>> mutexes;
    /// The flag, if compiled.
    std::vector<bool> flags;
    /// The asts.
    std::vector<std::unique_ptr<FunctionAST>> asts;
    /// The Evaluation Contextes.
    std::vector<std::unique_ptr<EvaluationContext>> ecs;

    public:
    /// Constructor.
    JIT() = default;
    /// Register function returning function handle, which is only compiled when it is called.
    FunctionHandle RegisterFunction(const std::string& code);
};
//---------------------------------------------------------------------------
/// A function handle for just-in-time compilation.
class FunctionHandle {
    private:
    /// The JIT pointer.
    JIT* jit;
    /// The index inside of JIT>
    const size_t index;

    /// The function compilation.
    int Compile();

    public:
    /// Constructor.
    FunctionHandle(JIT* jit, size_t index);

    /// Call operator the call the function handle.
    template<typename... Parameters>
    std::optional<int64_t> operator()(Parameters... parameters) {
        {
            /// Check.
            std::scoped_lock lock_reg(jit->register_mutex);
            std::scoped_lock lock_fun(*jit->mutexes[index]);
            if (!jit->flags[index]) {
                if (this->Compile() != 0) {
                    return {};
                }
                jit->flags[index] = true;
            }
        }

        /// Set parameter's values.
        std::vector<unsigned> parameter_vector = {static_cast<unsigned>(parameters)...};
        EvaluationContext ec = *jit->ecs[index];
        size_t next_argument_index = 0;
        for (auto& entry : ec.GetValueTable()) {
            if (entry.second.GetType() == Symbol::Type::PARAMETER) {
                entry.second.SetValue(parameter_vector[next_argument_index++]);
            }
        }

        /// Run the function.
        const int64_t return_value = jit->asts[index]->Evaluate(ec);
        if (ec.GetDivisionByZero()) {
            std::cerr << "Division by zero error" << std::endl;
            return {};
        }
        return {return_value};
    }
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
