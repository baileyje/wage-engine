#pragma once

namespace wage {
  namespace input {

    enum class MouseButton {
      One,
      Two,
      Three,
      Four,
      Five,
      Six,
      Seven,
      Eight
    };

  }
}

template <>
class std::hash<wage::input::MouseButton> {
public:
  std::size_t operator()(wage::input::MouseButton const& button) const noexcept {
    return static_cast<std::size_t>(button) - static_cast<std::size_t>(wage::input::MouseButton::One);
  }
};
