#include "entity/reference.h"
#include "entity/component/func_component.h"
#include "entity/component/context.h"

namespace wage {
    
    EntityReference EntityReference::onUpdate(ComponentCallback func) {
      return create<FunctionComponent>(func);
    }

}

