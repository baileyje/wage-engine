#pragma once

#include <unistd.h>
#include <iostream>

#include "audio-al/audio.h"
#include "fs/local.h"

#include "engine.h"

using namespace wage;

class DumbMusicSystem : public ecs::System {

public:
  DumbMusicSystem() : System() {
  }

  void start(const ecs::SystemContext& context) {
  }

  void update(const ecs::SystemContext& context) {
    if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::m)) {
      char buffer[255];
      std::string path = std::string(getcwd(buffer, sizeof(buffer)));
      std::cout << path + "/resources/audio/sample.wav"
                << "\n";
      wage::fs::LocalFileInputStream wav(path + "/../game/resources/audio/sample.wav");
      core::Core::Instance->get<audio::Audio>()->play(&wav);
    }
  }
};