#include "engine-ext/render-gl/program.h"

#include "engine/memory/allocator.h"

namespace wage {

  GlProgram* GlProgram::Default = make<GlProgram>("shaders/default.vs", "shaders/default.fs");
  
  GlProgram* GlProgram::Font = make<GlProgram>("shaders/font.vs", "shaders/font.fs");

}