module;

#include <fmt/core.h>
#include <string>
#include <unordered_map>

export module scenes:scene_manager;

export struct Scene;

export struct SceneManager {
  SceneManager();
  Scene* _current_scene { nullptr };
  std::unordered_map<std::string, Scene*> _scene_map;

  auto init() -> void;
  auto add_scene(Scene* scene) -> void;
  auto set_scene(std::string name) -> void;
  auto delete_scene(std::string name) -> void;
};