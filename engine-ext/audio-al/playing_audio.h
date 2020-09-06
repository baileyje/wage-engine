#pragma once

#include "audio-al/clip.h"
#include "audio-al/util.h"
#include "audio-al/data.h"

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

namespace wage {
  namespace audio {

    class PlayingAudio : public ClipHandle {
    public:
      enum class State {
        playing,
        paused,
        pausedByService
      };

      PlayingAudio(Clip* clip, math::Vector3 position) : clip(clip),
                                                         _position(position) {}

      virtual void play() {
        AL_FAIL_CHECK(alSourcePlay(source));
        _state = State::playing;
      }

      virtual void pause() {
        pause(false);
      }

      void pause(bool fromService = false) {
        AL_FAIL_CHECK(alSourcePause(source));
        _state = fromService ? State::pausedByService : State::paused;
      }

      void volume(float volume) {
        clip->volume(volume);
        AL_FAIL_CHECK(alSourcef(source, AL_GAIN, (ALfloat)volume));
      }

      void pitch(float pitch) {
        clip->pitch(pitch);
        AL_FAIL_CHECK(alSourcef(source, AL_PITCH, (ALfloat)pitch));
      }

      void loop(bool loop) {
        clip->loop(loop);
      }

      void position(math::Vector3 position) {
        _position = position;
        AL_FAIL_CHECK(alSource3f(source, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z));
      }

      void start() {
        auto audio = clip->data();
        AL_FAIL_CHECK(alGenBuffers(NUM_BUFFERS, &buffers[0]));
        for (size_t i = 0; i < NUM_BUFFERS; ++i) {
          char buffer[BUFFER_SIZE];
          auto read = audio.input()->read((memory::Byte*)&buffer[0], BUFFER_SIZE);
          AL_FAIL_CHECK(alBufferData(buffers[i], audio.format(), &buffer[0], read, audio.sampleRate()));
        }
        AL_FAIL_CHECK(alGenSources(1, &source));
        AL_FAIL_CHECK(alSourcef(source, AL_PITCH, (ALfloat)clip->pitch()));
        AL_FAIL_CHECK(alSourcef(source, AL_GAIN, (ALfloat)clip->volume()));
        if (_position != math::Vector3::Invalid) {
          AL_FAIL_CHECK(alSource3f(source, AL_POSITION, (ALfloat)_position.x, (ALfloat)_position.y, (ALfloat)_position.z));
        }
        AL_FAIL_CHECK(alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]));
        play();
      }

      void cleanup() {
        AL_FAIL_CHECK(alDeleteSources(1, &source));
        AL_FAIL_CHECK(alDeleteBuffers(NUM_BUFFERS, &buffers[0]));
        clip->data().input()->seek(clip->data().dataStartPosition());
      }

      void ensureBuffers() {
        ALint buffersProcessed = 0;
        AL_FAIL_CHECK(alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed));
        if (buffersProcessed <= 0)
          return;
        auto audioData = clip->data();
        auto input = audioData.input();
        while (buffersProcessed--) {
          ALuint buffer;
          AL_FAIL_CHECK(alSourceUnqueueBuffers(source, 1, &buffer));
          char data[BUFFER_SIZE];
          auto read = input->read((memory::Byte*)&data[0], BUFFER_SIZE);
          if (clip->loop() && read < BUFFER_SIZE) {
            clip->data().input()->seek(clip->data().dataStartPosition());
          }
          AL_FAIL_CHECK(alBufferData(buffer, audioData.format(), data, read, audioData.sampleRate()));
          AL_FAIL_CHECK(alSourceQueueBuffers(source, 1, &buffer));
        }
      }

      inline State state() {
        return _state;
      }

      Clip* clip;

      ALuint buffers[NUM_BUFFERS];

      ALuint source;

      State _state;

    private:
      math::Vector3 _position;
    };
  }
}
