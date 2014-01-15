#include <sstream>
#include <fstream>
#include <dodge/xml/xml.hpp>
#include "ObjectContainer.hpp"
#include "Exporter.hpp"
#include "MapSettings.hpp"
#include "Common.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Exporter::Exporter
//===========================================
Exporter::Exporter(const std::string& path)
   : m_path(path) {}

//===========================================
// Exporter::buildMapFile
//===========================================
void Exporter::buildMapFile(const MapSettings& settings, const vector<string>& includes, const ObjectContainer& objects) {
   XmlDocument xml = settings.toXml();

   XmlNode customSettings = xml.addNode("customSettings");
   XmlNode using_ = xml.addNode("using");
   for (unsigned int i = 0; i < includes.size(); ++i) {
      XmlNode file = using_.addNode("file");
      file.setValue(includes[i]);
   }

   XmlNode assets = xml.addNode("assets");
   auto objs = objects.get(-1, -1);
   for (auto iObj = objs.begin(); iObj != objs.end(); ++iObj) {
      auto ptr = iObj->lock();
      assert(ptr);

      auto doc = ptr->xml().lock();
      assert(doc);

      assets.addNode(doc->firstNode());
   }

   stringstream path;
   path << m_path << "/" << settings.fileName;

   ofstream fout(path.str());
   if (!fout.good()) {
      fout.close();
      EXCEPTION("Error opening file '" << path.str() << "'");
   }

   fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
   fout << "<MAPFILE>\n";
   xml.print(fout);
   fout << "</MAPFILE>\n";

   fout.close();
}

//===========================================
// Exporter::exportPrototypes
//===========================================
void Exporter::exportPrototypes(const ObjectContainer& objects, vector<string>& includes) {
   const ObjectContainer::wkPtrSet_t& objs = objects.get(EptObject::PROTOTYPE);

   for (auto i = objs.begin(); i != objs.end(); ++i) {
      shared_ptr<EptObject> obj = i->lock();
      assert(obj);

      stringstream ss;
      ss << m_path << "/" << obj->name().toLocal8Bit().data() << ".xml";

      ofstream fout(ss.str());
      if (!fout.good()) {
         fout.close();
         cerr << "Error writing to file '" << ss.str() << "'\n";

         continue;
      }

      includes.push_back(ss.str());

      auto xml = obj->xml().lock();
      assert(xml);

      fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
      fout << "<ASSETFILE>\n";
      fout << "<assets>\n";
      xml->print(fout);
      fout << "</assets>\n";
      fout << "</ASSETFILE>\n";

      fout.close();
   }
}

//===========================================
// Exporter::exportInstances
//===========================================
void Exporter::exportInstances(const MapSettings& settings, const ObjectContainer& objects) {
   const Vec2i& segs = settings.numSegments;

   for (int i = 0; i < segs.x; ++i) {
      for (int j = 0; j < segs.y; ++j) {
         stringstream ss;
         ss << m_path << "/" << settings.segmentsDir;

         if (!createDir(ss.str()))
            EXCEPTION("Error creating directory '" << ss.str() << "'");

         ss << "/" << i << j << ".xml";

         ofstream fout(ss.str());
         if (!fout.good()) {
            fout.close();
            EXCEPTION("Error opening file '" << ss.str() << "'");
         }

         fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
         fout << "<ASSETFILE>\n";
         fout << "<assets>\n";

         auto objs = objects.get(i, j);

         for (auto iObj = objs.begin(); iObj != objs.end(); ++iObj) {
            auto obj = iObj->lock();
            assert(obj);

            if (obj->type() == EptObject::PROTOTYPE) continue;

            auto xml = obj->xml().lock();
            assert(xml);

            xml->print(fout);
         }

         fout << "</assets>\n";
         fout << "</ASSETFILE>\n";

         fout.close();
      }
   }
}

//===========================================
// Exporter::export_
//===========================================
void Exporter::export_(const MapSettings& settings, const ObjectContainer& objects) {
   vector<string> includes;

   exportPrototypes(objects, includes);
   exportInstances(settings, objects);
   buildMapFile(settings, includes, objects);
}
