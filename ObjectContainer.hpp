#ifndef __OBJECT_CONTAINER_HPP__
#define __OBJECT_CONTAINER_HPP__


#include <memory>
#include <map>
#include <set>
#include "EptObject.hpp"


class ObjectContainer {
   public:
      typedef std::set<std::weak_ptr<EptObject>, std::owner_less<std::weak_ptr<EptObject> > > wkPtrSet_t;

      class iterator {
         public:
            iterator(const std::set<std::shared_ptr<EptObject> >::iterator& it)
               : m_it(it) {}

            std::weak_ptr<EptObject> operator*() const {
               return *m_it;
            }

            std::weak_ptr<EptObject>* operator->() const {
               m_wkPtr = *m_it;
               return &m_wkPtr;
            }

            iterator operator++() {
               ++m_it;
               return iterator(m_it);
            }

            iterator operator++(int) {
               auto i = m_it;
               ++m_it;
               return iterator(i);
            }

            bool operator==(const iterator& rhs) const {
               return m_it == rhs.m_it;
            }

            bool operator!=(const iterator& rhs) const {
               return !(*this == rhs);
            }

         private:
            std::set<std::shared_ptr<EptObject> >::iterator m_it;
            mutable std::weak_ptr<EptObject> m_wkPtr;
      };

      ObjectContainer();

      bool contains(const QString& name) const;

      void insert(std::shared_ptr<EptObject> obj);

      inline std::weak_ptr<EptObject> get(long id) const;
      inline std::weak_ptr<EptObject> get(const QString& name) const;
      inline const wkPtrSet_t& get(EptObject::type_t type) const;
      const wkPtrSet_t& get(int i, int j) const;

      void move(const QString& name, int i, int j);

      void changeType(const QString& name, EptObject::type_t type);

      inline iterator begin();
      inline iterator end();

   private:
      void resize(int i, int j);

      std::set<std::shared_ptr<EptObject> > m_objects;
      wkPtrSet_t m_prototypes;
      wkPtrSet_t m_instances;
      std::map<long, std::weak_ptr<EptObject> > m_byId;
      std::map<QString, std::weak_ptr<EptObject> > m_byName;
      std::vector<std::vector<wkPtrSet_t> > m_bySegment;
      wkPtrSet_t m_globals;
};

inline ObjectContainer::iterator ObjectContainer::begin() {
   return iterator(m_objects.begin());
}

inline ObjectContainer::iterator ObjectContainer::end() {
   return iterator(m_objects.end());
}

inline std::weak_ptr<EptObject> ObjectContainer::get(const QString& name) const {
   auto i = m_byName.find(name);
   return i == m_byName.end() ? std::weak_ptr<EptObject>() : i->second;
}

inline std::weak_ptr<EptObject> ObjectContainer::get(long id) const {
   auto i = m_byId.find(id);
   return i == m_byId.end() ? std::weak_ptr<EptObject>() : i->second;
}

inline const ObjectContainer::wkPtrSet_t& ObjectContainer::get(EptObject::type_t type) const {
   switch (type) {
      case EptObject::PROTOTYPE: return m_prototypes;
      case EptObject::INSTANCE:  return m_instances;
      default: assert(false);
   }
}


#endif
