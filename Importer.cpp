#include <sstream>
#include "Importer.hpp"
#include "MapSettings.hpp"
#include "ObjectContainer.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Importer::Importer
//===========================================
Importer::Importer(const std::string& path)
   : m_path(path) {}

//===========================================
// Importer::import
//===========================================
void Importer::import(const XmlNode& assets, const MapSettings& settings, ObjectContainer& objects) {
   XML_NODE_CHECK(assets, assets);
   XmlNode node = assets.firstChild();

   while (!node.isNull()) {
      XML_NODE_CHECK(node, asset);

      shared_ptr<EptObject> ent = shared_ptr<EptObject>(new EptObject(node, EptObject::INSTANCE));
      objects.insert(ent);

      node = node.nextSibling();
   }

   importAssets(settings, objects);

   for (auto i = objects.begin(); i != objects.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      obj->computeDependencies();
   }
}

//===========================================
// Importer::importAssets
//===========================================
void Importer::importAssets(const MapSettings& settings, ObjectContainer& objects) {
   for (unsigned int i = 0; i < settings.includes.size(); ++i) {
      stringstream path;
      path << m_path << "/" << settings.includes[i].toLocal8Bit().data();

      try {
         XmlDocument doc;
         doc.parse(path.str());

         XmlNode node = doc.firstNode();
         node = node.nextSibling();
         XML_NODE_CHECK(node, ASSETFILE);

         node = node.firstChild();
         XML_NODE_CHECK(node, assets);

         node = node.firstChild();

         while (!node.isNull()) {
            XML_NODE_CHECK(node, asset);

            shared_ptr<EptObject> ent(new EptObject(node, EptObject::INSTANCE));
            objects.insert(ent);

            node = node.nextSibling();
         }
      }
      catch (XmlException&) {
         // TODO
         throw;

         continue;
      }
   }

   const Vec2i& segs = settings.numSegments;

   for (int i = 0; i < segs.x; ++i) {
      for (int j = 0; j < segs.y; ++j) {
         stringstream ss;
         ss << m_path << "/" << settings.segmentsDir << "/" << i << j << ".xml";

         try {
            XmlDocument doc;
            doc.parse(ss.str());

            XmlNode node = doc.firstNode();
            node = node.nextSibling();
            XML_NODE_CHECK(node, ASSETFILE);

            node = node.firstChild();
            XML_NODE_CHECK(node, assets);

            node = node.firstChild();
            while (!node.isNull()) {
               XML_NODE_CHECK(node, asset);

               shared_ptr<EptObject> ent(new EptObject(node, EptObject::INSTANCE));
               objects.insert(ent);

               node = node.nextSibling();
            }
         }
         catch (XmlException&) {
            // TODO
            throw;

            continue;
         }
      }
   }
}
