#include "Common.hpp"
#include "ObjectContainer.hpp"


using namespace std;


//===========================================
// ObjectContainer::ObjectContainer
//===========================================
ObjectContainer::ObjectContainer() {
   resize(1, 1);
}

//===========================================
// ObjectContainer::insert
//===========================================
void ObjectContainer::insert(shared_ptr<EptObject> obj) {
   resize(obj->m_segment.x, obj->m_segment.y);

   m_objects.insert(obj);

   switch (obj->type()) {
      case EptObject::PROTOTYPE: m_prototypes.insert(obj); break;
      case EptObject::INSTANCE:  m_instances.insert(obj);  break;
      default: assert(false);
   }

   m_byName.insert(make_pair(obj->name(), obj));
   m_bySegment[obj->m_segment.x][obj->m_segment.y].insert(obj);
}

//===========================================
// ObjectContainer::contains
//===========================================
bool ObjectContainer::contains(const QString& name) const {
   return m_byName.find(name) != m_byName.end();
}

//===========================================
// ObjectContainer::get
//===========================================
weak_ptr<EptObject> ObjectContainer::get(const QString& name) const {
   auto i = m_byName.find(name);
   return i == m_byName.end() ? weak_ptr<EptObject>() : i->second;
}

//===========================================
// ObjectContainer::get
//===========================================
const ObjectContainer::wkPtrSet_t& ObjectContainer::get(int i, int j) const {
   if (i < 0) EXCEPTION("Index out of range");
   if (j < 0) EXCEPTION("Index out of range");
   if (i >= static_cast<int>(m_bySegment.size())) EXCEPTION("Index out of range");
   if (j >= static_cast<int>(m_bySegment[i].size())) EXCEPTION("Index out of range");

   return m_bySegment[i][j];
}

//===========================================
// ObjectContainer::move
//===========================================
void ObjectContainer::move(const QString& name, int i, int j) {
   auto it = m_byName.find(name);

   if (it == m_byName.end())
      EXCEPTION("Object '" << name.toLocal8Bit().data() << "' not found");

   auto ptr = it->second.lock();
   assert(ptr);

   int x = ptr->m_segment.x;
   int y = ptr->m_segment.y;

   ptr->m_segment = Dodge::Vec2i(i, j);

   insert(ptr);
   m_bySegment[x][y].erase(ptr);
}

//===========================================
// ObjectContainer::resize
//
// Ignores requests to down-size
//===========================================
void ObjectContainer::resize(int i, int j) {
   if (i < 0) EXCEPTION("Index out of range");
   if (j < 0) EXCEPTION("Index out of range");

   if (i > static_cast<int>(m_bySegment.size()))
      m_bySegment.resize(i);

   assert(m_bySegment.size() > 0);

   if (j > static_cast<int>(m_bySegment[0].size())) {
      for (auto it = m_bySegment.begin(); it != m_bySegment.end(); ++it)
         it->resize(j);
   }
}
