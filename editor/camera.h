#pragma once

#include "engine.h"
#include "scene-serializer/serializer.h"

using namespace wage;


namespace wage::editor {

  class CameraController : public core::Service {
  public:
    CameraController() : Service("CameraController") {}

    void start() {
      lastPos = core::Core::Instance->get<input::Input>()->mousePosition();
      core::Core::Instance->onUpdate([&](const core::Frame& frame) {
        auto input = core::Core::Instance->get<input::Input>();
        auto camera = *scene::Scene::current().entities().with({PerspectiveCameraComponent, TransformComponent}).begin();
        auto transform = camera.get<math::Transform>(TransformComponent);
        if (!camera.valid()) return;
        auto mousePos = input->mousePosition();
        if (input->isPressed(input::MouseButton::Two)) {
          auto dx = lastPos.x - mousePos.x;
          auto dy = lastPos.y - mousePos.y;
          auto bearing = transform->localRotation;
          auto newDir = glm::rotate(bearing.glm(), dx / 150, math::Vector3::Up.glm());
          newDir = glm::rotate(newDir, -dy / 150, math::Vector3::Right.glm());
          transform->localRotation = {newDir.x, newDir.y, newDir.z, newDir.w};
        }
        float speed = 20 * frame.deltaTime();
        auto movement = math::Vector3::Zero;
        bool notSuper = !input->isPressed(input::Key::leftSuper) && !input->isPressed(input::Key::rightSuper);
        if (input->isPressed(input::Key::w) && notSuper) {
          movement += (transform->localRotation * math::Vector::Forward).normalized() * speed;
        }
        if (input->isPressed(input::Key::s) && notSuper) {
          movement += (transform->localRotation * math::Vector::Backward).normalized() * speed;
        }
        if (input->isPressed(input::Key::a) && notSuper) {
          movement += (transform->localRotation * math::Vector::Right).normalized() * speed;
        }
        if (input->isPressed(input::Key::d) && notSuper) {
          movement += (transform->localRotation * math::Vector::Left).normalized() * speed;
        }
        if (input->isPressed(input::Key::space) && notSuper) {
          movement += (transform->localRotation * math::Vector::Up).normalized() * speed;
        }
        if (input->isPressed(input::Key::leftShift) && notSuper) {
          movement += (transform->localRotation * math::Vector::Down).normalized() * speed;
        }
        transform->localPosition = transform->localPosition + movement;

        lastPos = mousePos;
      });
    }

  private:
    math::Vector2 lastPos;
  };

  void addCamera(scene::Scene& scene) {
    auto cameraEntity = scene.entities().create();
    auto camTransform = cameraEntity.assign<math::Transform>(TransformComponent);
    camTransform->localPosition = {0, 0, -10};
    camTransform->setEulerRotation({5, 0, 0});
    cameraEntity.assign<render::PerspectiveCamera>(PerspectiveCameraComponent);    
    cameraEntity.assign<serialize::NoSerialize>(NoSerializeComponent);
  }

}
