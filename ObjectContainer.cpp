#include "Common.hpp"
#include "ObjectContainer.hpp"


using namespace std;
using namespace Dodge;


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
   Vec2i& seg = EptObjAccessor::segment(*obj);

   bool global = false;

   if (seg.x < 0 || seg.y < 0) {
      if (seg.x == -1 && seg.y == -1)
         global = true;
      else
         EXCEPTION("Index out of range");
   }

   if (!global) resize(seg.x + 1, seg.y + 1);

   m_objects.insert(obj);

   switch (obj->type()) {
      case EptObject::PROTOTYPE: m_prototypes.insert(obj); break;
      case EptObject::INSTANCE:  m_instances.insert(obj);  break;
      default: assert(false);
   }

   m_byId.insert(make_pair(obj->id(), obj));
   m_byName.insert(make_pair(obj->name(), obj));

   if (global)
      m_globals.insert(obj);
   else
      m_bySegment[seg.x][seg.y].insert(obj);
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
const ObjectContainer::wkPtrSet_t& ObjectContainer::get(int i, int j) const {
   if (i == -1 && j == -1) return m_globals;
   if (i < 0) EXCEPTION("Index out of range");
   if (j < 0) EXCEPTION("Index out of range");
   if (i >= static_cast<int>(m_bySegment.size()) || j >= static_cast<int>(m_bySegment[i].size()))
      return m_nullSet;

   return m_bySegment[i][j];
}

//===========================================
// ObjectContainer::move
//===========================================
void ObjectContainer::move(long id, int i, int j) {
   auto it = m_byId.find(id);

   if (it == m_byId.end())
      EXCEPTION("No object with id " << id);

   auto ptr = it->second.lock();
   assert(ptr);

   Vec2i& seg = EptObjAccessor::segment(*ptr);

   int x = seg.x;
   int y = seg.y;
   seg = Dodge::Vec2i(i, j);

   m_globals.erase(ptr);
   insert(ptr);

   if (x != -1 && y != -1) {
      assert(x >= 0 && y >= 0);
      m_bySegment[x][y].erase(ptr);
   }
}

//===========================================
// ObjectContainer::changeName
//===========================================
void ObjectContainer::changeName(long id, const QString& name) {
   auto i = m_byId.find(id);

   if (i == m_byId.end())
      EXCEPTION("No object with id " << id);

   auto ptr = i->second.lock();
   assert(ptr);

   m_byName.erase(ptr->name());
   EptObjAccessor::name(*ptr) = name;

   m_byName.insert(make_pair(name, ptr));
}

//===========================================
// ObjectContainer::changeType
//===========================================
void ObjectContainer::changeType(long id, EptObject::type_t type) {
   auto i = m_byId.find(id);

   if (i == m_byId.end())
      EXCEPTION("No object with id " << id);

   auto ptr = i->second.lock();
   assert(ptr);

   EptObjAccessor::type(*ptr) = type;

   if (type == EptObject::PROTOTYPE) {
      m_instances.erase(ptr);
   }
   else if (type == EptObject::INSTANCE) {
      m_prototypes.erase(ptr);
   }
   else
      assert(false);

   insert(ptr);
}

//===========================================
// ObjectContainer::clear
//===========================================
void ObjectContainer::clear() {
   m_prototypes.clear();
   m_instances.clear();
   m_byId.clear();
   m_byName.clear();
   m_bySegment.clear();
   m_globals.clear();
   m_objects.clear();
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

   unsigned int h = (j > static_cast<int>(m_bySegment[0].size()) ? j : m_bySegment[0].size());

   for (auto it = m_bySegment.begin(); it != m_bySegment.end(); ++it) {
      if (it->size() < h) {
         it->resize(h);
      }
      else
         assert(it->size() == h);
   }
}
