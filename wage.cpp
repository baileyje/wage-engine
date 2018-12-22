#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#include "math/vector.h"
#include "math/matrix.h"

#include "core/core.h"
#include "platform/platform.h"
#include "input/input.h"
#include "messaging/messaging.h"
#include "fs/local.h"
#include "assets/fs_manager.h"
#include "jsrt/jsrt.h"
#include "physics/physics.h"
#include "render-gl/renderer.h"
#include "render/mesh_renderer.h"
#include "engine/engine.h"
#include "entity/entity.h"
#include "entity/component.h"
#include "entity/component/dynamic.h"
#include "entity/component/physics/rigid_body.h"
#include "entity/component/physics/collider.h"
#include "entity/component/render/mesh.h"
#include "entity/component/render/material.h"
#include "entity/component/camera/perspective_camera.h"
#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"

#include "ui/label.h"

using namespace wage;

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
    body->addImpulse(Vector(0, 0.001, 0));
  }
  if (Input::isPressed(GLFW_KEY_LEFT_SHIFT)) {
    body->addImpulse(Vector(0, -0.0001, 0));
  }
};

ComponentCallback CamMove = [](ComponentContext* context) {
  Transform& transform = context->getEntity()->getTransform();
  if (Input::isPressed(GLFW_KEY_UP)) {
      transform.getLocalPosition() += Vector(0, 0, 1);
  }
  if (Input::isPressed(GLFW_KEY_DOWN)) {
    transform.getLocalPosition() +=  Vector(0, 0, -1);
  } 
  if (Input::isPressed(GLFW_KEY_LEFT)) {
    transform.getLocalPosition() += Vector(-1, 0, 0);
  }
  if (Input::isPressed(GLFW_KEY_RIGHT)) {
    transform.getLocalPosition() += Vector(1, 0, 0);
  }
};

ComponentCallback CamRotate = [](ComponentContext* context) {
  Transform transform = context->getEntity()->getTransform();
  Vector rotation = eulerAngles(transform.getRotation());
  if (Input::isPressed(GLFW_KEY_COMMA)) {
    rotation += Vector(0, 0.005, 0);
  }
  if (Input::isPressed(GLFW_KEY_PERIOD)) {
    rotation -= Vector(0, 0.005, 0);
  }
  context->getEntity()->getTransform().setRotation(Vector(btDegrees(rotation.x), btDegrees(rotation.y), btDegrees(rotation.z)));
};

ComponentCallback Raise = [](ComponentContext* context) {
  if (Input::isPressed(GLFW_KEY_M)) {
    Entity* entity = context->getEntity();
    RigidBody* body = entity->get<RigidBody>();
    body->addImpulse(Vector(0, 0.0004, 0));
  }
};

class Follow : public DynamicComponent {

public: 

  Follow(EntityReference target) : DynamicComponent("Follow"), target(target) {    
  }

  void start(ComponentContext* context) {
    offset = context->getTransform().getPosition() - target->getTransform().getPosition();
  }

  void update(ComponentContext* context) {
    context->getTransform().setPosition(target->getTransform().getPosition() + offset);
  }

private:

  EntityReference target;
  
  Vector offset;

};

class Chase : public DynamicComponent {

public: 

  Chase(EntityReference target) : DynamicComponent("Chase"), target(target) {    
  }

  void fixedUpdate(ComponentContext* context) {
    if (glm::distance(target->getTransform().getPosition(), context->getTransform().getPosition()) < 2) {
      return;
    } 
    Vector dir = target->getTransform().getPosition() - context->getTransform().getPosition();
    Vector movement = (glm::normalize(dir) * 0.0005);
    context->getEntity()->get<RigidBody>()->addImpulse(movement);
  }

private:

  EntityReference target;

};

Core* coreRef;

void intHandler(int);

void addEntity(EntityManager* manager, Vector position) {
  EntityReference entity = manager->create();    
    entity->getTransform().setPosition(position);
    entity->getTransform().setLocalScale(Vector(1, 1, 1));
    entity
      ->add(new RigidBody(0.001))
      ->add(Raise)
      ->add(&Mesh::Cube)
      ->add(&Collider::Box);
}

void addRandomEntity(EntityManager* manager) {
  float x = rand() % 200 - 100;
  float z = rand() % 200 - 100;
  addEntity(manager, Vector(x, 1, z));
}

