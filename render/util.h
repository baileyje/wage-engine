#ifndef RENDER_UTIL_H
#define RENDER_UTIL_H

#include <glad/glad.h>
#include <iostream>

void checkGLError(const char* command);


#define FAIL_CHECK(cmd)    \
    do                     \
    {                      \
        cmd;               \
        checkGLError(#cmd); \
    } while(0)              \

#endif //RENDER_UTIL_H