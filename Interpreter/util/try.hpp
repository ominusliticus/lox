#pragma once

#include <stdio.h>

#include "print.hpp"

// TODO: Add error message to error message, once me move to fmtlib
// TODO: Add try clause for REPL: shouldn't fail if user makes a mistake
#define TRY(result)                                                                               \
    ({                                                                                            \
        auto expression{ result };                                                                \
        if (expression.is_error()) {                                                              \
            printf("\033[31m[Error] %s:%s:%d\n\033[0m", __FILE__, #result, __LINE__);             \
            return expression.error();                                                            \
        }                                                                                         \
        std::move(expression.release());                                                          \
    })

#define TRY_MAIN(result)                                                                          \
    ({                                                                                            \
        auto expression{ result };                                                                \
        if (expression.is_error()) {                                                              \
            printf("\033[31m[Error] %s:%s:%d\n\033[0m", __FILE__, #result, __LINE__);             \
            print("\033[31mExiting with error:", expression.error(),"\033[0m");                   \
            return 69;                                                                            \
        }                                                                                         \
        std::move(expression.release());                                                          \
    })
