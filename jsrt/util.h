#ifndef JSRT_UTIL_H
#define JSRT_UTIL_H

#include <string>
#include "ChakraCore.h"

namespace wage {
  std::string jsErrorCodeToString(JsErrorCode jsErrorCode);

  char* stringFromValue(JsValueRef value);

  JsValueRef valueFromString(std::string);

  void printException(JsErrorCode errCode, JsValueRef exception);

  JsPropertyIdRef createProperty(std::string name);

  JsValueRef getProperty(JsValueRef object, std::string name);

  void handleJsError(JsErrorCode errCode, const char* cmd);

  #define FAIL_CHECK(cmd)                     \
      do                                      \
      {                                       \
          JsErrorCode errCode = cmd;          \
          if (errCode != JsNoError)           \
          {                                   \
              handleJsError(errCode, #cmd);   \
          }                                   \
      } while(0)                              \



  /**
   * JS String Wrapper
   */

  class JsStringWrapper {

  public:

    JsStringWrapper(JsValueRef ref);

    JsStringWrapper(std::string str);  

    ~JsStringWrapper();

    std::string getString();

    const char* getCString();

    JsValueRef getJsRef();

  private:

    std::string str;

  };


  class JsProperty {

  public:

    JsProperty(std::string name);

    ~JsProperty();

    JsValueRef get(JsValueRef object);

    void set(JsValueRef object, JsValueRef value);

    void set(JsValueRef object, double value);

    JsValueRef invoke(JsValueRef object, JsValueRef* args, size_t argCount);

  private:

    JsPropertyIdRef propertyId;

  };

}

#endif //JSRT_UTIL_H