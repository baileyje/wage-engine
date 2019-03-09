#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "engine.h"
#include "physics-bullet/physics.h"
#include "render-gl/renderer.h"
#include "platform-glfw/platform.h"
#include "input-glfw/input.h"

using namespace wage;


class Player {  
};

class Enemy {  
};

class PlayerMovement : public System {

public:

  PlayerMovement() : System("PlayerMovement"), lastPos(Vector2()), mouseSpeed(0.0005) {
  }

  void start(SystemContext* context) {
    lastPos = Core::Instance->get<Input>()->mousePosition();
  }

  void fixedUpdate(SystemContext* context) {    
    auto player = *Core::Instance->get<EntityManager>()->registry()->with<Player>().begin();
    auto body = player.get<RigidBody>();
    auto mousePos = Core::Instance->get<Input>()->mousePosition();    
    auto dx = lastPos.x - mousePos.x;
    auto dy = lastPos.y - mousePos.y;
    auto torque = Vector3::Up * mouseSpeed * dx + Vector3::Right * mouseSpeed * -dy;
    body->addTorqueImpulse(torque);
    auto bearing = player.get<Transform>()->rotation();
    float force = 0.1;
    auto impulse = Vector3::Zero;
    if (Core::Instance->get<Input>()->isPressed(Key::W)) {
      impulse += (bearing * Vector(0, 0, 1)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::S)) {      
      body->addImpulse((bearing * Vector(0, 0, -1)).normalized() * force);
    } 
    if (Core::Instance->get<Input>()->isPressed(Key::A)) {
      impulse += (bearing * Vector(1, 0, 0)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::D)) {
      impulse += (bearing * Vector(-1, 0, 0)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::Space)) {
      impulse += (bearing * Vector(0, 1, 0)).normalized() * force;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::LeftShift)) {    
      impulse += (bearing * Vector(0, -1, 0)).normalized() * force;
    }
    body->addImpulse(impulse);
    
    if (Core::Instance->get<Input>()->isPressed(Key::F)) {
      body->shouldStop(true);
    }
    lastPos = mousePos;
  }

private:

  Vector2 lastPos;

  float mouseSpeed;
  
};

// This is a fairly lame third person rigging
class ThirdPersonCamera : public System {

public: 

  ThirdPersonCamera(Entity camera, Entity player) : System("ThirdPersonCamera"), camera(camera), player(player) {
  }

  void start(SystemContext* context) {    
    followDistance = camera.get<Transform>()->position().distanceTo(player.get<Transform>()->position());
  }

  void fixedUpdate(SystemContext* context) {

    // printf("Pre TP Rotation,%f,%f,%f,%f\n", player.get<Transform>()->rotation().x, player.get<Transform>()->rotation().y, player.get<Transform>()->rotation().z, player.get<Transform>()->rotation().w);
    
  
    // auto xRotation = player.get<Transform>()->rotation().rotated(
    //     mouseSpeed * dx, 
    //     { 0, 1, 0 }
    // );
    // auto rotation = xRotation.rotated(
    //   mouseSpeed * -dy, 
    //   { 1, 0, 0 }
    // );
    //printf("Post TP Rotation,%f,%f,%f,%f\n", rotation.x, rotation.y, rotation.z, rotation.w);
  
    // 2. Set both camera and player rotation
    // player.get<Transform>()->rotation(rotation);
    // player.get<Bearing>()->rotation = rotation;
    // camera.get<Transform>()->rotation(rotation);
    
    // . Adjust camera position
    auto bearing = player.get<Transform>()->rotation();
    auto reverseBearing = (bearing * Vector(0, 0, -1)).normalized();
    // auto distance = Vector::distance(player.get<Transform>()->position(), camera.get<Transform>()->position());
    auto pos = player.get<Transform>()->position() + reverseBearing * followDistance;
    printf("Cam Pos: %f:%f:%f\n", pos.x, pos.y, pos.z);
    camera.get<Transform>()->position(pos);
    camera.get<Transform>()->rotation(bearing);        
  }

private:

    float followDistance;

    Entity camera;

    Entity player;
};


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

class EnemyMovement : public System {

public: 

  EnemyMovement() : System("EnemyMovement"), chasing(false), running(false) {
  }
  void fixedUpdate(SystemContext* context) {
    auto manager = Core::Instance->get<EntityManager>();
    if (Core::Instance->get<Input>()->isPressed(Key::C) && !chasing) {
      for (auto entity : manager->registry()->with<Enemy>()) {
        entity.get<RigidBody>()->shouldStop(true);        
      }
      chasing = true;
      running = false;
      return;
    }
    if (Core::Instance->get<Input>()->isPressed(Key::R) && !running) {
      for (auto entity : manager->registry()->with<Enemy>()) {
        entity.get<RigidBody>()->shouldStop(true);
      }
      running = true;
      chasing = false;  
    }
  // }

  // void fixedUpdate(SystemContext* context) {
  //   auto manager = Core::Instance->get<EntityManager>();
    auto target = *manager->registry()->with<Player>().begin();
    if (running || chasing) {    
      for (auto entity : manager->registry()->with<Enemy>()) {
        if (!entity.valid()) {
          return;
        }
        // if (chasing && Vector::distance(target.get<Transform>()->position(), entity.get<Transform>()->position()) < 20) {
        //   entity.get<RigidBody>()->shouldStop(true);
        //   continue;
        // }
        auto dir = target.get<Transform>()->position() - entity.get<Transform>()->position();
        auto impulse = dir.normalized() * 0.1;
        if (running) {
          impulse *= -1;
        }
        entity.get<RigidBody>()->addImpulse(impulse);
      } 
    }
  }

private:

  bool chasing;

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


class PrePhysicsService : public Service {
public:
  PrePhysicsService() : Service("Poop2") {}

