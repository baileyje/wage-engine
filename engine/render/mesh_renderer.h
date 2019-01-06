#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H


#include "engine/core/core.h"
#include "engine/memory/allocator.h"
#include "engine/render/renderer.h"
#include "engine/entity/component/dynamic.h"
#include "engine/entity/component/context.h"

namespace wage {


  class MeshRenderer : public DynamicComponent {
    
  public:

    static MeshRenderer* Instance;

    MeshRenderer() : DynamicComponent("MeshRenderer") {      
    }

    virtual ~MeshRenderer() {}

    void update(ComponentContext* context) {
      Core::Instance->get<Renderer>()->renderMesh(context->getTransform(), context->getEntity().get<Mesh>(), context->getEntity().get<Material>());
    }

  };

  MeshRenderer* MeshRenderer::Instance = make<MeshRenderer>();

}

#endif //MESH_RENDERER_H

