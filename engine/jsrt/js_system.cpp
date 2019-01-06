#include "engine/jsrt/js_system.h"

#include "engine/core/system/context.h"

namespace wage {

  JsSystem::JsSystem(std::string name, JsValueRef objectRef) : System(name), object(objectRef) {  
  }

  JsSystem::~JsSystem() {
  }

  void JsSystem::init(SystemContext* context) {
    // TODO: Ensure system has `init`
    object.invoke("init");
  }

  void JsSystem::start(SystemContext* context) {
    // TODO: Ensure system has `start`
    object.invoke("start");
  }

  void JsSystem::fixedUpdate(SystemContext* context) {
    // TODO: Ensure system has `fixedUpdate`
    JsObjectWrapper jsContext;
    jsContext.set("time", context->getTime());
    jsContext.set("deltaTime", context->getDeltaTime());
    jsContext.set("timeStep", context->getTimeStep());
    JsValueRef args[] = { jsContext.get() };
    object.invoke("fixedUpdate", args, 1); 
  }

  void JsSystem::update(SystemContext* context) {
    // TODO: Ensure system has `update`
    JsObjectWrapper jsContext;
    jsContext.set("time", context->getTime());
    jsContext.set("deltaTime", context->getDeltaTime());
    jsContext.set("timeStep", context->getTimeStep());
    JsValueRef args[] = { jsContext.get() };
    object.invoke("update", args, 1); 
  }

  void JsSystem::stop(SystemContext* context) {
    // TODO: Ensure system has `stop`
    object.invoke("stop");
  }

  void JsSystem::deinit(SystemContext* context) {
    // TODO: Ensure system has `deinit`
    object.invoke("deinit");
  }

}