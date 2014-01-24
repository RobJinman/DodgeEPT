#include <sstream>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "WgtMapSettings.hpp"
#include "EptObject.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// WgtMapSettings::WgtMapSettings
//===========================================
WgtMapSettings::WgtMapSettings(QWidget* parent)
   : QWidget(parent) {

   m_wgtGrpMapBoundary = new QGroupBox("Map Boundary", this);
   m_wgtLblMapPosition = new QLabel("Position", m_wgtGrpMapBoundary);
   m_wgtLblMapPositionX = new QLabel("X", m_wgtGrpMapBoundary);
   m_wgtLblMapPositionY = new QLabel("Y", m_wgtGrpMapBoundary);
   m_wgtLblMapSize = new QLabel("Size", m_wgtGrpMapBoundary);
   m_wgtLblMapSizeW = new QLabel("W", m_wgtGrpMapBoundary);
   m_wgtLblMapSizeH = new QLabel("H", m_wgtGrpMapBoundary);
   m_wgtSpnMapPositionX = new QDoubleSpinBox(m_wgtGrpMapBoundary);
   m_wgtSpnMapPositionY = new QDoubleSpinBox(m_wgtGrpMapBoundary);
   m_wgtSpnMapSizeW = new QDoubleSpinBox(m_wgtGrpMapBoundary);
   m_wgtSpnMapSizeH = new QDoubleSpinBox(m_wgtGrpMapBoundary);
   m_wgtGrpMapSegments = new QGroupBox("Map Segments", this);
   m_wgtLblMapSegmentsQuantity = new QLabel("Quantity", m_wgtGrpMapSegments);
   m_wgtLblMapSegmentsQuantityW = new QLabel("W", m_wgtGrpMapSegments);
   m_wgtLblMapSegmentsQuantityH = new QLabel("H", m_wgtGrpMapSegments);
   m_wgtLblMapSegmentsSize = new QLabel("Size", m_wgtGrpMapSegments);
   m_wgtLblMapSegmentsSizeW = new QLabel("W", m_wgtGrpMapSegments);
   m_wgtLblMapSegmentsSizeH = new QLabel("H", m_wgtGrpMapSegments);
   m_wgtSpnMapSegmentsQuantityW = new QSpinBox(m_wgtGrpMapSegments);
   m_wgtSpnMapSegmentsQuantityH = new QSpinBox(m_wgtGrpMapSegments);
   m_wgtSpnMapSegmentsSizeW = new QDoubleSpinBox(m_wgtGrpMapSegments);
   m_wgtSpnMapSegmentsSizeH = new QDoubleSpinBox(m_wgtGrpMapSegments);
   m_wgtGrpCustomSettings = new QGroupBox("Custom Settings", this);
   m_wgtBtnCSSelect = new QPushButton("Select", m_wgtGrpCustomSettings);

   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(m_wgtGrpMapBoundary);
   layout->addWidget(m_wgtGrpMapSegments);
   layout->addWidget(m_wgtGrpCustomSettings);
   setLayout(layout);

   m_wgtSpnMapPositionX->setSingleStep(0.1);
   m_wgtSpnMapPositionY->setSingleStep(0.1);
   m_wgtSpnMapSizeW->setSingleStep(0.1);
   m_wgtSpnMapSizeH->setSingleStep(0.1);
   m_wgtSpnMapSegmentsSizeW->setSingleStep(0.1);
   m_wgtSpnMapSegmentsSizeH->setSingleStep(0.1);

   QGridLayout* mapBoundaryLayout = new QGridLayout;
   mapBoundaryLayout->addWidget(m_wgtLblMapPosition, 0, 0);
   mapBoundaryLayout->addWidget(m_wgtLblMapPositionX, 0, 1);
   mapBoundaryLayout->addWidget(m_wgtSpnMapPositionX, 0, 2);
   mapBoundaryLayout->addWidget(m_wgtLblMapPositionY, 0, 3);
   mapBoundaryLayout->addWidget(m_wgtSpnMapPositionY, 0, 4);
   mapBoundaryLayout->addWidget(m_wgtLblMapSize, 1, 0);
   mapBoundaryLayout->addWidget(m_wgtLblMapSizeW, 1, 1);
   mapBoundaryLayout->addWidget(m_wgtSpnMapSizeW, 1, 2);
   mapBoundaryLayout->addWidget(m_wgtLblMapSizeH, 1, 3);
   mapBoundaryLayout->addWidget(m_wgtSpnMapSizeH, 1, 4);
   m_wgtGrpMapBoundary->setLayout(mapBoundaryLayout);

   QGridLayout* mapSegmentsLayout = new QGridLayout;
   mapSegmentsLayout->addWidget(m_wgtLblMapSegmentsQuantity, 0, 0);
   mapSegmentsLayout->addWidget(m_wgtLblMapSegmentsQuantityW, 0, 1);
   mapSegmentsLayout->addWidget(m_wgtSpnMapSegmentsQuantityW, 0, 2);
   mapSegmentsLayout->addWidget(m_wgtLblMapSegmentsQuantityH, 0, 3);
   mapSegmentsLayout->addWidget(m_wgtSpnMapSegmentsQuantityH, 0, 4);
   mapSegmentsLayout->addWidget(m_wgtLblMapSegmentsSize, 1, 0);
   mapSegmentsLayout->addWidget(m_wgtLblMapSegmentsSizeW, 1, 1);
   mapSegmentsLayout->addWidget(m_wgtSpnMapSegmentsSizeW, 1, 2);
   mapSegmentsLayout->addWidget(m_wgtLblMapSegmentsSizeH, 1, 3);
   mapSegmentsLayout->addWidget(m_wgtSpnMapSegmentsSizeH, 1, 4);
   m_wgtGrpMapSegments->setLayout(mapSegmentsLayout);

   QVBoxLayout* csLayout = new QVBoxLayout;
   csLayout->addWidget(m_wgtBtnCSSelect);
   m_wgtGrpCustomSettings->setLayout(csLayout);

   connect(m_wgtSpnMapPositionX, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapPositionY, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSizeW, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSizeH, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSegmentsQuantityW, SIGNAL(valueChanged(int)), this, SLOT(onChange(int)));
   connect(m_wgtSpnMapSegmentsQuantityH, SIGNAL(valueChanged(int)), this, SLOT(onChange(int)));
   connect(m_wgtSpnMapSegmentsSizeW, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSegmentsSizeH, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtBtnCSSelect, SIGNAL(released()), this, SLOT(onBtnCSSelectClick()));

   m_wgtSpnMapSegmentsQuantityW->setMinimum(1);
   m_wgtSpnMapSegmentsQuantityH->setMinimum(1);
}

