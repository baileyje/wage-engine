#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>

#include "core/core.h"
#include "platform/platform.h"
#include "input/input.h"
#include "messaging/messaging.h"
#include "fs/local.h"
#include "jsrt/jsrt.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "engine/engine.h"
#include "entity/entity.h"
#include "entity/component.h"
#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"
#include "entity/component/mesh.h"
#include "entity/component/material.h"
#include "entity/component/perspective_camera.h"
#include "entity/component/orthographic_camera.h"
#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"

ComponentCallback MoveIt = [](ComponentContext* context) {
  Entity* entity = context->getEntity();
  RigidBody* body = entity->get<RigidBody>();
  if (Input::isPressed(GLFW_KEY_W)) {
    body->addImpulse(Vector(0, 0, 0.0004));
  }
  if (Input::isPressed(GLFW_KEY_S)) {
    body->addImpulse(Vector(0, 0, -0.0004));
  } 
  if (Input::isPressed(GLFW_KEY_A)) {
    body->addImpulse(Vector(0.0004, 0, 0));
  }
  if (Input::isPressed(GLFW_KEY_D)) {
    body->addImpulse(Vector(-0.0004, 0, 0));
  }
  if (Input::isPressed(GLFW_KEY_SPACE)) {
    body->addImpulse(Vector(0, 0.0005, 0));
  }
  if (Input::isPressed(GLFW_KEY_LEFT_SHIFT)) {
    body->addImpulse(Vector(0, -0.0001, 0));
  }
};

ComponentCallback CamMove = [](ComponentContext* context) {
  Transform* transform = context->getEntity()->getTransform();
  Vector position = transform->getPosition();
  if (Input::isPressed(GLFW_KEY_UP)) {
      transform->setPosition(position + Vector(0, 0, 0.3));
  }
  if (Input::isPressed(GLFW_KEY_DOWN)) {
    transform->setPosition(position +Vector(0, 0, -0.3));
  } 
  if (Input::isPressed(GLFW_KEY_LEFT)) {
    transform->setPosition(position + Vector(0.3, 0, 0));
  }
  if (Input::isPressed(GLFW_KEY_RIGHT)) {
    transform->setPosition(position + Vector(-0.3, 0, 0));
  }
};

ComponentCallback CamTilt = [](ComponentContext* context) {
  Transform* transform = context->getEntity()->getTransform();
  Vector rotation = eulerAngles(transform->getRotation());
  if (Input::isPressed(GLFW_KEY_COMMA)) {
    rotation -= Vector(0.005, 0, 0);
  }
  if (Input::isPressed(GLFW_KEY_PERIOD)) {
    rotation += Vector(0.005, 0, 0);
  }
  context->getEntity()->getTransform()->setRotation(Vector(btDegrees(rotation.x), btDegrees(rotation.y), btDegrees(rotation.z)));
};

ComponentCallback Raise = [](ComponentContext* context) {
  if (Input::isPressed(GLFW_KEY_M)) {
    Entity* entity = context->getEntity();
    RigidBody* body = entity->get<RigidBody>();
    body->addImpulse(Vector(0, 0.0004, 0));
  }
};

class Follow : public Component {

public: 

  Follow(EntityReference target) : Component("Follow"), target(target) {    
  }

  void start(ComponentContext* context) {
    offset = context->getTransform()->getPosition() - target->getTransform()->getPosition();
  }

  void update(ComponentContext* context) {
    context->getTransform()->setPosition(target->getTransform()->getPosition() + offset);
  }

private:

  EntityReference target;
  
  Vector offset;

};

class Chase : public Component {

public: 

  Chase(EntityReference target) : Component("Chase"), target(target) {    
  }

  void fixedUpdate(ComponentContext* context) {
    if (glm::distance(target->getTransform()->getPosition(), context->getTransform()->getPosition()) < 2) {
      return;
    } 
    Vector dir = target->getTransform()->getPosition() - context->getTransform()->getPosition();
    Vector movement = (glm::normalize(dir) * 0.001);
    context->getEntity()->get<RigidBody>()->addImpulse(movement);
  }

private:

