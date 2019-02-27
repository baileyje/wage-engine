#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "engine.h"
#include "physics-bullet/physics.h"
#include "render-gl/renderer.h"

using namespace wage;

class MoveIt : public System {

public:

  MoveIt(Entity entity) : System("MoveIt"), entity(entity) {
  }

  void update(SystemContext* context) {    
    auto body = entity.get<RigidBody>();
    if (Input::Instance->isPressed(Key::W)) {
      body->addImpulse(Vector(0, 0, 0.005));
    }
    if (Input::Instance->isPressed(Key::S)) {
      body->addImpulse(Vector(0, 0, -0.005));
    } 
    if (Input::Instance->isPressed(Key::A)) {
      body->addImpulse(Vector(0.005, 0, 0));
    }
    if (Input::Instance->isPressed(Key::D)) {
      body->addImpulse(Vector(-0.005, 0, 0));
    }
    if (Input::Instance->isPressed(Key::Space)) {
      body->addImpulse(Vector(0, 0.005, 0));
    }
    if (Input::Instance->isPressed(Key::LeftShift)) {
      body->addImpulse(Vector(0, -0.005, 0));
    }  
  }

private:

  Entity entity;

};

class CamLook : public System {

public: 

  CamLook(Entity entity) : System("CamLook"), lastPos(Vector2()), mouseSpeed(0.2), entity(entity) {
  }

  void start(SystemContext* context) {
    lastPos = Input::Instance->mousePosition();
  }

  void update(SystemContext* context) {
    auto mousePos = Input::Instance->mousePosition();    
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto transform = entity.get<Transform>();
    auto rotation = eulerAngles(transform->rotation());
    auto yaw = mouseSpeed * context->deltaTime() * dx;
    auto pitch = mouseSpeed * context->deltaTime() * dy;
    rotation += Vector(-pitch, yaw, 0);
    transform->rotation(Vector(btDegrees(rotation.x), btDegrees(rotation.y), btDegrees(rotation.z)));
    lastPos = mousePos;
  }

private:

    Vector2 lastPos;

    float mouseSpeed;

    Entity entity;
};


// class CamMove : public System {

// public: 

//   CamMove(Entity entity) : System("CamMove"), entity(entity) {
//   }
  
//   void update(SystemContext* context) {
//     Transform& transform = entity->transform();
//     if (Input::Instance->isPressed(GLFW_KEY_UP)) {
//         transform.localPosition() += Vector(0, 0, 1);
//     }
//     if (Input::Instance->isPressed(GLFW_KEY_DOWN)) {
//       transform.localPosition() +=  Vector(0, 0, -1);
//     } 
//     if (Input::Instance->isPressed(GLFW_KEY_LEFT)) {
//       transform.localPosition() += Vector(-1, 0, 0);
//     }
//     if (Input::Instance->isPressed(GLFW_KEY_RIGHT)) {
//       transform.localPosition() += Vector(1, 0, 0);
//     }
//   }

// private:

//   Entity entity;

// };

// ComponentCallback Raise = [](ComponentContext* context) {
//   if (Input::Instance->isPressed(GLFW_KEY_M)) {
//     auto body = context->entity().get<RigidBody>();
//     body->addImpulse(Vector(0, 0.0004, 0));
//   }
// };

class Follow : public System {

public: 

  Follow(Entity follower, Entity target) : System("Follow"), follower(follower.get<Transform>()), target(target.get<Transform>()) {
  }

  void start(SystemContext* context) {
    offset = follower->position() - target->position();
  }

  void fixedUpdate(SystemContext* context) {
    follower->position(target->position() + offset);
  }

private:

  Reference<Transform> follower;

  Reference<Transform> target;
  
  Vector offset;

};

class Chase : public System {

public: 

  Chase(Entity target) : System("Chase"), target(target), running(false) {    
  }
  void update(SystemContext* context) {
    if (!running && Input::Instance->isPressed(Key::G)) {
      running = true;
    }
  }

  void fixedUpdate(SystemContext* context) {
    if (!running) return;
    auto manager = context->get<EntityManager>();
    for (int i = 10; i < 500; ++i) {
      auto entity = manager->get(i);
      if (!entity.valid()) {
        return;
      }
      if (glm::distance(target.get<Transform>()->position(), entity.get<Transform>()->position()) < 2) {
        continue;
      }
      auto dir = target.get<Transform>()->position() - entity.get<Transform>()->position();
      auto movement = (glm::normalize(dir) * 0.0005);
      entity.get<RigidBody>()->addImpulse(movement);
    } 
  }

private:

  Entity target;

  bool running;

};

class FpsDisplay : public System {

public:

  FpsDisplay(Reference<Label> label) :  System("FPS"), label(label), lastTime(0), frames(0) {}

  void update(SystemContext* context) {
    double currentTime = context->time();
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

  Reference<Label> label;

  double lastTime;

  int frames;  

};

class PosDisplay : public System {

public:

  PosDisplay(Reference<Label> label, Entity target) :  System("POS"), label(label), target(target) {}

  void fixedUpdate(SystemContext* context) {
    auto pos = target.get<Transform>()->position();
    std::ostringstream os;
    os << "POS: " << int(pos.x) << ":" << int(pos.y) << ":" << int(pos.z);
    label->set(os.str());
  }

private:

  Reference<Label> label;

