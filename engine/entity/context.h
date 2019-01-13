#ifndef ENTITY_CONTEXT_H
#define ENTITY_CONTEXT_H

namespace wage {

  class EntityContext {

  public:

    EntityContext(const double time, const double deltaTime) : time_(time), deltaTime_(deltaTime)  { }

    virtual ~EntityContext() { }

    double time() const { return time_; }

    double deltaTime() const { return deltaTime_; }
    
  private:

    double time_;

    double deltaTime_;

  };

}

#endif //ENTITY_CONTEXT_H