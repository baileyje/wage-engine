#include "entity/entity.h"

#include "memory/allocator.h"

#include "entity/component.h"
#include "entity/context.h"
#include "entity/component/context.h"
#include "entity/component/func_component.h"

namespace wage {

  #define LIFECYCLE_FUNC(Name) \
  void Entity::Name(EntityContext* context) {  \
    EntityComponentContext componentContext(this, context); \
    for (auto component : dynamicComponents) { \
      component->Name(&componentContext); \
    } \
  } \


  class EntityComponentContext : public ComponentContext {

  public:

    EntityComponentContext(Entity* entity, EntityContext* context) : entity(entity), context(context) {    
    }

    inline Transform& getTransform() {
      return entity->getTransform();
    }

    inline double getTime() const {
      return context->getTime();
    }

    inline double getDeltaTime() const {
      return context->getDeltaTime();
    }

    inline ComponentMap<Component>* getComponents() {
      return entity->getComponents();
    }

    inline Entity* getEntity() {
      return entity;
    }
      

  private:

    Entity* entity;

    EntityContext* context;

  };

  EntityId Entity::CurrentId = 1; // 0 = invalid Id

  Entity::~Entity() {}

  LIFECYCLE_FUNC(start)
  LIFECYCLE_FUNC(update)
  LIFECYCLE_FUNC(fixedUpdate)
  LIFECYCLE_FUNC(stop)

  Entity* Entity::add(ComponentCallback func) {
    add<FunctionComponent>(make<FunctionComponent>(func));    
    return this;
  }

}