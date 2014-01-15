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
void Importer::import(MapSettings& settings, ObjectContainer& objects) {
   string mapFilePath = m_path + "/" + settings.fileName;

   shared_ptr<XmlDocument> doc(new XmlDocument);
   doc->parse(mapFilePath);

   // <settings/>
   // <customSettings/>
   // <using/>
   // <assets/>

   XmlNode node = doc->firstNode();
   node = node.nextSibling();
   XML_NODE_CHECK(node, MAPFILE);

   node = node.firstChild();
   settings.loadFromXml(node);

   node = node.nextSibling();
   XML_NODE_CHECK(node, customSettings);

   node = node.nextSibling();
   XML_NODE_CHECK(node, using);

   vector<string> includes;

   XmlNode node_ = node.firstChild();
   while (!node_.isNull()) {
      XML_NODE_CHECK(node_, file);

      includes.push_back(node_.getString());

      node_ = node_.nextSibling();
   }

   node = node.nextSibling();
   XML_NODE_CHECK(node, assets);

   node_ = node.firstChild();
   while (!node_.isNull()) {
      XML_NODE_CHECK(node_, asset);

      shared_ptr<EptObject> ent = shared_ptr<EptObject>(new EptObject(node_, EptObject::INSTANCE));
      objects.insert(ent);

      node_ = node_.nextSibling();
   }

   importAssets(settings, includes, objects);

   for (auto i = objects.begin(); i != objects.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      obj->computeDependencies();
   }
}

//===========================================
// Importer::importAssets
//===========================================
void Importer::importAssets(const MapSettings& settings, const vector<string>& includes, ObjectContainer& objects) {
   for (unsigned int i = 0; i < includes.size(); ++i) {
      stringstream path;
      path << m_path << "/" << includes[i];

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
