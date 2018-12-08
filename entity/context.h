#ifndef ENTITY_CONTEXT_H
#define ENTITY_CONTEXT_H

namespace wage {

  class EntityContext {

  public:

    EntityContext(const double time, const double deltaTime) : time(time), deltaTime(deltaTime)  { }

    virtual ~EntityContext() { }

    double getTime() const { return time; }

    double getDeltaTime() const { return deltaTime; }
    
  private:

    double time;

    double deltaTime;

  };

}

#endif //ENTITY_CONTEXT_H