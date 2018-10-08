#include "jsrt/js_system.h"

JsSystem::JsSystem(std::string name, JsValueRef objectRef) : System(name), object(objectRef) {  
}

JsSystem::~JsSystem() {
}

void JsSystem::init(Context* context) {
  // TODO: Ensure system has `init`
  object.invoke("init");
}

void JsSystem::start(Context* context) {
  // TODO: Ensure system has `start`
  object.invoke("start");
}

void JsSystem::fixedUpdate(Context* context) {
  // TODO: Ensure system has `fixedUpdate`
  JsObjectWrapper jsContext;
  jsContext.set("time", context->time);
  jsContext.set("deltaTime", context->deltaTime);
  jsContext.set("timeStep", context->timeStep);
  JsValueRef args[] = { jsContext.get() };
  object.invoke("fixedUpdate", args, 1); 
}

void JsSystem::update(Context* context) {
  // TODO: Ensure system has `update`
  JsObjectWrapper jsContext;
  jsContext.set("time", context->time);
  jsContext.set("deltaTime", context->deltaTime);
  jsContext.set("timeStep", context->timeStep);
  JsValueRef args[] = { jsContext.get() };
  object.invoke("update", args, 1); 
}

void JsSystem::stop(Context* context) {
  // TODO: Ensure system has `stop`
  object.invoke("stop");
}

void JsSystem::deinit(Context* context) {
  // TODO: Ensure system has `deinit`
  object.invoke("deinit");
}
