#include <filesystem>
#include <fstream>
#include <iostream>

#include "util/print.hpp"
#include "util/error.hpp"
#include "util/try.hpp"

// Runs lox interpreter on file
auto 
run_file(
    char const* file_name
) -> ErrorOr<void> {
    std::filesystem::path file{ file_name };
    if (std::filesystem::exists(file)) {
        return {};
    }
    else { 
        return ErrorType::FILE_NOT_FOUND;
    }
}

// Runs lox interpreter on command line
auto
run_prompt(
) -> ErrorOr<void> {
    return {};
}

auto
main(
    [[maybe_unused]] int argc, 
    [[maybe_unused]] char **argv
) -> int {
    if (argc > 2) {
        print("Usage: ilox [script]");
        exit(64);
    } else if (argc == 2) {
        TRY_MAIN(run_file(argv[1]));
    }
    else {
        TRY_MAIN(run_prompt());
    }

   
    return 0;
}


