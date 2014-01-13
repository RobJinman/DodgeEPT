#include <sstream>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include "WgtMapSettings.hpp"
#include "EptObject.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MapSettings::loadSettingsFromXml
//===========================================
void MapSettings::loadSettingsFromXml(XmlNode node) {
   // <settings>
   //   <boundary>
   //     <Range>
   //       <pos>
   //         <Vec2f/>
   //       </pos>
   //       <size>
   //         <Vec2f/>
   //       </size>
   //     </Range>
   //   </boundary>
   //   <numSegments>
   //     <Vec2i/>
   //   </numSegments>
   //   <segmentSize>
   //     <Vec2f/>
   //   </segmentSize>
   //   <segmentsDir/>
   // </settings>

   XML_NODE_CHECK(node, settings);

   node = node.firstChild();
   XML_NODE_CHECK(node, boundary);

   boundary = Range(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(node, numSegments);
   numSegments = Vec2i(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(node, segmentSize);
   segmentSize = Vec2f(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(node, segmentsDir);
   segmentsDir = node.getString();
}

//===========================================
// MapSettings::loadIncludesFromXml
//===========================================
void MapSettings::loadIncludesFromXml(XmlNode node) {
   includes.clear();

   XML_NODE_CHECK(node, using);

   node = node.firstChild();
   while (!node.isNull()) {
      XML_NODE_CHECK(node, file);

      includes.push_back(QString(node.getString().data()));

      node = node.nextSibling();
   }
}

//===========================================
// MapSettings::toXml
//===========================================
XmlDocument MapSettings::toXml() const {
   XmlDocument doc;

   stringstream ss;

   XmlNode settings = doc.addNode("settings");
      XmlNode boundary = settings.addNode("boundary");
         XmlNode range = boundary.addNode("Range");
            XmlNode pos = range.addNode("pos");
               XmlNode brp_vec2f = pos.addNode("Vec2f");
                  ss.str("");
                  ss << this->boundary.getPosition().x;

                  brp_vec2f.addAttribute("x", ss.str().data());

                  ss.str("");
                  ss << this->boundary.getPosition().y;

                  brp_vec2f.addAttribute("y", ss.str().data());
            XmlNode size = range.addNode("size");
               XmlNode brs_vec2f = size.addNode("Vec2f");
                  ss.str("");
                  ss << this->boundary.getSize().x;

                  brs_vec2f.addAttribute("x", ss.str().data());

                  ss.str("");
                  ss << this->boundary.getSize().y;

                  brs_vec2f.addAttribute("y", ss.str().data());
      XmlNode numSegments = settings.addNode("numSegments");
         XmlNode ns_vec2i = numSegments.addNode("Vec2i");
            ss.str("");
            ss << this->numSegments.x;

            ns_vec2i.addAttribute("x", ss.str().data());

            ss.str("");
            ss << this->numSegments.y;

            ns_vec2i.addAttribute("y", ss.str().data());
      XmlNode segmentSize = settings.addNode("segmentSize");
         XmlNode ss_vec2f = segmentSize.addNode("Vec2f");
            ss.str("");
            ss << this->segmentSize.x;

            ss_vec2f.addAttribute("x", ss.str().data());

            ss.str("");
            ss << this->segmentSize.y;

            ss_vec2f.addAttribute("y", ss.str().data());
      XmlNode segmentsDir = settings.addNode("segmentsDir");
         segmentsDir.setValue("data/xml/0"); // TODO
   XmlNode customSettings = doc.addNode("customSettings");
   XmlNode using_ = doc.addNode("using");
      for (unsigned int i = 0; i < includes.size(); ++i) {
         XmlNode file = using_.addNode("file");
         file.setValue(includes[i].toLocal8Bit().data());
      }
   XmlNode assets = doc.addNode("assets");
      for (unsigned int i = 0; i < this->assets.size(); ++i) {
         auto xml = this->assets[i].lock();
         assert(xml);

         assets.addNode(xml->firstNode());
      }

   return doc;
}

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

   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget(m_wgtGrpMapBoundary);
   layout->addWidget(m_wgtGrpMapSegments);
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

   connect(m_wgtSpnMapPositionX, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapPositionY, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSizeW, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSizeH, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSegmentsQuantityW, SIGNAL(valueChanged(int)), this, SLOT(onChange(int)));
   connect(m_wgtSpnMapSegmentsQuantityH, SIGNAL(valueChanged(int)), this, SLOT(onChange(int)));
   connect(m_wgtSpnMapSegmentsSizeW, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));
   connect(m_wgtSpnMapSegmentsSizeH, SIGNAL(valueChanged(double)), this, SLOT(onChange(double)));

   m_wgtSpnMapSegmentsQuantityW->setMinimum(1);
   m_wgtSpnMapSegmentsQuantityH->setMinimum(1);
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
// WgtMapSettings::addTopLevelAsset
//===========================================
void WgtMapSettings::addTopLevelAsset(const EptObject& obj) {
   m_mapSettings.assets.push_back(obj.xml());
}

//===========================================
// WgtMapSettings::addFileDependency
//===========================================
void WgtMapSettings::addFileDependency(const QString& path) {
   m_mapSettings.includes.push_back(path);
}

//===========================================
// WgtMapSettings::loadFromXml
//===========================================
void WgtMapSettings::loadFromXml(weak_ptr<XmlDocument> doc, XmlNode& assets) {
   // <settings/>
   // <customSettings/>
   // <using/>
   // <assets/>

   auto pDoc = doc.lock();
   assert(pDoc);

   XmlNode node = pDoc->firstNode();
   node = node.nextSibling();
   XML_NODE_CHECK(node, MAPFILE);

   node = node.firstChild();
   m_mapSettings.loadSettingsFromXml(node);

   node = node.nextSibling(); // customSettings
   node = node.nextSibling();
   m_mapSettings.loadIncludesFromXml(node);

   node = node.nextSibling();
   XML_NODE_CHECK(node, assets);

   assets = node;

   m_wgtSpnMapPositionX->setValue(m_mapSettings.boundary.getPosition().x);
   m_wgtSpnMapPositionY->setValue(m_mapSettings.boundary.getPosition().y);
   m_wgtSpnMapSizeW->setValue(m_mapSettings.boundary.getSize().x);
   m_wgtSpnMapSizeH->setValue(m_mapSettings.boundary.getSize().y);
   m_wgtSpnMapSegmentsQuantityW->setValue(m_mapSettings.numSegments.x);
   m_wgtSpnMapSegmentsQuantityH->setValue(m_mapSettings.numSegments.y);
   m_wgtSpnMapSegmentsSizeW->setValue(m_mapSettings.segmentSize.x);
   m_wgtSpnMapSegmentsSizeH->setValue(m_mapSettings.segmentSize.y);
}

//===========================================
// WgtMapSettings::mapSettings
//===========================================
const MapSettings& WgtMapSettings::mapSettings() const {
   m_mapSettings.boundary.setPosition(m_wgtSpnMapPositionX->value(), m_wgtSpnMapPositionY->value());
   m_mapSettings.boundary.setSize(m_wgtSpnMapSizeW->value(), m_wgtSpnMapSizeH->value());
   m_mapSettings.numSegments.x = m_wgtSpnMapSegmentsQuantityW->value();
   m_mapSettings.numSegments.y = m_wgtSpnMapSegmentsQuantityH->value();
   m_mapSettings.segmentSize.x = m_wgtSpnMapSegmentsSizeW->value();
   m_mapSettings.segmentSize.y = m_wgtSpnMapSegmentsSizeH->value();

   m_mapSettings.segmentsDir = "0"; // TODO
   m_mapSettings.fileName = "map0.xml"; // TODO

   return m_mapSettings;
}
