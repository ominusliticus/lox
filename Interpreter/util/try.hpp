#pragma once

#include <stdio.h>
#include <cstdio>

#include "util/error.hpp"
#include "util/decorate_error.hpp"
#include "util/print.hpp"


// TODO: Add error message to error message, once me move to fmtlib
// TODO: Add try clause for REPL: shouldn't fail if user makes a mistake
            // error(__FILE__, __LINE__, #result, xxpression.error());                      
#define TRY(result)                                                                               \
    ({                                                                                            \
        auto xxpression{ result };                                                                \
        if (xxpression.is_error()) {                                                              \
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

// We need to increment the lexeme_count by one since floor(log10(2)) = 0 but has one digit
#define TRY_LOX(result, token)                                                                    \
    ({                                                                                            \
        auto xxpression{ result };                                                                \
        if (xxpression.is_error()) {                                                              \
            char error_msg[1024];                                                                 \
            std::sprintf(                                                                         \
                error_msg, "\033[31m[Error] %s:%s:%d\033[0m", __FILE__, #result, __LINE__         \
            );                                                                                    \
            print(*token);                                                                        \
            std::size_t lexeme_size = token->lexeme.size();                                       \
            decorate_error(token->filename, token->line, token->column, lexeme_size, error_msg);  \
            return xxpression.error();                                                            \
        }                                                                                         \
        std::move(xxpression.release());                                                          \
    })

