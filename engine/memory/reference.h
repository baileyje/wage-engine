#ifndef MEMORY_REFERENCE_H
#define MEMORY_REFERENCE_H

#include <cstddef>
#include <assert.h>  

namespace wage {
  
  template <typename T>
  class Reference {
  
  public:

    class Source {      

    public:  
      
      virtual bool isValid(Reference ref) const = 0;

      virtual void free(Reference ref) const = 0;

      virtual T* get(Reference ref) = 0;

    };

    static const size_t OutOfBounds = static_cast<size_t>(-1);

    Reference() : Reference(nullptr, OutOfBounds, 0) {
    }

    Reference(Source* source, size_t index, size_t version) : source_(source), index_(index), version_(version) {
    }

    virtual ~Reference() {}

    inline bool isValid() {
      return source_->isValid(*this);
    }

    virtual void free() const {
      source_->free(*this);
    };

    virtual T* get() {
      return source_->get(*this);
    }

    operator bool() const { 
      return isValid(); 
    }

    T* operator->() {
      assert(isValid());
      return get();
    }

    const T* operator->() const {
      return const_cast<Reference*>(this)->operator->();
    }

    T& operator*() {
      assert(isValid());
      return *get();
    }

    const T& operator*() const {
      return const_cast<Reference<T>*>(this)->operator*();
    }

    bool operator==(const Reference& other) const {
      return source_ == other.source_ && index_ == other.index_;
    }
    
    bool operator!=(const Reference& other) const {
      return !operator==(other);
    }

    inline Source* source() {
      return source_;
    }

    inline size_t index() {
      return index_;
    }

    inline size_t version() {
      return version_;
    }

  private:
    
    Source* source_;
    
    size_t index_;

    size_t version_;
  
  };

}

#endif //MEMORY_REFERENCE_H