module;

#include <fmt/core.h>
// #include <fmt/ranges.h>
#include <iostream>

export module engine;

export struct Engine {
  auto init() -> void { fmt::print("Engine.init()\n"); }
};