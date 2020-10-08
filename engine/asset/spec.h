#pragma once

#include <string>

namespace wage {
  namespace asset {

    /**
     * A specificaton class to help identify assets in the system. These are not actual assets,
     * act as a specification for what what asset to load.
     */
    class AssetSpec {

    public:
      typedef std::string Type;

      typedef std::string Key;

      /**
       * Create a asset specification with a type and key.
       */
      AssetSpec(Type type, Key key) :_key(key),  _type(type) {
      }

      virtual ~AssetSpec() {}

      /**
       * Get the asset type.
       */
      virtual Type type() const {
        return _type;
      }

      /**
       * Get the asset key.
       */
      virtual Key key() const {
        return _key;
      }

      virtual void key(std::string newKey) {
        _key = newKey;
      }

    protected:
      Key _key;

    private:
      Type _type;

    };
  }
}
