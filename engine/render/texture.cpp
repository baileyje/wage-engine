#include "render/texture.h"
#include "memory/allocator.h"

namespace wage {

  Texture* Texture::Default = make<Texture>("textures/default.png");

}