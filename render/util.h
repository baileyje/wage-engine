#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>

#include "entity/vector.h"

void checkGLError(const char* command);


#define FAIL_CHECK(cmd)    \
    do                     \
    {                      \
        cmd;               \
        checkGLError(#cmd); \
    } while(0)              \


inline glm::vec3 vec3From(Vector* vector) {
    return glm::vec3(vector->x, vector->y, vector->z);
}

#endif //RENDER_UTIL_H
