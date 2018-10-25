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
#include "entity/component/camera.h"
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
    if (Input::isPressed(GLFW_KEY_UP)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
    }
    if (Input::isPressed(GLFW_KEY_DOWN)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, -0.3);
    } 
    if (Input::isPressed(GLFW_KEY_LEFT)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0.3, 0, 0);
    }
    if (Input::isPressed(GLFW_KEY_RIGHT)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(-0.3, 0, 0);
    }
  }
};

class Tilt : public Component {

public:

  Tilt() : Component("Tilt") {    
  }

  void update(EntityContext* context) {
    if (Input::isPressed(GLFW_KEY_COMMA)) {
      *context->getEntity()->getTransform()->getRotation() += Vector(0, 0, -1);
    }
    if (Input::isPressed(GLFW_KEY_PERIOD)) {
      *context->getEntity()->getTransform()->getRotation() += Vector(0, 0, 1);
    }    
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
  // Jsrt jsrt;
  // core.add(&jsrt);
  core.add(new Physics());
  
  Engine engine;
  core.add(&engine);
  core.add(new Renderer());

  Entity* camera = core.getCamera();
  camera->add(new Camera());
  camera->add(new CamMove());
  core.add(camera);

  camera->getTransform()->setPosition(Vector(0, 20, -50));
  camera->getTransform()->setRotation(Vector(25, 0.0, 0));

  Entity dirLight;
  dirLight.getTransform()->setRotation(Vector(-90, 0, 0));
  DirectionalLight* temp = new DirectionalLight();
  temp->setDiffuse(Color(0.7,0.7,0.7,1));
  dirLight.add(temp);
  core.add(&dirLight);

  Material blueMat(Color(0, 0, 1, 1));

  Entity pointLight;
  pointLight.getTransform()->setPosition(Vector(0, 2, 0));
  pointLight.add(new PointLight());
  core.add(&pointLight);

  Entity spotlight;
  spotlight.getTransform()->setPosition(Vector(0, 2, -3));
  spotlight.getTransform()->setRotation(Vector(-90, 0, 0));  
  Spotlight spot;
  spot.setCutOff(40);
  spot.setOuterCutOff(50);
  spotlight.add(&spot);
  core.add(&spotlight);

  for (int i = 0; i < 5; i++) {
      Entity* entity = new Entity();
      RigidBody* body = new RigidBody(0.001);
      entity->getTransform()->setPosition(Vector(3, 3 * i, 0));       
      entity->add(body);
      entity->add(new DorkComp2());
      entity->add(&Mesh::Cube);
      entity->add(&blueMat);
      entity->add(&Collider::Box);
      core.add(entity);
  }

  Entity* mover = new Entity();
  mover->getTransform()->setPosition(Vector(0, 2, 0));
  mover->getTransform()->setScale(Vector(1, 1, 1));
  mover->getTransform()->setRotation(Vector(0, 0, 0));
  mover->add(new RigidBody(0.001));
  mover->add(new MoveIt());
  mover->add(&Mesh::Cube);
  Material redMat(Color(1, 0, 0, 1));
  mover->add(&redMat);
  mover->add(&Collider::Box);
  core.add(mover);

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
  core.add(ground);

  core.init();
  core.start();
  return 0;
}

void intHandler(int) { 
  if (coreRef != NULL) {
    coreRef->stop();
  }  
}