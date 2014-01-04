#include "EptObject.hpp"


using namespace std;
using namespace Dodge;


EptObject::EptObject(const QString& name, type_t type)
   : m_type(type), m_name(name) {

   m_xml = shared_ptr<XmlDocument>(new XmlDocument);
}