  void start() {
    Core::Instance->onFixedUpdate([&](const Frame&) {
      auto registry = Core::Instance->get<EntityManager>()->registry();
      auto entTran = (*registry->with<Player>().begin()).get<Transform>();
      //printf("Pre Physics Rotation,%f,%f,%f,%f\n", entTran->rotation().x, entTran->rotation().y, entTran->rotation().z, entTran->rotation().w);
    });
  }
};

class PostPhysicsService : public Service {
public:
  PostPhysicsService() : Service("Poop3") {}

  void start() {
    Core::Instance->onFixedUpdate([&](const Frame&) {
      auto registry = Core::Instance->get<EntityManager>()->registry();
      auto entTran = (*registry->with<Player>().begin()).get<Transform>();
      //printf("Post Physics Rotation,%f,%f,%f,%f\n", entTran->rotation().x, entTran->rotation().y, entTran->rotation().z, entTran->rotation().w);
    });
  }
};

class TestRenderService : public Service {
public:
  TestRenderService() : Service("Poop") {}

  void start() {
    Core::Instance->onRender([&](const Frame&) {
      auto registry = Core::Instance->get<EntityManager>()->registry();
      auto entTran = (*registry->with<Player>().begin()).get<Transform>();
      // printf("Pre Render Rotation,%f,%f,%f,%f\n", entTran->rotation().x, entTran->rotation().y, entTran->rotation().z, entTran->rotation().w);
    });
  }

};

void intHandler(int);

void addEnemy(EntityManager* entityManager, SystemManager* systemManager, Vector position, float scale) {
  Entity entity = entityManager->create();    
  auto transform = entity.assign<Transform>();
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<RigidBody>(0.001);
  entity.assign<Mesh>(Mesh::Cube);
  entity.assign<Collider>(ColliderType::box);
  entity.assign<Material>(make<Texture>("textures/odd_space_2.png"));
  entity.assign<Enemy>();
}

void addRandomEnemy(EntityManager* entityManager, SystemManager* systemManager) {
  float x = rand() % 400 - 200;
  float y = rand() % 400 - 200;
  float z = rand() % 400 - 200;
  float scale = (rand() % 100) / 30.0;
  addEnemy(entityManager, systemManager, Vector(x, y, z) * 2, 1/*scale*/);
}

Entity addPlayer(EntityManager* entityManager, SystemManager* systemManager) {
  auto player = entityManager->create();
  auto transform = player.assign<Transform>();
  transform->position(Vector(0, 20, 0));
  transform->localScale(Vector(5, 5, 5));
  player.assign<RigidBody>(0.01);
  player.assign<Mesh>(Mesh::Sphere);
  player.assign<Collider>(ColliderType::sphere);
  player.assign<Material>(make<Texture>("textures/test_planet.png"));
  player.assign<Player>();
  return player;
}

void setupServices(Core* core, std::string path) {
  Allocator::Assets();
  auto fileSystem = core->create<FileSystem, LocalFileSystem>(path);
  core->create<AssetManager, FsAssetManager>(fileSystem);
  core->create<Messaging>();
  core->create<Platform, GlfwPlatform>();
  core->create<Input, GlfwInput>();
  core->create<EntityManager>();
  core->create<SystemManager>();
  core->create<PrePhysicsService>();  
  core->create<Physics, BulletPhysics>();
  core->create<PostPhysicsService>();
  core->create<TestRenderService>();
  core->create<Renderer, GlRenderer>();
}

void setupCoreSystems(SystemManager* systemManager) {  
  systemManager->create<UI>();
  systemManager->create<MeshRenderer>();  
}

void setupScene(EntityManager* entityManager, SystemManager* systemManager) {
  auto topLightEnt = entityManager->create();
  topLightEnt.assign<Transform>()->rotation(Vector(-90, 0, 0));
  auto topLight = topLightEnt.assign<DirectionalLight>();
  topLight->diffuse(Color(0.7,0.7,0.7,1));
  topLight->ambient(Color(0.4,0.4,0.4,1));

  auto bottomLightEnt = entityManager->create();
  bottomLightEnt.assign<Transform>()->rotation(Vector(90, 0, 0));
  auto bottomLight = bottomLightEnt.assign<DirectionalLight>();
  bottomLight->diffuse(Color(0.7,0.7,0.9,1));
  bottomLight->ambient(Color(0.4,0.4,0.4,1));
  
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

  auto player = addPlayer(entityManager, systemManager);

  auto cameraEntity = entityManager->create();  
  auto camTransform = cameraEntity.assign<Transform>();
  camTransform->position(Vector(0, 30, -30));
  camTransform->rotation(Vector(00, 0.0, 0));
  cameraEntity.assign<PerspectiveCamera>();
  systemManager->create<ThirdPersonCamera>(cameraEntity, player);


  Font font("fonts/ARCADE.TTF", 60);
  auto fpsLabelEntity = entityManager->create();  
  fpsLabelEntity.assign<Transform>()->position(Vector(20, 0, 0));   
  auto fpsLabel = fpsLabelEntity.assign<Label>("FPS: ", font, Color(1, 1, 1, 0));  
  systemManager->create<FpsDisplay>(fpsLabel);

  auto posLabelEntity = entityManager->create();  
  posLabelEntity.assign<Transform>()->position(Vector(300, 0, 0));   
  auto posLabel = posLabelEntity.assign<Label>("POS: ", font, Color(1, 1, 1, 0));  
  systemManager->create<PosDisplay>(posLabel, player);
  
    for (int i = 0; i < 200; i++) {
      addRandomEnemy(entityManager, systemManager);
    }

  systemManager->create<EnemyMovement>();
  systemManager->create<PlayerMovement>();
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