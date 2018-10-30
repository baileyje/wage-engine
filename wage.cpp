#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>

#include "core/core.h"
#include "platform/platform.h"
#include "input/input.h"
#include "jsrt/jsrt.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "engine/engine.h"
#include "entity/entity.h"
#include "entity/component.h"
#include "entity/entity_context.h"
#include "entity/component/rigid_body.h"
#include "entity/component/collider.h"
#include "entity/component/mesh.h"
#include "entity/component/material.h"
#include "entity/component/perspective_camera.h"
#include "entity/component/orthographic_camera.h"
#include "entity/component/directional_light.h"
#include "entity/component/point_light.h"
#include "entity/component/spotlight.h"

class MoveIt : public Component {

public:

  MoveIt() : Component("MoveIt") {    
  }

  void update(EntityContext* context) {
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
  }
};

class CamMove : public Component {

public:

  CamMove() : Component("CamMove") {    
  }

  void update(EntityContext* context) {
    Transform* transform = context->getEntity()->getTransform();
    Vector position = transform->getPosition();
    if (Input::isPressed(GLFW_KEY_UP)) {
       transform->setPosition(transform->getPosition() + Vector(0, 0, 0.3));
    }
    if (Input::isPressed(GLFW_KEY_DOWN)) {
      transform->setPosition(transform->getPosition() +Vector(0, 0, -0.3));
    } 
    if (Input::isPressed(GLFW_KEY_LEFT)) {
      transform->setPosition(transform->getPosition() + Vector(0.3, 0, 0));
    }
    if (Input::isPressed(GLFW_KEY_RIGHT)) {
      transform->setPosition(transform->getPosition() + Vector(-0.3, 0, 0));
    }
  }
};

class Tilt : public Component {

public:

  Tilt() : Component("Tilt") {    
  }

  void update(EntityContext* context) {
    Transform* transform = context->getEntity()->getTransform();
    Vector position = transform->getPosition();
    // if (Input::isPressed(GLFW_KEY_COMMA)) {
    //   *context->getEntity()->getTransform()->getRotation() += Vector(0, 0, -1);
    // }
    // if (Input::isPressed(GLFW_KEY_PERIOD)) {
    //   *context->getEntity()->getTransform()->getRotation() += Vector(0, 0, 1);
    // }
  }
};

class DorkComp2 : public Component {

public:

  DorkComp2() : Component("DorkComp2") {    
  }

  void update(EntityContext* context) {    
    if (Input::isPressed(GLFW_KEY_M)) {
      Entity* entity = context->getEntity();
      RigidBody* body = entity->get<RigidBody>();
      body->addImpulse(Vector(0, 0.0004, 0));
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
    }
  }

};

class GameController : public Component {

public:

  GameController(Core* core) : Component("GameController"), core(core) {    
  }

  void update(EntityContext* context) {    
    if (Input::isPressed(GLFW_KEY_R)) {
      // Entity* entity = context->getEntity();
      // RigidBody* body = entity->get<RigidBody>();
      // body->addImpulse(Vector(0, 0.0004, 0));
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
      core->stop();
      core->start();
    }
  }

private:

  Core* core;

};

Core* coreRef;

void intHandler(int);

// So far so dumb
int main(int argc, char* argv[]) {
  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  printf("Path: %s\n", path.c_str());
  signal(SIGINT, intHandler);  
  Core core(path);  
  coreRef = &core;
  core.add(new Platform());
  core.add(new Input());  
  core.add(new Physics());

  // Jsrt jsrt;
  // core.add(&jsrt);

  Engine engine;
  core.add(&engine);
  core.add(new Renderer());

  Scene* scene = core.getScene();

  Entity* camera = new Entity();
  camera
    ->add(new PerspectiveCamera())
    ->add(new CamMove());
  scene->add(camera);

  camera->getTransform()->setPosition(Vector(-20, 10, -30));
  camera->getTransform()->setRotation(Vector(10, 20.0, 0));

  Entity dirLight;
  dirLight.getTransform()->setRotation(Vector(-90, 0, 0));
  DirectionalLight* temp = new DirectionalLight();
  temp->setDiffuse(Color(0.7,0.7,0.7,1));
  dirLight.add(temp);
  scene->add(&dirLight);

  Material blueMat(Color(0, 0, 1, 1));

  Entity pointLight;
  pointLight.getTransform()->setPosition(Vector(0, 2, 0));
  pointLight.add(new PointLight());
  scene->add(&pointLight);

  Entity spotlight;
  spotlight.getTransform()->setPosition(Vector(0, 2, -3));
  spotlight.getTransform()->setRotation(Vector(-90, 0, 0));  
  Spotlight spot;
  spot.setCutOff(40);
  spot.setOuterCutOff(50);
  spotlight.add(&spot);
  scene->add(&spotlight);

  for (int i = 0; i < 0; i++) {
      Entity* entity = new Entity();
      entity->getTransform()->setPosition(Vector(3, 3 * i, 0));       
      entity
        ->add(new RigidBody(0.001))
        ->add(new DorkComp2())
        ->add(&Mesh::Cube)
        ->add(&Collider::Box);
      scene->add(entity);
  }

  Entity mover;
  mover.getTransform()->setPosition(Vector(0, 10, 0));
  // mover.getTransform()->setScale(Vector(1, 1, 1));
  // mover.getTransform()->setRotation(Vector(0, 45, 0));
  mover
    .add(new RigidBody(0.001))    
    ->add(&Mesh::Cube)
    ->add(new MoveIt())
    ->add(&Collider::Box);
  scene->add(&mover);

  Entity child;
  child.getTransform()->setPosition(Vector(0, 0, 1));
  child.getTransform()->setRotation(Vector(45, 0, 0));
  child.getTransform()->setScale(Vector(0.5, 0.5, 1));
  child
    .add(&Mesh::Cube)  
    ->add(&Collider::Box);
  mover.add(&child);

  Entity* ground = new Entity();
  ground->getTransform()->setPosition(Vector(0, -2, 0));
  ground->getTransform()->setScale(Vector(100, 0.1, 100));
  ground->getTransform()->setRotation(Vector(0, 0, 0));
  ground->add(new RigidBody(0.0, kinematic));
  ground->add(&Mesh::Cube);
  ground->add(&Collider::Box);
  ground->add(new Tilt());
  // Material whiteMat(Color(0.5, 0.5, 0.5, 1));
  // ground->add(&whiteMat);
  scene->add(ground);

  Entity* controller = new Entity();
  controller->add(new GameController(&core));
  scene->add(controller);

  core.init();
  core.start();
  return 0;
}

void intHandler(int) { 
  if (coreRef != NULL) {
    coreRef->stop();
  }  
}