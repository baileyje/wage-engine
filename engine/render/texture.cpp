#include "engine/render/texture.h"
#include "engine/memory/allocator.h"

namespace wage {

  Texture* Texture::Default = make<Texture>("textures/default.png");

}