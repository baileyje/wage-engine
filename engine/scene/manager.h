#pragma once

#include "core/service.h"

#include "scene/scene.h"
#include "scene/builder.h"

namespace wage {
  namespace scene {

    class Manager : public core::Service {
    public:
      Manager() : Service("SceneManager") {}

      void builder(SceneBuilder builder) {
        _builder = builder;
      }

      void start() override {
        _builder(_currentScene);
        _currentScene.entities().start();
        _currentScene.systems().start();
        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          _currentScene.entities().update();
          _currentScene.systems().update();
        });
        core::Core::Instance->onFixedUpdate([&](const core::Frame& frame) {
          _currentScene.systems().fixedUpdate();
        });
        core::Core::Instance->onPostUpdate([&](const core::Frame& frame) {
          _currentScene.entities().postUpdate();
        });
      }

      virtual void reset() override {
        _currentScene = Scene{};
        _builder(_currentScene);
        _currentScene.entities().start();
        _currentScene.systems().start();
      }

      void stop() override {
        _currentScene.entities().stop();
        _currentScene.systems().stop();
        // TODO: Reload the new scene
      }

      void update() {
      }

      inline Scene& currentScene() {
        return _currentScene;
      }

    private:
      Scene _currentScene;

      SceneBuilder _builder;
    };

  }
}