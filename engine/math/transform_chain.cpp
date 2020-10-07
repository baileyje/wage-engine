#include "math/transform_chain.h"

namespace wage::math {

  TransformChain::TransformChain(std::vector<Transform> chain) : chain(chain) {
  }

  Matrix TransformChain::worldProjection() const {
    // auto it = chain.begin();
    // Matrix current = it->localProjection();
    // it++;
    // while(it ) {
    //   current
    // }
    // return it->localProjection()
    
  }
}