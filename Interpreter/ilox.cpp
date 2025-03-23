#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "internals/ast.hpp"
#include "internals/nil.hpp"
#include "internals/object.hpp"
#include "internals/parse.hpp"
#include "internals/scanner.hpp"
#include "internals/token.hpp"

#include "util/error.hpp"
#include "util/print.hpp"
#include "util/try.hpp"

// TODO: Profile the number of string copies, where can we use std::string_view?

// Reads and lexes file
auto 
run(
    std::string source
) -> ErrorOr<void> {
    Scanner scanner(source);
    std::vector<Token> tokens{ TRY(scanner.scan_tokens()) };
    for (auto const& token : tokens) 
        print(token);
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
        auto size{ std::filesystem::file_size(file_path) }; 
        file_stream.seekg(0);
        std::string source(size, '\0');
        print(size, source);
        file_stream.read(&source[0], size);
        return run(source);
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
        // A hack to keep REPL from crashing due to user error
        [[maybe_unused]] auto _ = [=]() -> ErrorOr<void> { TRY(run(line)); return {}; }();
    }
    return {};
}

auto
main(
    [[maybe_unused]] int    argc,
    [[maybe_unused]] char **argv
) -> int {
    if (argc > 2) {
        print("Usage: ilox [script]");
        exit(69);
    } else if (argc == 2) {
        TRY_MAIN(run_file(argv[1]));
    } else {
        TRY_MAIN(run_prompt());
    }

  return 0;
}

