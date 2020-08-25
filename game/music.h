#pragma once

#include <unistd.h>
#include <iostream>

#include "audio/clip_spec.h"
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
      if (running) return;
      running = true;
      audio::ClipSpec spec("2mb.wav");
      core::Core::Instance->get<audio::Audio>()->play(spec);
    }
  }

private:
  bool running;
};