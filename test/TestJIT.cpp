#include "jit/JIT.hpp"
#include <thread>
#include <vector>
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
TEST(JIT, NoParameterTest0) {
    const std::string code = "BEGIN\n"
                             "    RETURN 12 * (8 - 5)\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    EXPECT_EQ(func(), 12 * (8 - 5));
}
//---------------------------------------------------------------------------
TEST(JIT, NoParameterTest1) {
    const std::string code = "BEGIN\n"
                             "    RETURN +42 + -42\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    EXPECT_EQ(func(), +42 + -42);
}
//---------------------------------------------------------------------------
TEST(JIT, NoParameterTest2) {
    const std::string code = "PARAM pa, pb;\n"
                             "VAR va, vb;\n"
                             "CONST ca=1, cb=2;\n"
                             "BEGIN\n"
                             "    RETURN ca + cb\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    EXPECT_EQ(func(1, 1), 1 + 2);
}
//---------------------------------------------------------------------------
TEST(JIT, NoParameterTest3) {
    const std::string code = "PARAM width, height;\n"
                             "VAR temp, foo;\n"
                             "CONST hello = 12, test = 2000;\n"
                             "BEGIN\n"
                             "    RETURN hello * (8 - 5)\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    EXPECT_EQ(func(1, 1), 12 * (8 - 5));
}
//---------------------------------------------------------------------------
TEST(JIT, NoParameterTest4) {
    const std::string code = "PARAM a, b, c;\n"
                             "VAR d, e, f;\n"
                             "CONST g=1, h=2, i=3;\n"
                             "BEGIN\n"
                             "    a := a + 1;\n"
                             "    b := b + a;\n"
                             "    c := c + b;\n"
                             "    d := a * b / c;\n"
                             "    e := d;\n"
                             "    f := 3 * e;\n"
                             "    RETURN g * h * i\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    EXPECT_EQ(func(1, 1, 1), 1 * 2 * 3);
}
//---------------------------------------------------------------------------
TEST(JIT, ParameterTest0) {
    const std::string code = "PARAM pa, pb;\n"
                             "VAR va, vb;\n"
                             "CONST ca=1, cb=2;\n"
                             "BEGIN\n"
                             "    RETURN pa + pb + ca + cb\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    for (int64_t param0 = 0, param1 = 15; param0 < 100; param0++, param1++) {
        EXPECT_EQ(func(param0, param1), [](int64_t pa, int64_t pb){
          return pa + pb + 1 + 2;
        }(param0, param1));
    }
}
//---------------------------------------------------------------------------
TEST(JIT, ParameterTest1) {
    const std::string code = "PARAM width, height;\n"
                             "VAR temp, foo;\n"
                             "CONST hello = 12, test = 2000;\n"
                             "BEGIN\n"
                             "    RETURN width * height + hello * (8 - 5)\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    for (int64_t param0 = 0, param1 = 15; param0 < 100; param0++, param1++) {
        EXPECT_EQ(func(param0, param1), [](int64_t width, int64_t height){
          return width * height + 12 * (8 - 5);
        }(param0, param1));
    }
}
//---------------------------------------------------------------------------
TEST(JIT, ParameterTest2) {
    const std::string code = "PARAM a;\n"
                             "VAR b, c, d;\n"
                             "CONST e = 1;\n"
                             "BEGIN\n"
                             "    b := 1 + 2;\n"
                             "    c := b + e;\n"
                             "    d := b + c + e;\n"
                             "    RETURN a * d + 1 * 2 - 2 / 1\n"
                             "END.";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    for (int64_t param = 0; param < 100; param++) {
        EXPECT_EQ(func(param), [](int64_t a){
            const int64_t e = 1;
            int64_t b = 1 + 2;
            int64_t c = b + e;
            int64_t d = b + c + e;
            return a * d + 1 * 2 - 2 / 1;
        }(param));
    }
}
//---------------------------------------------------------------------------
TEST(JIT, MultithreadingNoParameterTest) {
    const std::string code = "BEGIN\n"
                             "    RETURN 12 * (8 - 5)\n"
                             "END.\n";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    std::vector<std::thread> threads;
    for (size_t i = 0; i < 100; i++) {
        threads.emplace_back([&func, i]() { EXPECT_EQ(func(), 12 * (8 - 5)); });
    }
    for (auto& t : threads) {
        t.join();
    }
}
//---------------------------------------------------------------------------
TEST(JIT, MultithreadingParameterTest) {
    const std::string code = "PARAM a;\n"
                             "VAR b, c, d;\n"
                             "CONST e = 1;\n"
                             "BEGIN\n"
                             "    b := 1 + 2;\n"
                             "    c := b + e;\n"
                             "    d := b + c + e;\n"
                             "    RETURN a * d + 1 * 2 - 2 / 1\n"
                             "END.";
    JIT jit;
    auto func = jit.RegisterFunction(code);
    std::vector<std::thread> threads;
    for (size_t i = 0; i < 100; i++) {
        threads.emplace_back([&func, i]() {
          EXPECT_EQ(func(i), [](int64_t a){
            const int64_t e = 1;
            int64_t b = 1 + 2;
            int64_t c = b + e;
            int64_t d = b + c + e;
            return a * d + 1 * 2 - 2 / 1;
          }(i));
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
