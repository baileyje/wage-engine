#ifndef UTIL_MAP_H
#define UTIL_MAP_H

#include <map>

namespace wage {

  template <typename KT, typename VT>
  class Map {

  public:
    typedef std::pair<KT, VT> Entry;

    typedef typename std::map<KT, VT>::iterator iterator;

    struct KeyIterator {
      using map_t = std::map<KT, VT>;
      using realiterator_t = typename map_t::iterator;
      using value_t = typename std::add_const<typename map_t::key_type>::type;

      realiterator_t wrapped;

      KeyIterator(realiterator_t wrapped) : wrapped(wrapped) {}

      auto operator*() -> typename std::add_lvalue_reference<value_t>::type { return wrapped->first; }

      auto operator-> () -> typename std::add_pointer<value_t>::type { return &wrapped->first; }

      bool operator!=(const KeyIterator& o) const {
        return wrapped != o.wrapped;
      }

      KeyIterator& operator++() {
        ++wrapped;
        return *this;
      }
      KeyIterator operator++(int) {
        KeyIterator x(*this);
        ++wrapped;
        return x;
      }
    };

    struct ValueIterator {
      using map_t = std::map<KT, VT>;
      using realiterator_t = typename map_t::iterator;
      using value_t = typename std::add_const<typename map_t::mapped_type>::type;

      realiterator_t wrapped;

      ValueIterator(realiterator_t wrapped) : wrapped(wrapped) {}

      auto operator*() -> typename std::add_lvalue_reference<value_t>::type { return wrapped->second; }

      auto operator-> () -> typename std::add_pointer<value_t>::type { return &wrapped->second; }

      bool operator!=(const ValueIterator& o) const {
        return wrapped != o.wrapped;
      }

      ValueIterator& operator++() {
        ++wrapped;
        return *this;
      }
      ValueIterator operator++(int) {
        ValueIterator x(*this);
        ++wrapped;
        return x;
      }
    };

    VT& operator[](const KT& key) {
      return storage[key];
    }

    VT& operator[](KT&& key) {
      return storage[key];
    }

    KeyIterator keysBegin() {
      return KeyIterator(storage.begin());
    }

    KeyIterator keysEnd() {
      return KeyIterator(storage.end());
    }

    ValueIterator valuesBegin() {
      return ValueIterator(storage.begin());
    }

    ValueIterator valuesEnd() {
      return ValueIterator(storage.end());
    }

    iterator find(const KT& key) {
      return storage.find(key);
    }

    iterator begin() {
      return storage.begin();
    }

    iterator end() {
      return storage.end();
    }

  private:
    std::map<KT, VT> storage;
  };
}

#endif //UTIL_MAP_H
