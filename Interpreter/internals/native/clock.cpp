#include "internals/native/clock.hpp"

#include <chrono>


namespace native {

Clock::Clock(
) 
    : Call({}, {}, {})
{}

auto
Clock::operator()(
    [[maybe_unused]] Interpreter* interpreter,
    [[maybe_unused]] std::vector<Object>&& values
) -> ErrorOr<Object> {
    auto time = std::chrono::steady_clock::now();
    Object lox_time(static_cast<double>(
        std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count()
    ));
    return lox_time; 
}

}

native::Clock system_clock();
