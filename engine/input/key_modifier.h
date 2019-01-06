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


// GLFW_MOD_SHIFT   0x0001
//  	If this bit is set one or more Shift keys were held down. More...
 
// #define 	GLFW_MOD_CONTROL   0x0002
//  	If this bit is set one or more Control keys were held down. More...
 
// #define 	GLFW_MOD_ALT   0x0004
//  	If this bit is set one or more Alt keys were held down. More...
 
// #define 	GLFW_MOD_SUPER   0x0008


#endif //INPUT_KEY_MOD_H