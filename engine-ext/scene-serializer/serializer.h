#pragma once

#include "engine.h"

#define NoSerializeComponent 9900


namespace wage::serialize {

  struct NoSerialize {
  };

  class SceneSerializer : public core::Service {
  public:
    SceneSerializer();

    void start();

    void load(fs::File::Path path, scene::Scene& scene);

    void store(fs::File::Path path, scene::Scene& scene);

  private:
    
    fs::FileSystem* fileSystem = nullptr;
  };
}