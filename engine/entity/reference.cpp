#include "engine/entity/reference.h"
#include "engine/entity/component/func_component.h"
#include "engine/entity/component/context.h"

namespace wage {
    
    EntityReference EntityReference::onUpdate(ComponentCallback func) {
      return create<FunctionComponent>(func);
    }

}

