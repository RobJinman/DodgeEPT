#ifndef __EXPORTER_HPP__
#define __EXPORTER_HPP__


#include <string>


class MapSettings;
class ObjectContainer;

class Exporter {
   public:
      Exporter(const std::string& path);

      void export_(const MapSettings& settings, const ObjectContainer& objects);

   private:
      void buildMapFile(const MapSettings& settings, const std::vector<std::string>& includes, const ObjectContainer& objects);
      void exportPrototypes(const ObjectContainer& objects, std::vector<std::string>& includes);
      void exportInstances(const MapSettings& settings, const ObjectContainer& objects);

      std::string m_path;
};


#endif
