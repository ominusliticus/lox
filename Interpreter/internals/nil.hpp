#pragma once

struct Nil{
};

inline auto
operator==(
    Nil const&,
    Nil const&
) -> bool {
    return true;
}

static Nil nil = {};

