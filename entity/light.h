#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

#include "entity/entity.h"

class Light : public Entity {

public:

  ~Light() {}

private:

   vec3 color;

  //  float attenuation;

  //  float ambientCoefficient;

  //  float coneAngle;

  //  vec3 coneDirection;

};

#endif //RENDERER_LIGHT_H