EntityReference addMover(EntityManager* manager) {
  EntityReference mover = manager->create();
  mover->getTransform().setPosition(Vector(0, 0, 0));
  mover->getTransform().setLocalScale(Vector(5, 5, 5));
  mover
    ->add(new RigidBody(0.0005))
    ->add(&Mesh::Sphere)
    ->add(&Collider::Sphere)
    ->add(MeshRenderer::Instance)
    ->add(MoveIt)
    // ->add(CamMove)
    ->add(new Material(new Texture("textures/mover.png")));

  // EntityReference follower = manager->create();
  // follower->getTransform().setPosition(Vector(4, 10, 0));
  // // follower->getTransform().setLocalScale(Vector(0.10, 0.10, 0.10));
  // follower
  //   ->add(new RigidBody(0.001))
  //   ->add(&Mesh::Sphere)
  //   ->add(&Collider::Sphere)
  //   ->add(new Chase(mover));

  return mover;
}

void drawGrid(EntityManager* manager) {
  addEntity(manager, Vector()); // 0,0
  addEntity(manager, Vector(0, 0, 2)); // Z Forward
  for (int i = 1; i <= 10; ++i) {
    int offset = i * 10;
    addEntity(manager, Vector(-offset, 0, -offset));
    addEntity(manager, Vector(-offset, 0, 0));
    addEntity(manager, Vector(-offset, 0, offset));
    addEntity(manager, Vector(0, 0, offset));
    addEntity(manager, Vector(offset, 0, offset));
    addEntity(manager, Vector(offset, 0, 0));
    addEntity(manager, Vector(offset, 0, -offset));
    addEntity(manager, Vector(0, 0, -offset));
  }
}

void setupSystems(Core* core, std::string path) {
  FileSystem* localFs = new LocalFileSystem(path);
  core->add<FileSystem>(localFs);
  core->add<AssetManager>(new FsAssetManager(localFs));
  core->add(new Messaging());
  core->add(new Input());
  core->add(new Platform());
  core->add(new Physics());
  core->add(new EntityManager());
  core->add(new Jsrt());
  core->add(new Engine());
  core->add<Renderer>(new GlRenderer());
}

void setupScene(EntityManager* manager) {
  EntityReference dirLight = manager->create();
  dirLight->getTransform().setRotation(Vector(-45, 0, 0));
  DirectionalLight* temp = new DirectionalLight();
  temp->setDiffuse(Color(0.7,0.7,0.7,1));
  temp->setAmbient(Color(0.4,0.4,0.4,1));
  dirLight->add(temp);

  // EntityReference pointLight = manager->create();
  // pointLight->getTransform().setPosition(Vector(0, 2, 0));
  // pointLight->add(new PointLight());

  // EntityReference spotlight = manager->create();
  // spotlight->getTransform().setPosition(Vector(0, 2, -2));
  // spotlight->getTransform().setRotation(Vector(-45, 0, 0));  
  // Spotlight* spot = new Spotlight();
  // spot->setCutOff(40);
  // spot->setOuterCutOff(50);
  // spot->setQuadratic(0.000001);
  // spotlight->add(spot);

  for (int i = 0; i < 100; i++) {
     addRandomEntity(manager);
  }

  EntityReference ground = manager->create();
  ground->getTransform().setPosition(Vector(0, -2, 0));
  ground->getTransform().setLocalScale(Vector(200, 2, 200));
  ground->getTransform().setRotation(Vector(0, 0, 0));
  ground
    ->add(new RigidBody(0.0, kinematic))
    ->add(&Mesh::Cube)
    ->add(&Collider::Box)
    ->add(MeshRenderer::Instance);

  auto mover = addMover(manager);

  Camera* camera = new PerspectiveCamera();
  EntityReference cameraEntity = manager->create();  
  cameraEntity->getTransform().setPosition(Vector(0, 15, -30));
  cameraEntity->getTransform().setRotation(Vector(10, 0.0, 0));
  cameraEntity
    ->add(camera)
    ->add(CamRotate)
    // ->add(CamMove);
    ->add(new Follow(mover));
  Camera::main = camera;

  // drawGrid(manager);
  Font font("fonts/ARCADE.TTF", 60);
  Label* label = new Label("Hello", font, Color(1, 1, 1, 0));
  EntityReference labelEntity = manager->create();  
  labelEntity->getTransform().setPosition(Vector(100, 100, 0));  
  labelEntity->add(label);
}

// So far so dumb
int main(int argc, char* argv[]) {
  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));  
  signal(SIGINT, intHandler);  
  
  setupSystems(Core::Instance, path);
  EntityManager* manager = Core::Instance->get<EntityManager>();
  Mesh::generatePrimitives();
  setupScene(manager);
  Core::Instance->init();
  Core::Instance->start();
  return 0;
}

void intHandler(int) { 
  Core::Instance->stop();
}