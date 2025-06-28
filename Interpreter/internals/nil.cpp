#include "internals/nil.hpp"

auto
operator==(
    Nil const&,
    Nil const&
) -> bool {
    return true;
}
