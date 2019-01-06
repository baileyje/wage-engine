#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "engine/math/vector.h"
#include "engine/math/matrix.h"
#include "engine/core/core.h"
#include "engine/platform/platform.h"
#include "engine/input/input.h"
#include "engine/input/key_event.h"
#include "engine/input/mouse_button_event.h"
#include "engine/input/mouse_move_event.h"
#include "engine/input/mouse_scroll_event.h"
#include "engine/messaging/messaging.h"
#include "engine/fs/local.h"
#include "engine/assets/fs_manager.h"
#include "engine/jsrt/jsrt.h"
#include "engine/render/mesh_renderer.h"
#include "engine/engine.h"
#include "engine/entity/entity.h"
#include "engine/entity/component.h"
#include "engine/entity/component/dynamic.h"
#include "engine/physics/rigid_body.h"
#include "engine/physics/collider.h"
#include "engine/render/mesh.h"
#include "engine/render/material.h"
#include "engine/entity/component/camera/perspective_camera.h"
#include "engine/entity/component/lighting/directional_light.h"
#include "engine/entity/component/lighting/point_light.h"
#include "engine/entity/component/lighting/spotlight.h"
#include "engine/ui/label.h"
#include "engine/memory/allocator.h"

#include "engine-ext/physics-bullet/physics.h"
#include "engine-ext/render-gl/renderer.h"


using namespace wage;

ComponentCallback MoveIt = [](ComponentContext* context) {
  auto entity = context->getEntity();
  auto body = entity.get<RigidBody>();
  if (Input::Instance->isPressed(GLFW_KEY_W)) {
    body->addImpulse(Vector(0, 0, 0.0004));
  }
  if (Input::Instance->isPressed(GLFW_KEY_S)) {
    body->addImpulse(Vector(0, 0, -0.0004));
  } 
  if (Input::Instance->isPressed(GLFW_KEY_A)) {
    body->addImpulse(Vector(0.0004, 0, 0));
  }
  if (Input::Instance->isPressed(GLFW_KEY_D)) {
    body->addImpulse(Vector(-0.0004, 0, 0));
  }
  if (Input::Instance->isPressed(GLFW_KEY_SPACE)) {
    body->addImpulse(Vector(0, 0.001, 0));
  }
  if (Input::Instance->isPressed(GLFW_KEY_LEFT_SHIFT)) {
    body->addImpulse(Vector(0, -0.0001, 0));
  }  
};

class CamLook : public DynamicComponent {

public: 

  CamLook() : DynamicComponent("CamLook"), lastPos(Vector2()), mouseSpeed(0.2) {
  }

  void start(ComponentContext* context) {
    lastPos = Input::Instance->mousePosition();
  }

  void update(ComponentContext* context) {
    auto mousePos = Input::Instance->mousePosition();
    // printf("Mouse Pos: %f:%f\n", mousePos.x, mousePos.y);
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;

    Transform transform = context->getEntity()->getTransform();
    Vector rotation = eulerAngles(transform.getRotation());
    auto yaw = mouseSpeed * context->getDeltaTime() * dx;
    auto pitch = mouseSpeed * context->getDeltaTime() * dy;
    // printf("Yaw: %f\n", yaw);
    // printf("Pitch: %f\n", pitch);
    rotation += Vector(-pitch, yaw, 0);
    context->getEntity()->getTransform().setRotation(Vector(btDegrees(rotation.x), btDegrees(rotation.y), btDegrees(rotation.z)));
    lastPos = mousePos;
  }

private:

    Vector2 lastPos;

    float mouseSpeed;

};

ComponentCallback CamMove = [](ComponentContext* context) {
  Transform& transform = context->getEntity()->getTransform();
  if (Input::Instance->isPressed(GLFW_KEY_UP)) {
      transform.getLocalPosition() += Vector(0, 0, 1);
  }
  if (Input::Instance->isPressed(GLFW_KEY_DOWN)) {
    transform.getLocalPosition() +=  Vector(0, 0, -1);
  } 
  if (Input::Instance->isPressed(GLFW_KEY_LEFT)) {
    transform.getLocalPosition() += Vector(-1, 0, 0);
  }
  if (Input::Instance->isPressed(GLFW_KEY_RIGHT)) {
    transform.getLocalPosition() += Vector(1, 0, 0);
  }
};

ComponentCallback Raise = [](ComponentContext* context) {
  if (Input::Instance->isPressed(GLFW_KEY_M)) {
    auto body = context->getEntity().get<RigidBody>();
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
    context->getEntity().get<RigidBody>()->addImpulse(movement);
  }

private:

  EntityReference target;

};

class FpsDisplay : public DynamicComponent {

public:

  FpsDisplay(ComponentReference<Label> label) :  DynamicComponent("FPS"), label(label), lastTime(0), frames(0) {}

  void update(ComponentContext* context) {
    double currentTime = context->getTime();
    frames++;
    if ( currentTime - lastTime >= 1.0 ) {
      std::cout << "FPS: " << frames << std::endl;
      std::ostringstream os;
      os << "FPS: " << frames;
      label->set(os.str());
      frames = 0;
      lastTime = currentTime;
    }
  }

private:

