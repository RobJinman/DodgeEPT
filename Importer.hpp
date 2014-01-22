#ifndef __IMPORTER_HPP__
#define __IMPORTER_HPP__


#include <string>
#include <vector>
#include <dodge/xml/xml.hpp>
#include "EptObject.hpp"


class ObjectContainer;
class MapSettings;

class Importer {
   public:
      Importer(const std::string& path);

      void import(MapSettings& settings, ObjectContainer& objects);

   private:
      void importPrototypes(ObjectContainer& objects);
      void readMapFile(MapSettings& settings, ObjectContainer& objects);
      void readAssetFile(ObjectContainer& objects, const std::string& path, EptObject::type_t type, const Dodge::Vec2i& segment);
      void importSegments(const MapSettings& settings, ObjectContainer& objects);

      std::string m_dataRoot;
      int m_numErrors;
};


#endif
