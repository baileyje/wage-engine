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
    audio::ClipSpec spec("GameBack.wav", 1, currentPitch, true);
    // audio::ClipSpec spec("bounce.wav", true);
    // audio::ClipSpec spec("tone.wav", true);
    musicHandle = core::Core::Instance->get<audio::Audio>()->play(spec, {28, 0, 30});
  }

  void update(const ecs::SystemContext& context) {
    // if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::m)) {
    //   if (running) {
    //     return;
    //   }
    //   running = true;
    //   // audio::ClipSpec spec("2mb.wav", true);
    //   // audio::ClipSpec spec("sample.wav", true);
    //   audio::ClipSpec spec("GameBack.wav", 1, currentPitch, true);
    //   // audio::ClipSpec spec("bounce.wav", true);
    //   // audio::ClipSpec spec("tone.wav", true);
    //   musicHandle = core::Core::Instance->get<audio::Audio>()->play(spec, {28, 0, 30});
    // }
    if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::end)) {
      musicHandle->pause();
    }
    if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::right)) {
      musicHandle->play();
    }
    if (running && core::Core::Instance->get<input::Input>()->isPressed(input::Key::pageUp)) {
      currentPitch += 0.01;
      musicHandle->pitch(currentPitch);
    }
    if (running && core::Core::Instance->get<input::Input>()->isPressed(input::Key::pageDown)) {
      currentPitch -= 0.01;
      musicHandle->pitch(currentPitch);
    }
    if (running && core::Core::Instance->get<input::Input>()->isPressed(input::Key::up)) {
      currentVolume += 0.01;
      musicHandle->volume(currentVolume);
    }
    if (running && core::Core::Instance->get<input::Input>()->isPressed(input::Key::down)) {
      currentVolume -= 0.01;
      musicHandle->volume(currentVolume);
    }
  }

private:
  bool running;
  float currentPitch = 1.0;
  float currentVolume = 1.0;
  audio::ClipHandle* musicHandle;
};