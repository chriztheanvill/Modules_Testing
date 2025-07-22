module;

#include <fmt/core.h>
#include <iostream>

export module game;

// import engine;
import scenes;

export struct Game {
  // Engine _engine;
  SceneManager _scene_manager;
  Scene _scene {};

  auto init() -> void {
    fmt::print("Game.init()\n");
    // _engine.init();
    _scene_manager.init();

    // auto scene = Scene();
    _scene.init();
    _scene_manager.add_scene(&_scene);
  }
};