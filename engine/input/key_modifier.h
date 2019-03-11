#ifndef INPUT_KEY_MOD_H
#define INPUT_KEY_MOD_H

namespace wage {

  enum class KeyModifier {
    shift = 0x01,
    control = 0x02,
    alt = 0x04,
    super = 0x08
  };
}

#endif //INPUT_KEY_MOD_H