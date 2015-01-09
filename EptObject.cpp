#include <sstream>
#include "Common.hpp"
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
// EptObject::EptObject
//===========================================
EptObject::EptObject(const XmlNode& node, type_t type)
   : m_type(type) {

   m_xml = shared_ptr<XmlDocument>(new XmlDocument);

   XML_NODE_CHECK(node, asset);

   bool idSet = false;
   XmlAttribute attr = node.firstAttribute();
   while (!attr.isNull()) {
      if (attr.name().compare("assetId") == 0) {
         m_id = attr.getLong();
         idSet = true;
      }

      attr = attr.nextAttribute();
   }

   m_xml->addNode(node);

   if (!idSet) {
      m_id = m_nextId++;
   }
   else {
      if (m_id >= m_nextId) m_nextId = m_id + 1;
   }

   stringstream ss;
   ss << "asset" << m_id;

   m_name = QString(ss.str().data());

   if (!idSet)
      EXCEPTION("Error constructing EptObject; Expected 'assetId' attribute");
}

//===========================================
// EptObject::EptObject
//===========================================
EptObject::EptObject(type_t type)
   : m_id(m_nextId++), m_type(type) {

   stringstream ss;
   ss << "asset" << m_id;

   m_name = QString(ss.str().data());
   m_xml = shared_ptr<XmlDocument>(new XmlDocument);
}

//===========================================
// EptObject::parseXml
//===========================================
void EptObject::parseXml(const QString& text, XmlParseResult& result) {
   QByteArray bytes = text.toLocal8Bit();
   const char* data = bytes.data();
   int len = bytes.length();

   result.result = XmlParseResult::SUCCESS;

   try {
      m_xml->parse(data, len);

      XmlNode node = m_xml->firstNode();
      if (node.name() != "asset") {
         result.result = XmlParseResult::FAILURE;
         result.msg = "Expected 'asset' tag";
         return;
      }

      XmlAttribute attr = node.firstAttribute();
      bool b = false;
      while (!attr.isNull()) {
         if (attr.name() == "assetId") {
            stringstream ss;
            ss << m_id;

            attr.setValue(ss.str());
            b = true;
            break;
         }

         attr = attr.nextAttribute();
      }

      if (!b) {
         result.result = XmlParseResult::FAILURE;
         result.msg = "Expected 'assetId' attribute";
         return;
      }

      computeDependencies();
   }
   catch (XmlException& e) {
      result.result = XmlParseResult::FAILURE;
      result.msg = e.what();
   }
}

//===========================================
// EptObject::computeDependencies
//
// Traverse XML tree looking for attributes named 'assetId' or
// 'protoId'.
//===========================================
void EptObject::computeDependencies() {
   XmlNode node = m_xml->firstNode();
   XML_NODE_CHECK(node, asset);

   m_dependencies.clear();
   computeDependencies_r(node);
}

//===========================================
// EptObject::computeDependencies_r
//===========================================
void EptObject::computeDependencies_r(const XmlNode& node, int depth) {

   XmlAttribute attr = node.firstAttribute();
   while (!attr.isNull()) {
      if (attr.name() == "protoId" || (attr.name() == "assetId" && depth > 0)) {
         try {
            long num = attr.getLong();
            m_dependencies.insert(num);
         }
         catch (XmlException&) {}
      }

      attr = attr.nextAttribute();
   }

   XmlNode ch = node.firstChild();
   while (!ch.isNull()) {
      computeDependencies_r(ch, depth + 1);
      ch = ch.nextSibling();
   }
}
