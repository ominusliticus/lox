#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "util/error.hpp"
#include "util/print.hpp"
#include "util/try.hpp"

#include "internals/token.hpp"

// Reads and lexes file
auto 
run(
    std::istream&& buf
) -> ErrorOr<void> {
    (void)buf;
    return {};
}

// Runs lox interpreter on file
auto 
run_file(
    char const *file_name
) -> ErrorOr<void> {
    std::filesystem::path file_path{file_name};
    if (std::filesystem::exists(file_path)) {
        std::fstream file_stream{ file_path };
        std::istream buf{ file_stream.rdbuf() };
        return run(std::move(buf));
    } else {
        return ErrorType::FILE_NOT_FOUND;
    }
    return ErrorType::UNREACHABLE;
}

// Runs lox interpreter on command line
auto
run_prompt(
) -> ErrorOr<void> { 
    std::string line;
    while (true) {
        printf("ilox> ");
        if (!std::getline(std::cin, line) && !std::cin.eof()) return ErrorType::INVALID_COMMAND;
        else if (line == "exit()" || std::cin.eof()) break;
        std::stringstream buf{ std::move(line) };
        TRY(run(std::move(buf)));
    }
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
    } else {
        TRY_MAIN(run_prompt());
    }

  return 0;
}

