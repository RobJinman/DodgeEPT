#ifndef __WGT_MAP_SETTINGS_HPP__
#define __WGT_MAP_SETTINGS_HPP__


#include <QWidget>
#include <dodge/Range.hpp>
#include <dodge/math/Vec2i.hpp>


class MapSettings {
   public:
      Dodge::Range boundary;
      Dodge::Vec2i numSegments;
      Dodge::Vec2f segmentSize;
      std::string filePath;

      Dodge::XmlDocument toXml() const;
};


class QGroupBox;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;

class WgtMapSettings : public QWidget {
   Q_OBJECT

   public:
      WgtMapSettings(QWidget* parent = NULL);

      const MapSettings& mapSettings() const;

   private slots:

   signals:

   private:
      mutable MapSettings m_mapSettings;

      QGroupBox*        m_wgtGrpMapBoundary;
      QLabel*           m_wgtLblMapPosition;
      QLabel*           m_wgtLblMapPositionX;
      QLabel*           m_wgtLblMapPositionY;
      QLabel*           m_wgtLblMapSize;
      QLabel*           m_wgtLblMapSizeW;
      QLabel*           m_wgtLblMapSizeH;
      QDoubleSpinBox*   m_wgtSpnMapPositionX;
      QDoubleSpinBox*   m_wgtSpnMapPositionY;
      QDoubleSpinBox*   m_wgtSpnMapSizeW;
      QDoubleSpinBox*   m_wgtSpnMapSizeH;
      QGroupBox*        m_wgtGrpMapSegments;
      QLabel*           m_wgtLblMapSegmentsQuantity;
      QLabel*           m_wgtLblMapSegmentsQuantityW;
      QLabel*           m_wgtLblMapSegmentsQuantityH;
      QLabel*           m_wgtLblMapSegmentsSize;
      QLabel*           m_wgtLblMapSegmentsSizeW;
      QLabel*           m_wgtLblMapSegmentsSizeH;
      QSpinBox*         m_wgtSpnMapSegmentsQuantityW;
      QSpinBox*         m_wgtSpnMapSegmentsQuantityH;
      QDoubleSpinBox*   m_wgtSpnMapSegmentsSizeW;
      QDoubleSpinBox*   m_wgtSpnMapSegmentsSizeH;
};


#endif
