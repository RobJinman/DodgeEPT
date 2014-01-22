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
   : m_dataRoot(path) {}

//===========================================
// Exporter::finaliseLocation
//===========================================
void Exporter::finaliseLocation(
   const MapSettings& settings,
   shared_ptr<EptObject> obj,
   ObjectContainer& objects,
   const set<long>& dependents,
   map<long, int>& membership, // File membership (idx of fileList)
   vector<file_t>& fileList) {

   // Place in user-specified location
   if (dependents.size() == 0) {
      if (obj->type() == EptObject::INSTANCE) {
         const Vec2f& seg = obj->segment();
         int idx = seg.y * settings.numSegments.x + seg.x + 1; // Add 1 because first file is map file

         if (seg.x == -1 && seg.y == -1)
            idx = 0;

         fileList[idx].assets.push_front(obj);
         membership.insert(make_pair(obj->id(), idx));
      }
      // If for some reason prototype has no dependents, put a 'using' tag in user-specified location
      else if (obj->type() == EptObject::PROTOTYPE) {
         const Vec2f& seg = obj->segment();
         int idx = seg.y * settings.numSegments.x + seg.x + 1; // Add 1 because first file is map file

         if (seg.x == -1 && seg.y == -1)
            idx = 0;

         stringstream ss;
         ss << obj->name().toLocal8Bit().data() << ".xml";

         fileList[idx].includes.push_front(ss.str());
      }
   }
   else {
      bool sameLocation = true;

      int idx = -1;
      Vec2i seg(-2, -2);
      for (auto i = dependents.begin(); i != dependents.end(); ++i) {
         auto pDep = objects.get(*i).lock();
         assert(pDep);

         auto it = membership.find(*i);
         assert(it != membership.end());
         int depIdx = it->second;

         if (idx == -1) {
            seg = pDep->segment();
            idx = depIdx;
            continue;
         }

         if (idx != depIdx) {
            sameLocation = false;
            break;
         }
      }

      assert(idx != -1);
      assert(seg != Vec2i(-2, -2));

      // If all dependents are in the same segment
      if (sameLocation) {
         if (obj->type() == EptObject::INSTANCE) {
            fileList[idx].assets.push_front(obj);
            membership.insert(make_pair(obj->id(), idx));
         }
         else if (obj->type() == EptObject::PROTOTYPE) {
            stringstream ss;
            ss << obj->name().toLocal8Bit().data() << ".xml";

            fileList[idx].includes.push_front(ss.str());
         }

         if (obj->segment() != seg)
            objects.move(obj->id(), seg.x, seg.y);
      }
      // If dependents are spread across multiple files, make global
      else {
         if (obj->type() == EptObject::INSTANCE) {
            fileList[0].assets.push_front(obj);
            membership.insert(make_pair(obj->id(), 0));
         }
         else if (obj->type() == EptObject::PROTOTYPE) {
            stringstream ss;
            ss << obj->name().toLocal8Bit().data() << ".xml";

            fileList[0].includes.push_front(ss.str());
         }

         if (obj->segment() != Vec2i(-1, -1))
            objects.move(obj->id(), -1, -1);
      }
   }
}

//===========================================
// Exporter::computeLocations
//===========================================
void Exporter::computeLocations(
   const MapSettings& settings,
   ObjectContainer& objects,
   vector<file_t>& fileList) {

   PRINT_STRING("exporter", "\tComputing dependencies ...");

   map<long, set<long> > dependents;
   map<long, int> numDependents;
   set<long> zeroDependents;
   vector<long> pending;

   // For each object
   for (auto i = objects.begin(); i != objects.end(); ++i) {
      shared_ptr<EptObject> obj = i->lock();
      assert(obj);

      if (dependents.find(obj->id()) == dependents.end()) {
         dependents.insert(make_pair(obj->id(), set<long>()));
         numDependents[obj->id()] = 0;
      }

      obj->computeDependencies();
      const set<long>& dependencies = obj->dependencies();

      // Populate dependents map
      for (auto dep = dependencies.begin(); dep != dependencies.end(); ++dep) {
         dependents[*dep].insert(obj->id());
         numDependents[*dep] += 1;
      }
   }

   for (auto i = dependents.begin(); i != dependents.end(); ++i) {
      if (i->second.size() == 0) pending.push_back(i->first);
   }

   map<long, int> membership;

   auto objs = objects.get(EptObject::PROTOTYPE);
   for (auto i = objs.begin(); i != objs.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      stringstream ss;
      ss << obj->name().toLocal8Bit().data() << ".xml";

      file_t protoFile;
      protoFile.dir = m_dataRoot;
      protoFile.name = ss.str();
      protoFile.assets.push_back(obj);

      fileList.push_back(protoFile);
      membership.insert(make_pair(obj->id(), fileList.size() - 1));
   }

   for (unsigned int i = 0; i < pending.size(); ++i) {
      shared_ptr<EptObject> obj = objects.get(pending[i]).lock();
      assert(obj);

      auto it = dependents.find(obj->id());
      assert(it != dependents.end());

      finaliseLocation(settings, obj, objects, it->second, membership, fileList);

      const set<long>& dependencies = obj->dependencies();

      for (auto dep = dependencies.begin(); dep != dependencies.end(); ++dep) {
         auto it = numDependents.find(*dep);
         assert(it != numDependents.end());

         it->second--;

         if (it->second == 0) pending.push_back(*dep);
      }
   }
}

