#include "entity/entity.h"

#include "entity/component.h"
#include "entity/context.h"
#include "entity/component/context.h"
#include "entity/component/func_component.h"


#define LIFECYCLE_FUNC(Name) \
void Entity::Name(EntityContext* context) {  \
  EntityComponentContext componentContext(this, context); \
  for (auto component : components) { \
    component->Name(&componentContext); \
  } \
  for (auto entity : children) { \
    entity->Name(context); \
  } \
} \       


class EntityComponentContext : public ComponentContext {

public:

  EntityComponentContext(Entity* entity, EntityContext* context) : entity(entity), context(context) {    
  }

  inline Transform* getTransform() {
    return entity->getTransform();
  }

  inline double getTime() const {
    return context->getTime();
  }

  inline double getDeltaTime() const {
    return context->getDeltaTime();
  }

  inline ComponentMap* getComponents() {
    return entity->getComponents();
  }

  inline Entity* getEntity() {
    return entity;
  }
    

private:

  Entity* entity;

  EntityContext* context;

};

long Entity::CurrentId = 0;

Entity* Entity::create() {
  return create(Transform());
}

// TODO: DITCH JANKY ID GEN
// TODO: Use a pool for these
Entity* Entity::create(Transform transform) {
  return new Entity(Entity::CurrentId++, transform);
}

Entity::Entity(long id, Transform transform) : id(id), transform(transform) {    
}

Entity::~Entity() {}

LIFECYCLE_FUNC(start)
LIFECYCLE_FUNC(update)
LIFECYCLE_FUNC(fixedUpdate)
LIFECYCLE_FUNC(stop)

Entity* Entity::add(ComponentCallback func) {
  components.add(new FunctionComponent(func));
}
