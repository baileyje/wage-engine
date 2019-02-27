
#include "ecs/system.h"
#include "entity/manager.h"
#include "render/renderer.h"
#include "label.h"

namespace wage {
  
  class UI : public System  {

  public:

    UI() : System ("UI") {}

    void update(SystemContext* context) {
      EntityManager* manager = Core::Instance->get<EntityManager>();
      for (auto entity : manager->registry()->with<Label>()) {
        auto label = entity.get<Label>();
        Core::Instance->get<SystemManager>()->get<Renderer>()->renderText(entity.get<Transform>()->position(), label->text(), label->font(), label->color());
      }
    }

  };
}

  