module;
#include <fmt/core.h>

export module scenes:scene;

export struct SceneManager;

export struct Scene {
  Scene() { }
  std::string _name {};
  SceneManager* _scene_manager { nullptr };

  auto init() -> void {
    fmt::print("Scene.init()\n");
    _name = "DefaultScene";
  }
};