//===========================================
// WgtMapSettings::onBtnCSSelectClick
//===========================================
void WgtMapSettings::onBtnCSSelectClick() {
   emit customSettingsSelected();
}

//===========================================
// WgtMapSettings::onChange
//===========================================
void WgtMapSettings::onChange(int) {
   emit changed();
}

//===========================================
// WgtMapSettings::onChange
//===========================================
void WgtMapSettings::onChange(double) {
   emit changed();
}

//===========================================
// WgtMapSettings::update
//===========================================
void WgtMapSettings::update() {
   m_wgtSpnMapPositionX->blockSignals(true);
   m_wgtSpnMapPositionY->blockSignals(true);
   m_wgtSpnMapSizeW->blockSignals(true);
   m_wgtSpnMapSizeH->blockSignals(true);
   m_wgtSpnMapSegmentsQuantityW->blockSignals(true);
   m_wgtSpnMapSegmentsQuantityH->blockSignals(true);
   m_wgtSpnMapSegmentsSizeW->blockSignals(true);
   m_wgtSpnMapSegmentsSizeH->blockSignals(true);

   m_wgtSpnMapPositionX->setValue(m_mapSettings.boundary.getPosition().x);
   m_wgtSpnMapPositionY->setValue(m_mapSettings.boundary.getPosition().y);
   m_wgtSpnMapSizeW->setValue(m_mapSettings.boundary.getSize().x);
   m_wgtSpnMapSizeH->setValue(m_mapSettings.boundary.getSize().y);
   m_wgtSpnMapSegmentsQuantityW->setValue(m_mapSettings.numSegments.x);
   m_wgtSpnMapSegmentsQuantityH->setValue(m_mapSettings.numSegments.y);
   m_wgtSpnMapSegmentsSizeW->setValue(m_mapSettings.segmentSize.x);
   m_wgtSpnMapSegmentsSizeH->setValue(m_mapSettings.segmentSize.y);

   m_wgtSpnMapPositionX->blockSignals(false);
   m_wgtSpnMapPositionY->blockSignals(false);
   m_wgtSpnMapSizeW->blockSignals(false);
   m_wgtSpnMapSizeH->blockSignals(false);
   m_wgtSpnMapSegmentsQuantityW->blockSignals(false);
   m_wgtSpnMapSegmentsQuantityH->blockSignals(false);
   m_wgtSpnMapSegmentsSizeW->blockSignals(false);
   m_wgtSpnMapSegmentsSizeH->blockSignals(false);

   emit changed();
}

//===========================================
// WgtMapSettings::mapSettings
//===========================================
MapSettings& WgtMapSettings::mapSettings() {
   m_mapSettings.boundary.setPosition(m_wgtSpnMapPositionX->value(), m_wgtSpnMapPositionY->value());
   m_mapSettings.boundary.setSize(m_wgtSpnMapSizeW->value(), m_wgtSpnMapSizeH->value());
   m_mapSettings.numSegments.x = m_wgtSpnMapSegmentsQuantityW->value();
   m_mapSettings.numSegments.y = m_wgtSpnMapSegmentsQuantityH->value();
   m_mapSettings.segmentSize.x = m_wgtSpnMapSegmentsSizeW->value();
   m_mapSettings.segmentSize.y = m_wgtSpnMapSegmentsSizeH->value();

   m_mapSettings.segmentsPath = "0"; // TODO
   m_mapSettings.filePath = "map0.xml"; // TODO

   return m_mapSettings;
}
