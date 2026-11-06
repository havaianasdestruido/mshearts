## 2024-05-14 - Builtin popcount and std::sqrt optimizations
**Learning:** Old codebases like this one (Windows NT 5.1 port) often contain hand-rolled functions for tasks like square roots (`IntSqrt`) and bit counting (`CountBits`). These were originally written to avoid pulling in floating-point libraries or to work around compiler limitations of the era.
**Action:** When working with legacy codebases, replace hand-rolled math and bit-manipulation functions with standard library equivalents (`std::sqrt`) or compiler intrinsics (`__builtin_popcount`), as modern compilers and hardware execute these much faster (e.g., using `FSQRT` or `POPCNT` instructions).

## 2024-05-15 - Danger of Compiler Intrinsics in MSVC Codebases
**Learning:** Using GCC/Clang specific compiler intrinsics like `__builtin_popcount` in a legacy Win32/MFC codebase targeted for MSVC causes build regressions, as MSVC does not support this intrinsic out of the box.
**Action:** When optimizing legacy C++ codebases, avoid compiler-specific intrinsics unless platform/compiler conditional compilation macros are heavily used. For bit counting, use Brian Kernighan's algorithm (`x &= (x - 1)`) as a fast, cross-platform alternative that guarantees compatibility.
