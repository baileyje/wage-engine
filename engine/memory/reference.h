#ifndef MEMORY_REFERENCE_H
#define MEMORY_REFERENCE_H

#include <cstddef>
#include <assert.h>  

namespace wage {
  
  template <typename T, typename IndexType = uint32_t>
  class Reference {
  
  public:

    class Source {      

    public:  
      
      virtual bool valid(Reference ref) = 0;

      virtual void destroy(Reference ref) = 0;

      virtual T* get(Reference ref) = 0;

    };

    static const IndexType OutOfBounds = static_cast<IndexType>(-1);

    Reference() : Reference(nullptr, OutOfBounds, 0) {
    }

    Reference(Source* source, IndexType index, size_t version) : _source(source), _index(index), _version(version) {
    }

    virtual ~Reference() {}

    inline bool valid() {
      return _source && _source->valid(*this);
    }

    virtual void destroy() {
      _source->destroy(*this);
    };

    virtual T* get() {
      return _source->get(*this);
    }

    operator bool() { 
      return valid(); 
    }

    T* operator->() {
      assert(valid());
      return get();
    }

    const T* operator->() const {
      return const_cast<Reference*>(this)->operator->();
    }

    T& operator*() {
      assert(valid());
      return *get();
    }

    const T& operator*() const {
      return const_cast<Reference<T, IndexType>*>(this)->operator*();
    }

    bool operator==(const Reference& other) const {
      return _source == other._source && _index == other._index;
    }
    
    bool operator!=(const Reference& other) const {
      return !operator==(other);
    }

    inline const Source* source() const {
      return _source;
    }

    inline IndexType index() const {
      return _index;
    }

    inline size_t version() const {
      return _version;
    }

  private:
    
    Source* _source;
    
    IndexType _index;

    size_t _version;
  
  };

}

#endif //MEMORY_REFERENCE_H