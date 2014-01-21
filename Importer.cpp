#include <sstream>
#include "Importer.hpp"
#include "MapSettings.hpp"
#include "ObjectContainer.hpp"
#include "Common.hpp"


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
   readMapFile(settings, objects);
   importPrototypes(objects);
   importSegments(settings, objects);

   for (auto i = objects.begin(); i != objects.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      obj->computeDependencies();
   }
}

//===========================================
// Importer::importPrototypes
//===========================================
void Importer::importPrototypes(ObjectContainer& objects) {
   vector<string> files;
   filesInDir(m_path, files);

   for (unsigned int i = 0; i < files.size(); ++i) {
      int l = files[i].length();

      if (l < 4) continue;
      if (files[i].substr(l - 4, 4).compare(".xml") != 0) continue;

      readAssetFile(objects, m_path + "/" + files[i], EptObject::PROTOTYPE);
   }
}

//===========================================
// Importer::readMapFile
//===========================================
void Importer::readMapFile(MapSettings& settings, ObjectContainer& objects) {
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

   node = node.nextSibling();
   XML_NODE_CHECK(node, assets);

   XmlNode node_ = node.firstChild();
   while (!node_.isNull()) {
      XML_NODE_CHECK(node_, asset);

      shared_ptr<EptObject> ent = shared_ptr<EptObject>(new EptObject(node_, EptObject::INSTANCE));
      objects.insert(ent);

      node_ = node_.nextSibling();
   }
}

//===========================================
// Importer::readAssetFile
//===========================================
void Importer::readAssetFile(ObjectContainer& objects, const string& path, EptObject::type_t type) {
   XmlDocument doc;
   doc.parse(path);

   XmlNode node = doc.firstNode();
   node = node.nextSibling();
   if (node.isNull() || node.name() != "ASSETFILE") return;

   node = node.firstChild();
   XML_NODE_CHECK(node, using);

   node = node.nextSibling();
   XML_NODE_CHECK(node, assets);

   node = node.firstChild();
   while (!node.isNull()) {
      XML_NODE_CHECK(node, asset);

      shared_ptr<EptObject> ent(new EptObject(node, type));
      objects.insert(ent);

      node = node.nextSibling();
   }
}

//===========================================
// Importer::importSegments
//===========================================
void Importer::importSegments(const MapSettings& settings, ObjectContainer& objects) {
   const Vec2i& segs = settings.numSegments;

   for (int i = 0; i < segs.x; ++i) {
      for (int j = 0; j < segs.y; ++j) {
         stringstream ss;
         ss << m_path << "/" << settings.segmentsDir << "/" << i << j << ".xml";

         try {
            readAssetFile(objects, ss.str(), EptObject::INSTANCE);
         }
         catch (XmlException&) {
            // TODO
            throw;

            continue;
         }
      }
   }
}