  Entity target;

};

void intHandler(int);

void addEntity(EntityManager* entityManager, SystemManager* systemManager, Vector position, float scale) {
  Entity entity = entityManager->create();    
  auto transform = entity.assign<Transform>();
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<RigidBody>(0.001);
  entity.assign<Mesh>(Mesh::Sphere);
  entity.assign<Collider>(ColliderType::sphere);
  entity.assign<Material>(make<Texture>("textures/default.png"));
}

void addRandomEntity(EntityManager* entityManager, SystemManager* systemManager) {
  float x = rand() % 400 - 200;
  float y = rand() % 400 - 200;
  float z = rand() % 400 - 200;
  float scale = (rand() % 100) / 30.0;
  addEntity(entityManager, systemManager, Vector(x, y, z), scale);
}

Entity addMover(EntityManager* entityManager, SystemManager* systemManager) {
  auto mover = entityManager->create();
  auto transform = mover.assign<Transform>();
  transform->position(Vector(0, 20, 0));
  transform->localScale(Vector(10, 10, 10));
  mover.assign<RigidBody>(0.01);
  mover.assign<Mesh>(Mesh::Sphere);
  mover.assign<Collider>(ColliderType::sphere);
  mover.assign<Material>(make<Texture>("textures/mover.png"));
  systemManager->create<MoveIt>(mover);
  
  return mover;
}

// void drawGrid(EntityManager* manager) {
//   addEntity(manager, Vector(), 1); // 0,0
//   addEntity(manager, Vector(0, 0, 2), 1); // Z Forward
//   for (int i = 1; i <= 10; ++i) {
//     int offset = i * 10;
//     addEntity(manager, Vector(-offset, 0, -offset), 1);
//     addEntity(manager, Vector(-offset, 0, 0), 1);
//     addEntity(manager, Vector(-offset, 0, offset), 1);
//     addEntity(manager, Vector(0, 0, offset), 1);
//     addEntity(manager, Vector(offset, 0, offset), 1);
//     addEntity(manager, Vector(offset, 0, 0), 1);
//     addEntity(manager, Vector(offset, 0, -offset), 1);
//     addEntity(manager, Vector(0, 0, -offset), 1);
//   }
// }

void setupServices(Core* core, std::string path) {
  Allocator::Assets();
  auto fileSystem = core->create<FileSystem, LocalFileSystem>(path);
  core->create<AssetManager, FsAssetManager>(fileSystem);
  core->create<Messaging>();
  core->create<Platform>();
  core->create<Input>();
  core->create<EntityManager>();
  core->create<SystemManager>();
}

void setupCoreSystems(SystemManager* systemManager) {
  systemManager->create<Renderer, GlRenderer>();
  systemManager->create<Physics, BulletPhysics>();
  systemManager->create<UI>();
}

void setupScene(EntityManager* entityManager, SystemManager* systemManager) {
  auto dirLight = entityManager->create();
  auto transform = dirLight.assign<Transform>();
  transform->rotation(Vector(-45, 0, 0));
  auto light = dirLight.assign<DirectionalLight>();
  light->diffuse(Color(0.7,0.7,0.7,1));
  light->ambient(Color(0.4,0.4,0.4,1));
  
  // Entity pointLight = entityManager->create();
  // pointLight->transform().setPosition(Vector(0, 2, 0));
  // pointLight->add(new PointLight());

  // Entity spotlight = entityManager->create();
  // spotlight->transform().setPosition(Vector(0, 2, -2));
  // spotlight->transform().setRotation(Vector(-45, 0, 0));  
  // Spotlight* spot = new Spotlight();
  // spot->setCutOff(40);
  // spot->setOuterCutOff(50);
  // spot->setQuadratic(0.000001);
  // spotlight->add(spot);

  // Entity ground = entityManager->create();
  // ground->transform().position(Vector(0, -2, 0));
  // ground->transform().localScale(Vector(2000, 2, 2000));
  // ground->transform().rotation(Vector(0, 0, 0));
  // ground
  //   .create<RigidBody>(0.0, RigidBodyType::kinematic)
  //   .add(&Mesh::Cube)
  //   .add(&Collider::Box)
  //   .add(MeshRenderer::Instance);

  auto mover = addMover(entityManager, systemManager);

  auto cameraEntity = entityManager->create();  
  auto camTransform = cameraEntity.assign<Transform>();
  camTransform->position(Vector(0, 15, -60));
  camTransform->rotation(Vector(00, 0.0, 0));
  cameraEntity.assign<PerspectiveCamera>();
  systemManager->create<CamLook>(cameraEntity);
  systemManager->create<Follow>(cameraEntity, mover);

  Font font("fonts/ARCADE.TTF", 60);
  auto fpsLabelEntity = entityManager->create();  
  fpsLabelEntity.assign<Transform>()->position(Vector(20, 0, 0));   
  auto fpsLabel = fpsLabelEntity.assign<Label>("FPS: ", font, Color(1, 1, 1, 0));  
  systemManager->create<FpsDisplay>(fpsLabel);

  auto posLabelEntity = entityManager->create();  
  posLabelEntity.assign<Transform>()->position(Vector(300, 0, 0));   
  auto posLabel = posLabelEntity.assign<Label>("POS: ", font, Color(1, 1, 1, 0));  
  systemManager->create<PosDisplay>(posLabel, mover);
  
  for (int i = 0; i < 1000; i++) {
    addRandomEntity(entityManager, systemManager);
  }
  systemManager->create<Chase>(mover);
}

int main(int argc, char* argv[]) {

  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  signal(SIGINT, intHandler);  

  setupServices(Core::Instance, path);
  auto systemManager = Core::Instance->get<SystemManager>();
  auto entityManager = Core::Instance->get<EntityManager>();
  setupCoreSystems(systemManager);
  
  Mesh::generatePrimitives();
  setupScene(entityManager, systemManager);  
  Core::Instance->init();
  Core::Instance->start();
  
  
  return 0;
}

void intHandler(int) { 
  Core::Instance->stop();
}