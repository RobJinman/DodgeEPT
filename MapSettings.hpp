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
      MapSettings();

      Dodge::Range boundary;
      Dodge::Vec2i numSegments;
      Dodge::Vec2f segmentSize;

      // Relative to data root
      std::string filePath;
      std::string segmentsPath;

      std::shared_ptr<Dodge::XmlDocument> customSettings;

      void loadFromXml(Dodge::XmlNode node);
      Dodge::XmlDocument toXml() const;
};


#endif