  EntityReference target;

};

Core* coreRef;

void intHandler(int);

void addEntity(EntityManager* manager, float offset) {
  EntityReference entity = manager->create();
  entity->getTransform()->setPosition(Vector(3 + offset * 3, 3, 0));       
  entity
    ->add(new RigidBody(0.001))
    ->add(Raise)
    ->add(&Mesh::Cube)
    ->add(&Collider::Box);
}

EntityReference addMover(EntityManager* manager) {
  EntityReference mover = manager->create();
  mover->getTransform()->setPosition(Vector(0, 0, 0));
  mover->getTransform()->setLocalScale(Vector(10, 10, 10));
  // mover.getTransform()->setRotation(Vector(0, 45, 0));
  mover
    ->add(new RigidBody(0.001))
    ->add(&Mesh::Sphere)
    ->add(&Collider::Sphere)
    ->add(MoveIt);

  EntityReference follower = manager->create();
  follower->getTransform()->setPosition(Vector(4, 10, 0));
  // follower->getTransform()->setLocalScale(Vector(0.10, 0.10, 0.10));
  follower
    ->add(new RigidBody(0.005))
    ->add(&Mesh::Sphere)
    ->add(&Collider::Sphere)
    ->add(new Chase(mover));

  return mover;
}

void setupSystems(Core& core, std::string path) {
  core.add<FileSystem>(new LocalFileSystem(path));
  core.add(new Messaging());
  core.add(new Input());
  core.add(new Platform());
  core.add(new Physics());
  core.add(new EntityManager());
  core.add(new Jsrt());
  core.add(new Engine());
  core.add(new Renderer());
}

void setupScene(EntityManager* manager) {
  EntityReference dirLight = manager->create();
  dirLight->getTransform()->setRotation(Vector(-90, 0, 0));
  DirectionalLight* temp = new DirectionalLight();
  temp->setDiffuse(Color(0.7,0.7,0.7,1));
  dirLight->add(temp);

  EntityReference pointLight = manager->create();
  pointLight->getTransform()->setPosition(Vector(0, 2, 0));
  pointLight->add(new PointLight());

  EntityReference spotlight = manager->create();
  spotlight->getTransform()->setPosition(Vector(0, 2, -3));
  spotlight->getTransform()->setRotation(Vector(-90, 0, 0));  
  Spotlight* spot = new Spotlight();
  spot->setCutOff(40);
  spot->setOuterCutOff(50);
  spotlight->add(spot);

  for (int i = 0; i < 10; i++) {
     addEntity(manager, i);
  }

  EntityReference ground = manager->create();
  ground->getTransform()->setPosition(Vector(0, -2, 0));
  ground->getTransform()->setLocalScale(Vector(100, 0.1, 100));
  ground->getTransform()->setRotation(Vector(0, 0, 0));
  ground
    ->add(new RigidBody(0.0, kinematic))
    ->add(&Mesh::Cube)
    ->add(&Collider::Box);

  addMover(manager);

  EntityReference camera = manager->create();  
  camera->getTransform()->setPosition(Vector(0, 15, -30));
  camera->getTransform()->setRotation(Vector(10, 0.0, 0));
  camera
    ->add(new PerspectiveCamera())
    ->add(CamTilt)
    ->add(CamMove);
    // ->add(new Follow(mover));
}

// So far so dumb
int main(int argc, char* argv[]) {
  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));  
  signal(SIGINT, intHandler);  
  Core core;
  coreRef = &core;
  
  setupSystems(core, path);
  EntityManager* manager = core.get<EntityManager>();
  Mesh::generatePrimitives();
  setupScene(manager);
  core.init();
  core.start();
  return 0;
}

void intHandler(int) { 
  if (coreRef != NULL) {
    coreRef->stop();
  }  
}