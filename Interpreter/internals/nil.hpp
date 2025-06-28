#pragma once

struct Nil{
};

bool operator==(Nil const&, Nil const&);

static Nil nil = {};

