#ifndef __MAP_SETTINGS_HPP__
#define __MAP_SETTINGS_HPP__


#include <memory>
#include <vector>
#include <string>
#include <QString>
#include <dodge/xml/xml.hpp>
#include <dodge/Range.hpp>
#include <dodge/math/Vec2i.hpp>


class MapSettings {
   public:
      Dodge::Range boundary;
      Dodge::Vec2i numSegments;
      Dodge::Vec2f segmentSize;
      std::string fileName;
      std::string segmentsDir;
      std::vector<QString> includes;
      std::vector<std::weak_ptr<Dodge::XmlDocument> > assets;

      void loadSettingsFromXml(Dodge::XmlNode node);
      void loadIncludesFromXml(Dodge::XmlNode node);
      Dodge::XmlDocument toXml() const;
};


#endif
