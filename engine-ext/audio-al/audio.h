#pragma once

#include <vector>

#include "core/core.h"
#include "asset/manager.h"
#include "audio/audio.h"
#include "audio-al/clip.h"
#include "audio-al/util.h"
#include "audio-al/data.h"
#include "audio-al/playing_audio.h"
#include "scene/scene.h"

#include "core/logger.h"

namespace wage {
  namespace audio {

    class AlAudio : public Audio {

    public:
      void start() {
        assetManager = core::Core::Instance->get<asset::Manager>();
        initAl();

        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          auto camTransform = cameraTransform();
          auto camPosition = camTransform.position();
          AL_FAIL_CHECK(alListener3f(AL_POSITION, camPosition.x, camPosition.y, camPosition.z));
          auto eulers = camTransform.rotation().eulerAngles();
          auto up = math::Vector3::Up;
          float direction[6];
          direction[0] = eulers.x;
          direction[1] = eulers.y;
          direction[2] = eulers.z;
          direction[3] = up.x;
          direction[4] = up.y;
          direction[5] = up.z;
          alListenerfv(AL_ORIENTATION, direction);
          // checkAlErrors();

          auto loadingIt = loading.begin();
          while (loadingIt != loading.end()) {
            auto loadingItem = *loadingIt;
            auto clip = loadingItem->clip;
            if (clip->loaded()) {
              loadingItem->start();
              this->playing.push_back(loadingItem);
              loadingIt = loading.erase(loadingIt);
            } else {
              ++loadingIt;
            }
          }

          auto playingIt = playing.begin();
          while (playingIt != playing.end()) {
            auto playingItem = *playingIt;
            ALint state = AL_PLAYING;
            alGetSourcei(playingItem->source, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED) {
              playingItem->cleanup();
              playingIt = playing.erase(playingIt);
            } else {
              playingItem->ensureBuffers();
              ++playingIt;
            }
          }
        });
      }

      void pause() {
        for (auto playingIt : playing) {
          if (playingIt->state() == PlayingAudio::State::playing) {
            playingIt->pause(true);
          }
        }
      }

      void unpause() {
        for (auto playingIt : playing) {
          if (playingIt->state() == PlayingAudio::State::pausedByService) {
            playingIt->play();
          }
        }
      }

      void reset() {
        for (auto playingIt : playing) {
          playingIt->stop();
          delete playingIt;
        }
        playing.clear();
        for (auto loadingIt : loading) {
          loadingIt->stop();
          delete loadingIt;
        }
        loading.clear();
      }

      ClipHandle* play(ClipSpec spec) {
        return play(spec, math::Vector3::Invalid);
      }

      virtual ClipHandle* play(ClipSpec spec, math::Vector3 position) {
        auto clip = assetManager->load<Clip>(spec, false);
        auto playing = new PlayingAudio(clip, position);
        loading.push_back(playing);
        return playing;
      }

    private:
      void initAl() {
        ALCdevice* device = alcOpenDevice(nullptr);
        if (!device) {
          core::Logger::error("Could not get an audio device.\n");
          return;
        }
        ALCcontext* context = alcCreateContext(device, NULL);
        if (context == NULL || alcMakeContextCurrent(context) == ALC_FALSE) {
          if (context != NULL)
            alcDestroyContext(context);
          alcCloseDevice(device);
          core::Logger::error("Could not set a context!\n");
          return;
        }
        const ALCchar* name = NULL;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
          name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || alcGetError(device) != AL_NO_ERROR)
          name = alcGetString(device, ALC_DEVICE_SPECIFIER);
        core::Logger::info("Opened Audio Device ", name);
      }

      math::Transform cameraTransform() {
        auto& entities = scene::Scene::current().entities();
        for (auto entity : entities.with({PerspectiveCameraComponent, TransformComponent})) {
          return *entity.get<math::Transform>(TransformComponent);
        }
        return math::Transform();
      }

      asset::Manager* assetManager;
      std::vector<PlayingAudio*> loading;
      std::vector<PlayingAudio*> playing;
    };
  } // namespace audio
} // namespace wage
