#ifndef __WGT_MAP_SETTINGS_HPP__
#define __WGT_MAP_SETTINGS_HPP__


#include <memory>
#include <QWidget>
#include "MapSettings.hpp"


class QGroupBox;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class EptObject;

class WgtMapSettings : public QWidget {
   Q_OBJECT

   public:
      WgtMapSettings(QWidget* parent = NULL);

      void update();

      MapSettings& mapSettings();

   private slots:
      void onChange(int);
      void onChange(double);
      void onBtnCSSelectClick();

   signals:
      void changed();
      void customSettingsSelected();

   private:
      MapSettings m_mapSettings;

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
      QGroupBox*        m_wgtGrpCustomSettings;
      QPushButton*      m_wgtBtnCSSelect;
};


#endif
