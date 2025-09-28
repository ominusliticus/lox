#pragma once

#include <stdio.h>
#include <cstdio>

#include "util/error.hpp"
#include "util/print.hpp"


// TODO: Add error message to error message, once me move to fmtlib
// TODO: Add try clause for REPL: shouldn't fail if user makes a mistake
#define TRY(result)                                                                               \
    ({                                                                                            \
        auto xxpression{ result };                                                                \
        if (xxpression.is_error()) {                                                              \
            error(__FILE__, __LINE__, #result, xxpression.error());                               \
            return xxpression.error();                                                            \
        }                                                                                         \
        std::move(xxpression.release());                                                          \
    })

#define TRY_MAIN(result)                                                                          \
    ({                                                                                            \
        auto xxpression{ result };                                                                \
        if (xxpression.is_error()) {                                                              \
            printf("\033[31m[Error] %s:%s:%d\n\033[0m", __FILE__, #result, __LINE__);             \
            print("\033[31mExiting with error:", xxpression.error(),"\033[0m");                   \
            return 69;                                                                            \
        }                                                                                         \
        std::move(xxpression.release());                                                          \
    })

#define TRY_LOX(result, token)                                                                    \
    ({                                                                                            \
        auto xxpression{ result };                                                                \
        if (xxpression.is_error()) {                                                              \
            char error_msg[1024];                                                                 \
            std::sprintf(                                                                         \
                error_msg, "\033[31m[Error] %s:%s:%d\033[0m", __FILE__, #result, __LINE__         \
            );                                                                                    \
            error(token->line, token->column, xxpression.error(), error_msg);                     \
            return xxpression.error();                                                            \
        }                                                                                         \
        std::move(xxpression.release());                                                          \
    })

