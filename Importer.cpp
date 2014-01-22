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
   : m_dataRoot(path) {}

//===========================================
// Importer::import
//===========================================
void Importer::import(MapSettings& settings, ObjectContainer& objects) {
   PRINT_STRING("importer", "Beginning import (" << m_dataRoot << ") ...");

   m_numErrors = 0;

   try {
      readMapFile(settings, objects);
   }
   catch (XmlException& e) {
      PRINT_STRING("importer", "\tBad map file; " << e.what() << ". Continuing ...");
      ++m_numErrors;
   }

   importPrototypes(objects);
   importSegments(settings, objects);

   for (auto i = objects.begin(); i != objects.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      obj->computeDependencies();
   }

   if (m_numErrors == 0) {
      PRINT_STRING("importer", "Import successful");
   }
   else {
      PRINT_STRING("importer", "Import finished with " << m_numErrors << " errors");
   }
}

//===========================================
// Importer::importPrototypes
//===========================================
void Importer::importPrototypes(ObjectContainer& objects) {
   vector<string> files;
   filesInDir(m_dataRoot, files);

   for (unsigned int i = 0; i < files.size(); ++i) {
      int l = files[i].length();

      if (l < 4) continue;
      if (files[i].substr(l - 4, 4).compare(".xml") != 0) continue;

      try {
         // Put the prototype in segment 0,0 and let the exporter decide whether to move it.
         readAssetFile(objects, m_dataRoot + "/" + files[i], EptObject::PROTOTYPE, Vec2i(0, 0));
      }
      catch (XmlException& e) {
         PRINT_STRING("importer", "\tBad prototype file; " << e.what() << ". Continuing ...");
         ++m_numErrors;
      }
   }
}

//===========================================
// Importer::readMapFile
//===========================================
void Importer::readMapFile(MapSettings& settings, ObjectContainer& objects) {
   string mapFilePath = m_dataRoot + "/" + settings.filePath;

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
      objects.move(ent->id(), -1, -1);

      node_ = node_.nextSibling();
   }
}

//===========================================
// Importer::readAssetFile
//===========================================
void Importer::readAssetFile(ObjectContainer& objects, const string& path, EptObject::type_t type, const Dodge::Vec2i& segment) {
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
      objects.move(ent->id(), segment.x, segment.y);

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
         ss << m_dataRoot << "/" << settings.segmentsPath << "/" << i << j << ".xml";

         try {
            readAssetFile(objects, ss.str(), EptObject::INSTANCE, Vec2i(i, j));
         }
         catch (XmlException& e) {
            PRINT_STRING("importer", "\tBad segment file; " << e.what() << ". Continuing ...");
            ++m_numErrors;
         }
      }
   }
}
