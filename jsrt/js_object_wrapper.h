#ifndef JSRT_JSOBJECT_H
#define JSRT_JSOBJECT_H

#include <string>
#include "ChakraCore.h"
#include "jsrt/util.h"

class JsObjectProperty;

class JsObjectWrapper {

public:

  JsObjectWrapper();
  
  JsObjectWrapper(JsValueRef objectRef);

  ~JsObjectWrapper();

  void set(std::string propertyName, JsValueRef value);

  void set(std::string propertyName, double value);

  void set(std::string propertyName, JsObjectWrapper object);

  void set(std::string propertyName, JsNativeFunction nativeFunction);

  void set(std::string propertyName, JsNativeFunction nativeFunction, void* callbackState);

  JsValueRef invoke(std::string propertyName, JsValueRef* args, size_t argCount);

  JsValueRef invoke(std::string propertyName);

  JsValueRef get();

  JsObjectProperty get(std::string propertyName);

private:
  
  JsValueRef objectRef;

};


class JsObjectProperty {

public:

  JsObjectProperty(JsObjectWrapper object, std::string name);

  ~JsObjectProperty();

  JsValueRef get();

  void set(JsValueRef value);

private:

  JsObjectWrapper object;

  JsProperty property;

};

#endif //JSRT_JSOBJECT_H