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
    RigidBody* body = (RigidBody*)entity->getComponent("RigidBody");
    // body->addForce(Vector(0, 0.0000000001, 0));
    if (Input::isPressed(GLFW_KEY_W)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
      body->addForce(Vector(0, 0, 0.0004));
    }
    if (Input::isPressed(GLFW_KEY_S)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, -0.3);
      body->addForce(Vector(0, 0, -0.0004));
    } 
    if (Input::isPressed(GLFW_KEY_A)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0.3, 0, 0);
      body->addForce(Vector(0.0004, 0, 0));
    }
    if (Input::isPressed(GLFW_KEY_D)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(-0.3, 0, 0);
      body->addForce(Vector(-0.0004, 0, 0));
    }
    if (Input::isPressed(GLFW_KEY_SPACE)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0.5, 0);
      body->addForce(Vector(0, 0.0005, 0));
    }
    if (Input::isPressed(GLFW_KEY_LEFT_SHIFT)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, -0.5, 0);
      body->addForce(Vector(0, -0.0001, 0));
    }
  }

  // void fixedUpdate(EntityContext* context) {
  //   // printf("Comp Fixed Called!\n");
  // }

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
    if (Input::isPressed(GLFW_KEY_COMMA)) {
      *context->getEntity()->getTransform()->getRotation() += Vector(0, 0, -0.1);
    }
    if (Input::isPressed(GLFW_KEY_PERIOD)) {
      *context->getEntity()->getTransform()->getRotation() += Vector(0, 0, 0.1);
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
      RigidBody* body = (RigidBody*)entity->getComponent("RigidBody");
      body->addForce(Vector(0, 0.0004, 0));
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

  camera->getTransform()->setPosition(Vector(0, 40, -30));
  camera->getTransform()->setRotation(Vector(45, 0.0, 0));

  Entity dirLight;
  dirLight.getTransform()->setRotation(Vector(-90, 0, 0));
  DirectionalLight* temp = new DirectionalLight();
  temp->setDiffuse(Color(1,1,1,1));
  dirLight.add(temp);
  core.add(&dirLight);

  Material blueMat(Color(0, 0, 1, 1));

  Entity pointLight;
  pointLight.getTransform()->setPosition(Vector(0, 2, 0));
  pointLight.add(new PointLight());
  pointLight.add(&Mesh::Cube);
  pointLight.add(&blueMat);
  core.add(&pointLight);

  Entity spotlight;
  spotlight.getTransform()->setPosition(Vector(0, 2, -3));
  spotlight.getTransform()->setRotation(Vector(-90, 0, 0));  
  Spotlight spot;
  spot.setCutOff(40);
  spot.setOuterCutOff(50);
  spotlight.add(&spot);
  // spotlight.add(new CamMove());
  spotlight.add(&Mesh::Cube);
  spotlight.add(&blueMat);
  core.add(&spotlight);

  for (int i = 0; i < 5; i++) {
      Entity* entity = new Entity();
      entity->getTransform()->setPosition(Vector(3, 3 * i, 0));   
      entity->getTransform()->setScale(Vector(1, 1, 1));
      entity->getTransform()->setRotation(Vector(-45, 0, 0));
      printf("Hmm-1: %f\n", entity->getTransform()->getRotation()->x);
      entity->add(new RigidBody(0.001));
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
  // ground->getTransform()->setRotation(Vector(90, 0, 0));
  ground->add(new RigidBody());
  ground->add(&Mesh::Cube);
  ground->add(&Collider::Box);
  Material whiteMat(Color(0.5, 0.5, 0.5, 1));
  ground->add(&whiteMat);
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