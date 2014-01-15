#ifndef __IMPORTER_HPP__
#define __IMPORTER_HPP__


#include <string>
#include <dodge/xml/xml.hpp>


class ObjectContainer;
class MapSettings;

class Importer {
   public:
      Importer(const std::string& path);

      void import(const Dodge::XmlNode& assets, const MapSettings& settings, ObjectContainer& objects);

   private:
      void importAssets(const MapSettings& settings, ObjectContainer& objects);

      std::string m_path;
};


#endif
