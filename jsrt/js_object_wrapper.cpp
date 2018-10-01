#include "jsrt/js_object_wrapper.h"

#include "jsrt/util.h"

JsObjectWrapper::JsObjectWrapper() {
  FAIL_CHECK(JsCreateObject(&objectRef));
  FAIL_CHECK(JsAddRef(objectRef, nullptr));
}

JsObjectWrapper::JsObjectWrapper(JsValueRef ref) {
  objectRef = ref;
  FAIL_CHECK(JsAddRef(objectRef, nullptr));
}

JsObjectWrapper::~JsObjectWrapper() {
  FAIL_CHECK(JsRelease(objectRef, nullptr));
}

JsValueRef JsObjectWrapper::get() {
  return objectRef;
}

JsObjectProperty JsObjectWrapper::get(std::string propertyName) {
  return JsObjectProperty(this, propertyName);
}

void JsObjectWrapper::set(std::string propertyName, JsValueRef value) {
  JsProperty(propertyName).set(get(), value);
}

void JsObjectWrapper::set(std::string propertyName, JsObjectWrapper object) {
  set(propertyName, object.get());
}

void JsObjectWrapper::set(std::string propertyName, JsNativeFunction nativeFunction) {
  JsValueRef functionVar;  
  FAIL_CHECK(JsCreateNamedFunction(valueFromString(propertyName), nativeFunction, nullptr, &functionVar));  
  set(propertyName, functionVar);
}


// JS Property

JsObjectProperty::JsObjectProperty(JsObjectWrapper object, std::string name) : object(object), property(name) {  
}

JsObjectProperty::~JsObjectProperty() {    
}

JsValueRef JsObjectProperty::get() {
  property.get(object.get());
}

void JsObjectProperty::set(JsValueRef value) {
  property.set(object.get(), value);
}
