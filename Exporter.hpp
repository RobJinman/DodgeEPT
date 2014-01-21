#ifndef __EXPORTER_HPP__
#define __EXPORTER_HPP__


#include <memory>
#include <list>
#include <string>


class MapSettings;
class ObjectContainer;

class Exporter {
   public:
      Exporter(const std::string& path);

      void export_(const MapSettings& settings, ObjectContainer& objects);

   private:
      struct file_t {
         std::string dir;
         std::string name;
         std::list<std::string> includes;
         std::list<std::shared_ptr<EptObject> > assets;
      };

      void exportMapFile(const MapSettings& settings, const file_t& file);
      void exportObjects(const std::vector<file_t>& files);

      void finaliseLocation(const MapSettings& settings,
         std::shared_ptr<EptObject> obj,
         ObjectContainer& objects,
         const std::set<long>& dependents,
         std::map<long, int>& membership,
         std::vector<file_t>& fileList);

      void computeLocations(const MapSettings& settings,
         ObjectContainer& objects, std::vector<file_t>& fileList);

      void doExport(const MapSettings& settings, const std::vector<file_t>& fileList);

      std::string m_path;
};


#endif
