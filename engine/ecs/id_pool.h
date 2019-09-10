#ifndef ECS_ID_POOL_H
#define ECS_ID_POOL_H

#include "ecs/common.h"

namespace wage { namespace ecs {

  // Simple pool for creating unique IDs. Eachcreate call will return a unique a free ID with a unique vesion to support ID reuse.
  template <typename IdType, typename VersionType>
  class IdPool {
  public:

    typedef VersionedId<IdType, VersionType> Id;

    Id create() {
      IdType id;
      if (!freeList.empty()) {
        id = freeList.back();
        freeList.pop_back();
        storage[id].version = storage[id].version + 1;
      } else {
        id = nextId++;
      }
      storage[id].valid = true;
      return get(id);
    }

    bool valid(Id id) const {
      auto stored = storage[id.id()];
      return stored.valid && stored.version == id.version();
    }

    void destroy(Id id) {
      if (valid(id)) {
        storage[id.id()].valid = false;
        freeList.push_back(id.id());
      }
    }

    inline Id get(IdType id) const {
      return { id, storage[id].version };
    }

  private:

    class Item {
      friend class IdPool;
      VersionType version;
      bool valid;
    };

    IdType nextId;

    std::vector<IdType> freeList;

    Item storage[std::numeric_limits<IdType>::max()];
  };

} }
#endif //ECS_ID_POOL_H