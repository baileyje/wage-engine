#ifndef ENTITY_COMPONENT_MANAGER_H
#define ENTITY_COMPONENT_MANAGER_H

#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "memory/object_pool.h"
#include "entity/entity.h"
#include "entity/component.h"
#include "entity/component/dynamic.h"
#include "entity/component/reference.h"

namespace wage {
  
  class ComponentManager {

  private:

    class PoolEntry {

      template <typename T>
      ObjectPool<T>* getPool() {
        return static_cast<T*>(pool);
      }

      private:

        void* pool;
        
    };

    typedef std::vector<ComponentId>::iterator ComponentIdIterator;

  public:

    class ComponentEntry {
      
    public:

      ComponentEntry() : _id(0), _entity(Reference<Entity>()), _component(nullptr), _typeId(nullptr) {}

      ComponentEntry(ComponentId id, Reference<Entity> entity, void* component, const std::type_info* typeId) : _id(id), _entity(entity), _component(component), _typeId(typeId) {
      }

      inline ComponentId id() {
        return _id;
      }

      inline Reference<Entity> entity() {
        return _entity;
      }

      template <typename T>
      inline T* component() {
        return static_cast<T*>(_component);
      }

      inline const std::type_info* typeId() {
        return _typeId;
      }

      inline bool valid() {
        return _component != nullptr;
      }

    private:

      ComponentId _id;

      Reference<Entity> _entity;
      
      void* _component;

      const std::type_info* _typeId;

    };
  
    template <typename T>
    class Iterator {
      
      typedef ComponentReference<T> reference;

      friend class ComponentManager;

    public:
      
      Iterator(ComponentManager* manager, ComponentIdIterator iterator) : manager(manager), iterator(iterator) {        
      }

      ~Iterator() {}

      bool operator==(const Iterator& other) const {
        return manager == other.manager && iterator == other.iterator;
      }

      bool operator!=(const Iterator& other) const {
        return !operator==(other);
      }

      Iterator& operator++() {
        iterator++;
        return (*this);
      }

      Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
      }

      Iterator& operator--() {
        iterator--;
        return (*this);
      }

      Iterator operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
      }

      reference operator*() { 
        return manager->get<T>(*iterator);
      }

      reference operator->() { 
        return manager->get<T>(*iterator);
      }      

      // ComponentReference<T> asReference() const {
      //   return currentRef;
      // }

    private:
      
      ComponentManager* manager;
      
      ComponentIdIterator iterator;
    
    };

    ComponentManager() {}

    ~ComponentManager() {}

    template <typename T>
    inline void setupComponent(Reference<Entity> entity, ComponentEntry entry) {
      auto component = entry.component<T>();
      component->transform(&entity->transform());
      if (component->isDynamic()) {
        dynamicComponents.push_back(entry.id());
      }
    }

    template <typename T, typename I, typename... Args>
    inline ComponentReference<T> create(Reference<Entity> entity,  Args... args) {
      auto component = make<I>(args...);
      return add<T>(entity, component);
    }

    template <typename T, typename... Args>
    inline ComponentReference<T> create(Reference<Entity> entity,  Args... args) {
      auto component = make<T>(args...);
      return add(entity, component);
    }

    template <typename T>
    inline ComponentReference<T> add(Reference<Entity> entity, T* component) {
      auto entry = ComponentEntry(nextId(), entity, (void*)component, &typeid(component));
      // printf("Added: %d -> %d -> %s\n", entity->id(), reference.id(), typeid(component).name());
      setupComponent<T>(entity, entry);
      ComponentName name = component->name();
      nameMap[typeid(component)] = name;
      byId[entry.id()] = entry;
      byName[name].push_back(entry.id());
      map[entity->id()].push_back(entry.id());      
      return ComponentReference<T>(entry.id(), entity, component);
    }

    inline ComponentEntry* entryFor(ComponentId id) {
      auto itr = byId.find(id);
      if (itr != byId.end()) {
        return &itr->second;
      }
      return nullptr;
    }
    
    template <typename T>
    inline ComponentReference<T> get(ComponentId id) {
      // printf("Getting: %d -> %s\n", id, typeid(T*).name());
      auto itr = byId.find(id);
      // printf("Found: %d -> %s\n", id, itr->second.typeId()->name());
      if (itr != byId.end()) {
        auto entry = itr->second;
        return ComponentReference<T>(entry.id(), entry.entity(), entry.component<T>());
        // return itr->second.component<T>();
      }
      return ComponentReference<T>();
    }

    template <typename T>
    inline ComponentReference<T> get(Reference<Entity> entity) {
      // std::cout << map[entity->id()].size() << std::endl;
      // printf("EntId: %d\n", entity->id());
      auto itr = map.find(entity->id());
      if (itr != map.end()) {
        // TODO: Ensure map has entry
        for (auto id : map[entity->id()]) {
          auto entry = entryFor(id);
          // auto value = get<T>(id);
          // printf("CompId: %d -> %s\n", id, typeid(value).name());
          // printf("IsNull: %d\n", value == nullptr);
          if (entry && entry->typeId() == &typeid(T*)) {
            return ComponentReference<T>(entry->id(), entry->entity(), entry->component<T>());
          }
        }   
      }   
      // printf("Really HMMMM!!!\n");
      return ComponentReference<T>();
    }

    template <typename T>
    Iterator<T> begin() {
      auto itr = nameMap.find(typeid(T*));
      if (itr != nameMap.end()) {
        return Iterator<T>(this, byName[itr->second].begin());
      }
      return Iterator<T>(this, byName["_COMPONENT_NOT_FOUND_"].begin());
    }

    template <typename T>
    Iterator<T> end() {
      auto itr = nameMap.find(typeid(T*));
      if (itr != nameMap.end()) {
        return Iterator<T>(this, byName[itr->second].end());
      }
      return Iterator<T>(this, byName["_COMPONENT_NOT_FOUND_"].end());
    }

    inline Iterator<DynamicComponent> dynamicBegin() {
      return Iterator<DynamicComponent>(this, dynamicComponents.begin());
    }

    inline Iterator<DynamicComponent> dynamicEnd() {
      return Iterator<DynamicComponent>(this, dynamicComponents.end());
    }

  private: 
    
    // TODO: Move to sparse sets.
    std::unordered_map<EntityId, std::vector<ComponentId>> map;

    std::unordered_map<ComponentName, std::vector<ComponentId>> byName;

    std::unordered_map<ComponentId, ComponentEntry> byId;

    std::unordered_map<std::type_index, ComponentName> nameMap;

    std::vector<ComponentId> dynamicComponents;

    static ComponentId nextId() {
      return CurrentId++;
    }

    static ComponentId CurrentId;

    // std::unordered_map<std::string, Entry*> pools;
  };

}

#endif //ENTITY_COMPONENT_MANAGER_H
