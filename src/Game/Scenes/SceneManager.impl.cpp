module;
#include <fmt/core.h>

// import scenes:scene_manager;
module scenes:scene_manager;

import :scene;

SceneManager::SceneManager() { }

auto SceneManager::init() -> void { fmt::print("SceneManager.init()\n"); }

auto SceneManager::add_scene(Scene* scene) -> void {
  fmt::print("SceneManager.add_scene()\n");
  if (scene && !scene->_name.empty()) {
    _scene_map.emplace(scene->_name, scene);
    scene->_scene_manager = this;
  }
}

auto SceneManager::set_scene(std::string name) -> void {
  fmt::print("SceneManager.set_scene({})\n", name);
  if (auto it = _scene_map.find(name); it != _scene_map.end()) { _current_scene = it->second; }
}

auto SceneManager::delete_scene(std::string name) -> void {
  fmt::print("SceneManager.delete_scene({})\n", name);
  _scene_map.erase(name);
  if (_current_scene && _current_scene->_name == name) { _current_scene = nullptr; }
}