//===========================================
// Exporter::exportMapFile
//===========================================
void Exporter::exportMapFile(const MapSettings& settings, const file_t& file) {
   XmlDocument xml = settings.toXml();

   XmlNode customSettings = xml.addNode("customSettings");
   XmlNode using_ = xml.addNode("using");
   for (auto i = file.includes.begin(); i != file.includes.end(); ++i) {
      XmlNode file_ = using_.addNode("file");
      file_.setValue(*i);
   }

   XmlNode assets = xml.addNode("assets");
   for (auto iObj = file.assets.begin(); iObj != file.assets.end(); ++iObj) {
      auto obj = *iObj;

      auto doc = obj->xml().lock();
      assert(doc);

      assets.addNode(doc->firstNode());
   }

   if (!createDir(file.dir))
      EXCEPTION("Error creating directory '" << file.dir << "'");

   stringstream path;
   path << file.dir << "/" << file.name;

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

   PRINT_STRING("exporter", "\t\t" << path.str());
}

//===========================================
// Exporter::exportObjects
//===========================================
void Exporter::exportObjects(const vector<file_t>& fileList) {
   // First file is map file
   for (unsigned int i = 1; i < fileList.size(); ++i) {
      const file_t& file = fileList[i];

      if (!createDir(file.dir))
         EXCEPTION("Error creating directory '" << file.dir << "'");

      stringstream ss;
      ss << file.dir << "/" << file.name;

      ofstream fout(ss.str());
      if (!fout.good()) {
         fout.close();
         EXCEPTION("Error opening file '" << ss.str() << "'");
      }

      fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
      fout << "<ASSETFILE>\n";

      fout << "<using>\n";
      for (auto it = file.includes.begin(); it != file.includes.end(); ++it) {
         fout << "\t<file>" << *it << "</file>\n";
      }
      fout << "</using>\n";

      fout << "<assets>\n";

      for (auto iObj = file.assets.begin(); iObj != file.assets.end(); ++iObj) {
         auto obj = *iObj;

         auto xml = obj->xml().lock();
         assert(xml);

         xml->print(fout);
      }

      fout << "</assets>\n";
      fout << "</ASSETFILE>\n";

      fout.close();

      PRINT_STRING("exporter", "\t\t" << ss.str());
   }
}

//===========================================
// Exporter::doExport
//===========================================
void Exporter::doExport(const MapSettings& settings, const vector<file_t>& fileList) {
   PRINT_STRING("exporter", "\tWriting files ...");

   assert(fileList.size() > 0);

   exportMapFile(settings, fileList[0]);
   exportObjects(fileList);
}

//===========================================
// Exporter::export_
//===========================================
void Exporter::export_(const MapSettings& settings, ObjectContainer& objects) {
   PRINT_STRING("exporter", "Beginning export (" << m_dataRoot << ") ...");

   m_numErrors = 0;

   if (!createDir(m_dataRoot))
      EXCEPTION("Error creating directory '" << m_dataRoot << "'");

   vector<file_t> fileList;

   file_t global;
   global.dir = m_dataRoot;
   global.name = settings.filePath;
   fileList.push_back(global);

   const Vec2i& segs = settings.numSegments;
   for (int i = 0; i < segs.x; ++i) {
      for (int j = 0; j < segs.y; ++j) {
         stringstream ss;

         file_t file;
         file.dir = m_dataRoot + "/" + settings.segmentsPath;
 
         ss << i << j << ".xml";

         file.name = ss.str();

         fileList.push_back(file);
      }
   }

   computeLocations(settings, objects, fileList);

   doExport(settings, fileList);

   if (m_numErrors == 0) {
      PRINT_STRING("exporter", "Export successful");
   }
   else {
      PRINT_STRING("exporter", "Export finished with " << m_numErrors << " errors");
   }
}
