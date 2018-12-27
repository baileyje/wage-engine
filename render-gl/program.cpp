#include "render-gl/program.h"

#include "memory/allocator.h"

namespace wage {

  GlProgram* GlProgram::Default = make<GlProgram>("shaders/default.vs", "shaders/default.fs");
  
  GlProgram* GlProgram::Font = make<GlProgram>("shaders/font.vs", "shaders/font.fs");

}