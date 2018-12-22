#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H


#include "core/core.h"
#include "render/renderer.h"
#include "entity/component/dynamic.h"

namespace wage {


  class MeshRenderer : public DynamicComponent {
    
  public:

    static MeshRenderer* Instance;

    MeshRenderer() : DynamicComponent("MeshRenderer") {      
    }

    virtual ~MeshRenderer() {}

    void update(ComponentContext* context) {
      Core::Instance->get<Renderer>()->renderMesh(context->getTransform(), context->getEntity()->get<Mesh>(), context->getEntity()->get<Material>());
    }

  };

  MeshRenderer* MeshRenderer::Instance = new MeshRenderer();

}

#endif //MESH_RENDERER_H

