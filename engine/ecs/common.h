#pragma once

#include <limits>
#include <iostream>

#include "util/types.h"

namespace wage {
  namespace ecs {

    /**
     * Handle to a specific entity.
     */
    typedef uint16 EntityId;

    /**
     * The current version of an issued entity ID. Support distinguishing between the current and an out of date reference.
     */
    typedef uint16 EntityVersion;

    /**
     * The maximum entity id allowed.
     */
#define MAX_ENTITY_ID std::numeric_limits<EntityId>::max() / 2 - 1

    /**
     * Reserve the maximum value to represent an invalid handle.
     */
#define INVALID_ENTITY_ID std::numeric_limits<EntityId>::max()

    /**
     * Handle for a component type.
     */
    typedef uint16 ComponentType;

    /**
     * The maximum component id allowed.
     */
#define MAX_COMPONENT_TYPE std::numeric_limits<ComponentType>::max() / 2 - 1

    /**
     * Reserve the maximum value to represent an invalid handle.
     */
#define INVALID_COMPONENT_TYPE std::numeric_limits<ComponentType>::max()

    template <typename IdType, typename VersionType = uint16>
    class VersionedId {
    public:
      VersionedId(IdType id, VersionType version) : _id(id), _version(version) {
      }

      IdType id() const {
        return _id;
      }

      VersionType version() const {
        return _version;
      }

      bool operator==(const VersionedId& other) const {
        return _id == other._id && _version == other._version;
      }

      bool operator!=(const VersionedId& other) const {
        return !this->operator==(other);
      }

    private:
      IdType _id;

      VersionType _version;
    };

    template <typename IdType, typename VersionType>
    std::ostream& operator<<(std::ostream& os, const VersionedId<IdType, VersionType>& id) {
      os << id.id() << ':' << id.version();
      return os;
    }
  }
}
