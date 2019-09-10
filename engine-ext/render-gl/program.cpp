#include "render-gl/program.h"

#include "memory/allocator.h"

namespace wage { namespace render {

  GlProgram* GlProgram::Default = memory::make<GlProgram>("shaders/default.vs", "shaders/default.fs");

  GlProgram* GlProgram::Font = memory::make<GlProgram>("shaders/font.vs", "shaders/font.fs");

  GlProgram* GlProgram::Sprite = memory::make<GlProgram>("shaders/sprite.vs", "shaders/sprite.fs");

} }