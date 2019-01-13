#ifndef ENTITY_CONTEXT_H
#define ENTITY_CONTEXT_H

namespace wage {

  class EntityContext {

  public:

    EntityContext(const double time, const double deltaTime) : _time(time), _deltaTime(deltaTime)  { }

    virtual ~EntityContext() { }

    double time() const { return _time; }

    double deltaTime() const { return _deltaTime; }
    
  private:

    double _time;

    double _deltaTime;

  };

}

#endif //ENTITY_CONTEXT_H