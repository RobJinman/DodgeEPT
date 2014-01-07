#include "EptObject.hpp"


using namespace std;
using namespace Dodge;


long EptObject::m_nextId = 0;


//===========================================
// EptObject::EptObject
//===========================================
EptObject::EptObject(const QString& name, type_t type)
   : m_id(m_nextId++), m_type(type), m_name(name) {

   m_xml = shared_ptr<XmlDocument>(new XmlDocument);
}

//===========================================
// EptObject::computeDependenciesFromXml
//===========================================
const std::set<long>& EptObject::computeDependenciesFromXml() const {
   // TODO
}
