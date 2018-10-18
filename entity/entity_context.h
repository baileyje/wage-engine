#ifndef ENTITY_CONTEXT_H
#define ENTITY_CONTEXT_H

#include "entity/entity.h"

class EntityContext {

public:

  EntityContext(Entity* entity, const double time, const double deltaTime) : entity(entity), time(time), deltaTime(deltaTime)  { }

  virtual ~EntityContext() { }

  Entity* getEntity() const { return entity; }

  double getTime() const { return time; }

  double getDeltaTime() const { return deltaTime; }
  
private:

  Entity* entity;

  double time;

  double deltaTime;

};


#endif //ENTITY_CONTEXT_H