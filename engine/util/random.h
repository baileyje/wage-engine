#pragma once

#include <stdlib.h>

namespace wage {

  static int randomBetween(int min, int max) {
    return rand() % (max - min + 1) + min;
  }

}