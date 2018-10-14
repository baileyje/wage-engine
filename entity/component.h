#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H


class Component {

public:

  virtual ~Component() {};

  // TODO: Need some kind of enity context
  virtual void update() {};

  // TODO: Need some kind of enity context
  virtual void fixedUpdate() {};

};

#endif //ENTITY_COMPONENT_H