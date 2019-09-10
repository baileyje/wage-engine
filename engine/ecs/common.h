#pragma once

#include <limits>

#include "util/types.h"

namespace wage { namespace ecs {

  typedef uint16 EntityId;

  #define MAX_ENTITY_ID std::numeric_limits<EntityId>::max() - 1

  #define INVALID_ENTITY_ID std::numeric_limits<EntityId>::max()

  typedef uint16 EntityVersion;

  typedef uint16 ComponentId;

  template <typename IdType, typename VersionType>
  class VersionedId {
  public:

    VersionedId(IdType id, VersionType version): _id(id), _version(version) {
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

  typedef VersionedId<EntityId, EntityVersion> VersionedEntityId;

} }
