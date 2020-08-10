#pragma once

#include "new_ecs/common.h"

namespace wage {
  namespace ecs {

    /**
     * Simple pool for creating unique IDs. Each create call will return a unique a free ID with a unique version to support ID recycling.
     * 
     * TODO: Thread safe for the win?
     */
    template <typename IdType, typename VersionType = uint16>
    class IdPool {
    public:
      typedef VersionedId<IdType, VersionType> Id;

      /**
       * Create a new ID instance from the free list if available or increment the base counter.
       */
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

      /**
       * Determine if the provided ID is the most recent valid version.
       */
      bool valid(Id id) const {
        auto stored = storage[id.id()];
        return stored.valid && stored.version == id.version();
      }

      /**
       * Destroy the ID provided and prepare it for reuse.
       */
      void destroy(Id id) {
        if (valid(id)) {
          storage[id.id()].valid = false;
          freeList.push_back(id.id());
        }
      }

      /**
       * Get and ID version reference for a provided ID.
       */
      inline Id get(IdType id) const {
        return {id, storage[id].version};
      }

    private:
      class Item {
        friend class IdPool;
        VersionType version;
        bool valid;
      };

      IdType nextId;

      // TODO: Uncontrolled memory usage.
      std::vector<IdType> freeList;

      Item storage[std::numeric_limits<IdType>::max()];
    };
  }
}