#pragma once
//---------------------------------------------------------------------------
#include <functional>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// Defer a function call.
struct Defer {
    /// The deferred function.
    std::function<void()> fn;

    /// Constructor.
    explicit Defer(std::function<void()> fn) : fn(std::move(fn)) {}

    /// Destructor.
    /// Calls the deferred function.
    ~Defer() { fn(); }

    /// Runs the deferred function.
    void run() { fn(); fn = [](){}; }
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------