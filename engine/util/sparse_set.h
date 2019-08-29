#ifndef UTIL_SPARSE_SET_H
#define UTIL_SPARSE_SET_H


namespace wage {

  template <typename T, typename IT = size_t>
  class SparseSet {
  public:

    SparseSet() {
    }

    virtual bool contains(T item) const {
      auto dIndex = sparse[item.id()];
      return dense[dIndex] == item;
    }

    virtual void add(T item) {
      if (!contains(item)) {
        dense[next] = item;
        sparse[item.id()] = next;
        next++;
      }
    }

    virtual void remove(T item) {
      if (contains(item)) {
        dense[sparse[item.id()]] = dense[next -1];
        sparse[dense[next -1].id()] = sparse[item.id()];
        next--;
      }
    }

  private:
    IT next = 0;

    IT max = sizeof(IT);

    IT sparse[sizeof(IT)];

    T dense[sizeof(IT)];

  };



}

#endif //UTIL_SPARSE_SET_H
