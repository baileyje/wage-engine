#ifndef JSRT_UTIL_H
#define JSRT_UTIL_H

#include <string>
#include "ChakraCore.h"

std::string jsErrorCodeToString(JsErrorCode jsErrorCode);

bool installObjectsOnObject(JsValueRef object, const char* name, JsNativeFunction nativeFunction);

bool createNamedFunction(const char* nameString, JsNativeFunction callback, JsValueRef* functionVar);

char* stringFromValue(JsValueRef value);

JsValueRef valueFromString(const char* string);

void printException(JsValueRef exception);

JsValueRef getProperty(JsValueRef object, std::string name);

#define FAIL_CHECK(cmd)                     \
    do                                      \
    {                                       \
        JsErrorCode errCode = cmd;          \
        if (errCode != JsNoError)           \
        {                                   \
            printf("Error %s at '%s'\n",    \
              jsErrorCodeToString(errCode).c_str(), #cmd);             \
        }                                   \
    } while(0)


class JSString {

public:

  JSString(JsValueRef ref);

  JSString(char* cSrt);

  JSString(std::string str);  

  ~JSString();

  std::string getString();

  const char* getCString();

  JsValueRef getJSRef();

private:

  JsValueRef valueRef;

  std::string str;

};


#endif //JSRT_UTIL_H