  ComponentReference<Label> label;

  double lastTime;

  int frames;  

};

Core* coreRef;

void intHandler(int);

void addEntity(EntityManager* manager, Vector position) {
  EntityReference entity = manager->create();    
    entity->getTransform().setPosition(position);
    entity->getTransform().setLocalScale(Vector(1, 1, 1));
    entity
      .create<RigidBody>(0.001)
      .onUpdate(Raise)
      .add(&Mesh::Cube)
      .add(&Collider::Box)
      .add(MeshRenderer::Instance);
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
    .create<RigidBody>(0.0005)
    .add(&Mesh::Sphere)
    .add(&Collider::Sphere)
    .add(MeshRenderer::Instance)
    .onUpdate(MoveIt)
    // ->add(CamMove)
    .create<Material>(make<Texture>("textures/mover.png"));

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

class InputLogger : 
  public MessageListener<KeyEvent>, 
  public MessageListener<MouseButtonEvent>,
  public MessageListener<MouseMoveEvent>,
  public MessageListener<MouseScrollEvent> {

public:

  void on(KeyEvent& event) {
    printf("Key -> %d -> %d -> %d\n", 
      event.key(),
      event.type(),
      event.set({ KeyModifier::shift, KeyModifier::alt })
    );
  }

  void on(MouseButtonEvent& event) {
    printf("Mouse Button -> %d -> %d -> %D\n", 
      event.button(),
      event.type(),
      event.set(KeyModifier::shift)
    );
  }

  void on(MouseMoveEvent& event) {
    printf("Mouse Move -> %f:%f\n", 
      event.position().x,
      event.position().y
    );
  }

  void on(MouseScrollEvent& event) {
    printf("Mouse Scroll -> %f:%f\n", 
      event.position().x,
      event.position().y
    );
  }
};

void setupSystems(Core* core, std::string path) {
  Allocator::Assets();
  auto fileSystem = core->add<FileSystem, LocalFileSystem>(path);
  core->add<AssetManager, FsAssetManager>(fileSystem);
  auto messaging = core->add<Messaging>();
  auto inputLogger = new InputLogger();
  messaging->listen<KeyEvent>(inputLogger);
  messaging->listen<MouseButtonEvent>(inputLogger);
  messaging->listen<MouseMoveEvent>(inputLogger);
  messaging->listen<MouseScrollEvent>(inputLogger);
  core->add<Input>();
  core->add<Platform>();
  core->add<EntityManager>();
  core->add<Jsrt>();
  core->add<Engine>();
  core->add<Renderer, GlRenderer>();
  core->add<Physics, BulletPhysics>();
}

void setupScene(EntityManager* manager) {
  EntityReference dirLight = manager->create();
  dirLight->getTransform().setRotation(Vector(-45, 0, 0));
  DirectionalLight* temp = make<DirectionalLight>();
  temp->setDiffuse(Color(0.7,0.7,0.7,1));
  temp->setAmbient(Color(0.4,0.4,0.4,1));
  dirLight.add(temp);

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

  for (int i = 0; i < 500; i++) {
     addRandomEntity(manager);
  }

  EntityReference ground = manager->create();
  ground->getTransform().setPosition(Vector(0, -2, 0));
  ground->getTransform().setLocalScale(Vector(200, 2, 200));
  ground->getTransform().setRotation(Vector(0, 0, 0));
  ground
    .create<RigidBody>(0.0, RigidBodyType::kinematic)
    .add(&Mesh::Cube)
    .add(&Collider::Box)
    .add(MeshRenderer::Instance);

  // auto ref = manager->componentManager().create<RigidBody>(ground.getWrapped(), 0.0, kinematic);
  // printf("\n\n\Created - %s!!!!\n\n\n\n", ref->getName().c_str());
  
  // for (auto thing = manager->componentManager().begin<RigidBody>(); thing != manager->componentManager().end<RigidBody>(); ++thing) {
  //   printf("\n\n\nFOUND - %s!!!!\n\n\n\n", thing->getName().c_str());
  // }

  auto mover = addMover(manager);

  EntityReference cameraEntity = manager->create();  
  cameraEntity->getTransform().setPosition(Vector(0, 15, -30));
  cameraEntity->getTransform().setRotation(Vector(10, 0.0, 0));
  cameraEntity
    .create<Camera, PerspectiveCamera>()
    .create<Follow>(mover)
    .create<CamLook>();
  Camera::main = cameraEntity.get<Camera>();

  // drawGrid(manager);
  Font font("fonts/ARCADE.TTF", 60);
  EntityReference labelEntity = manager->create();  
  labelEntity->getTransform().setPosition(Vector(20, 0, 0));   
  labelEntity.create<Label>("FPS: ", font, Color(1, 1, 1, 0));  
  labelEntity.create<FpsDisplay>(labelEntity.get<Label>());
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