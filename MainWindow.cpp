#include <iostream>
#include <fstream>
#include <QApplication>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include <QTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QTabWidget>
#include <QGraphicsView>
#include <QToolBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTreeWidget>
#include <QCheckBox>
#include <QFileDialog>
#include "MainWindow.hpp"
#include "WgtXmlTreeView.hpp"
#include "WgtMapSettings.hpp"
#include "Common.hpp"
#include "Importer.hpp"
#include "Exporter.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MainWindow::MainWindow
//===========================================
MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent) {

   resize(600, 400);
   setWindowTitle("Dodge :: Entity Placement Tool");

   m_actOpen = new QAction("Open", this);
   m_actSave = new QAction("Save", this);
   m_actSaveAs = new QAction("Save As", this);
   m_actImport = new QAction("Import", this);
   m_actExport = new QAction("Export", this);
   m_actQuit = new QAction("Quit", this);

   m_mnuFile = menuBar()->addMenu("File");
   m_mnuFile->addAction(m_actOpen);
   m_mnuFile->addAction(m_actSave);
   m_mnuFile->addAction(m_actSaveAs);
   m_mnuFile->addAction(m_actImport);
   m_mnuFile->addAction(m_actExport);
   m_mnuFile->addAction(m_actQuit);

   m_wgtCentral = new QWidget(this);
   m_wgtLeftColumnTabs = new QTabWidget(m_wgtCentral);
   m_wgtToolsTab = new QWidget(m_wgtLeftColumnTabs);
   m_wgtTools = new QToolBox(m_wgtToolsTab);
   m_wgtCboPrototypes = new QComboBox(m_wgtToolsTab);
   m_wgtCentralColumnTabs = new QTabWidget(m_wgtCentral);
   m_wgtDrawScreenTab = new QTabWidget(m_wgtCentralColumnTabs);
   m_wgtDrawScreen = new QGraphicsView(m_wgtDrawScreenTab);
   m_wgtXmlEditTab = new QWidget(m_wgtCentralColumnTabs);
   m_wgtXmlEdit = new QTextEdit(m_wgtXmlEditTab);
   m_wgtXmlApply = new QPushButton("Apply", m_wgtXmlEditTab);
   m_wgtRightColumnTabs = new QTabWidget(m_wgtCentral);
   m_wgtXmlTreeTab = new QWidget(m_wgtRightColumnTabs);
   m_wgtXmlTree = new WgtXmlTreeView(m_wgtXmlTreeTab);
   m_wgtChkPrototype = new QCheckBox("prototype", m_wgtXmlTreeTab);
   m_wgtGrpSegment = new QGroupBox("Segment", m_wgtXmlTreeTab);
   m_wgtLblSegmentX = new QLabel("X", m_wgtGrpSegment);
   m_wgtLblSegmentY = new QLabel("Y", m_wgtGrpSegment);
   m_wgtSpnSegmentX = new QSpinBox(m_wgtGrpSegment);
   m_wgtSpnSegmentY = new QSpinBox(m_wgtGrpSegment);
   m_wgtChkGlobal = new QCheckBox("global", m_wgtGrpSegment);
   m_wgtBtnInferSegment = new QPushButton("Infer from position", m_wgtXmlTreeTab);
   m_wgtObjectsTab = new QWidget(m_wgtRightColumnTabs);
   m_wgtTreAssets = new QTreeWidget(m_wgtObjectsTab);
   m_wgtGrpAssets = new QGroupBox("New", m_wgtObjectsTab);
   m_wgtTxtNewAsset = new QLineEdit(m_wgtGrpAssets);
   m_wgtChkNewIsPrototype = new QCheckBox("prototype", m_wgtGrpAssets);
   m_wgtBtnNewAsset = new QPushButton("Add", m_wgtGrpAssets);
   m_wgtMapSettingsTab = new WgtMapSettings(m_wgtRightColumnTabs);


   // LEFT COLUMN

   m_wgtLeftColumnTabs->addTab(m_wgtToolsTab, "Tools");

   QVBoxLayout* toolsTabLayout = new QVBoxLayout;
   toolsTabLayout->addWidget(m_wgtCboPrototypes);
   m_wgtToolsTab->setLayout(toolsTabLayout);


   // CENTRAL COLUMN

   m_wgtCentralColumnTabs->addTab(m_wgtDrawScreenTab, "Viewer");
   m_wgtCentralColumnTabs->addTab(m_wgtXmlEditTab, "XML Code");

   QVBoxLayout* drawScreenTabLayout = new QVBoxLayout;
   drawScreenTabLayout->addWidget(m_wgtDrawScreen);
   m_wgtDrawScreenTab->setLayout(drawScreenTabLayout);

   QVBoxLayout* xmlEditTabLayout = new QVBoxLayout;
   xmlEditTabLayout->addWidget(m_wgtXmlEdit);
   xmlEditTabLayout->addWidget(m_wgtXmlApply);
   m_wgtXmlEditTab->setLayout(xmlEditTabLayout);

   m_wgtXmlApply->setDisabled(true);


   // RIGHT COLUMN

   m_wgtRightColumnTabs->addTab(m_wgtXmlTreeTab, "Properties");
   m_wgtRightColumnTabs->addTab(m_wgtObjectsTab, "Objects");
   m_wgtRightColumnTabs->addTab(m_wgtMapSettingsTab, "Map Settings");

   QFont font;
   font.setFamily("Courier");
   font.setStyleHint(QFont::Monospace);
   font.setFixedPitch(true);
   font.setPointSize(10);

   m_wgtXmlEdit->setFont(font);

   const int tabStop = 2;
   QFontMetrics metrics(font);
   m_wgtXmlEdit->setTabStopWidth(tabStop * metrics.width(' '));

   QVBoxLayout* xmlTreeTabLayout = new QVBoxLayout;
   xmlTreeTabLayout->addWidget(m_wgtXmlTree);
   xmlTreeTabLayout->addWidget(m_wgtChkPrototype);
   xmlTreeTabLayout->addWidget(m_wgtGrpSegment);
   m_wgtXmlTreeTab->setLayout(xmlTreeTabLayout);

   m_wgtSpnSegmentX->setMaximum(0);
   m_wgtSpnSegmentY->setMaximum(0);

   QGridLayout* segmentsLayout = new QGridLayout;
   segmentsLayout->addWidget(m_wgtLblSegmentX, 0, 0);
   segmentsLayout->addWidget(m_wgtSpnSegmentX, 0, 1);
   segmentsLayout->addWidget(m_wgtLblSegmentY, 0, 2);
   segmentsLayout->addWidget(m_wgtSpnSegmentY, 0, 3);
   segmentsLayout->addWidget(m_wgtChkGlobal, 1, 0, 1, 2);
   segmentsLayout->addWidget(m_wgtBtnInferSegment, 1, 2, 1, 2);
   m_wgtGrpSegment->setLayout(segmentsLayout);

   QVBoxLayout* objectsTabLayout = new QVBoxLayout;
   objectsTabLayout->addWidget(m_wgtTreAssets);
   objectsTabLayout->addWidget(m_wgtGrpAssets);
   m_wgtObjectsTab->setLayout(objectsTabLayout);

   QGridLayout* newAssetLayout = new QGridLayout;
   newAssetLayout->addWidget(m_wgtTxtNewAsset, 1, 1, 1, 2);
   newAssetLayout->addWidget(m_wgtChkNewIsPrototype, 2, 1, 1, 2);
   newAssetLayout->addWidget(m_wgtBtnNewAsset, 3, 1, 1, 1);
   m_wgtGrpAssets->setLayout(newAssetLayout);


   QHBoxLayout* mainLayout = new QHBoxLayout;

   setCentralWidget(m_wgtCentral);
   m_wgtCentral->setLayout(mainLayout);

   mainLayout->addWidget(m_wgtLeftColumnTabs, 1);
   mainLayout->addWidget(m_wgtCentralColumnTabs, 6);
   mainLayout->addWidget(m_wgtRightColumnTabs, 3);

   connect(m_actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
   connect(m_actOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
   connect(m_actSave, SIGNAL(triggered()), this, SLOT(onSave()));
   connect(m_actSaveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
   connect(m_actImport, SIGNAL(triggered()), this, SLOT(onImport()));
   connect(m_actExport, SIGNAL(triggered()), this, SLOT(onExport()));
   connect(m_wgtXmlApply, SIGNAL(released()), this, SLOT(btnApplyClick()));
   connect(m_wgtBtnNewAsset, SIGNAL(released()), this, SLOT(btnNewAssetClick()));
   connect(m_wgtXmlTree, SIGNAL(onUpdate()), this, SLOT(xmlTreeUpdated()));
   connect(m_wgtCboPrototypes, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onPrototypeSelection(const QString&)));
   connect(m_wgtTreAssets, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onAssetSelection(QTreeWidgetItem*, int)));
   connect(m_wgtChkPrototype, SIGNAL(stateChanged(int)), this, SLOT(onChkPrototypeChanged(int)));
   connect(m_wgtChkGlobal, SIGNAL(stateChanged(int)), this, SLOT(onChkGlobalChanged(int)));
   connect(m_wgtSpnSegmentX, SIGNAL(valueChanged(int)), this, SLOT(onSpnSegmentXChanged(int)));
   connect(m_wgtSpnSegmentY, SIGNAL(valueChanged(int)), this, SLOT(onSpnSegmentYChanged(int)));
   connect(m_wgtMapSettingsTab, SIGNAL(changed()), this, SLOT(onMapSettingsChange()));
   connect(m_wgtTreAssets, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onObjectNameChange(QTreeWidgetItem*, int)));
}

//===========================================
// MainWindow::onOpen
//===========================================
void MainWindow::onOpen() {
   QString qPath = QFileDialog::getOpenFileName(this, "Choose a file to open", "/home", "EPT files (*.ept)");
   string path(qPath.toLocal8Bit().data());

   int i = static_cast<int>(path.length()) - 1;
   for (; i >= 0; --i) {
      if (path[i] == '/' || path[i] == '\'') break;
   }

   m_objects.clear();

   m_rootPath = path.substr(0, i);
   m_projFilePath = path.substr(i + 1, path.length() - i - 1);
   m_assetsPath = "assets";

   m_importer = unique_ptr<Importer>(new Importer(m_rootPath + "/" + m_assetsPath));
   m_importer->import(m_wgtMapSettingsTab->mapSettings(), m_objects);

   loadProjectFile();

   m_wgtMapSettingsTab->update();
   updateAssetList("");
   updatePrototypesCombo();
}

//===========================================
// MainWindow::loadProjectFile
//===========================================
void MainWindow::loadProjectFile() {
   XmlDocument xml;
   try {
      xml.parse(m_rootPath + "/" + m_projFilePath);
   }
   catch (XmlException& e) {
      EXCEPTION("Error loading project file '" << m_rootPath + "/" + m_projFilePath << "'; " << e.what());
   }

   XmlNode settings = xml.firstNode();
   XML_NODE_CHECK(settings, settings);

   XmlNode assets = settings.nextSibling();
   XML_NODE_CHECK(assets, assets);

   XmlNode asset = assets.firstChild();
   while (!asset.isNull()) {
      XmlAttribute attr = asset.firstAttribute();
      XML_ATTR_CHECK(attr, id);

      long id = attr.getLong();
      shared_ptr<EptObject> obj = m_objects.get(id).lock();
      if (!obj) {
         // TODO: Error!
         continue;
      }

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, type);

      EptObject::type_t type;
      string strType = attr.getString();

      if (strType.compare("instance") == 0)
         type = EptObject::INSTANCE;
      else if (strType.compare("prototype") == 0)
         type = EptObject::PROTOTYPE;
      else {
         // TODO Error!
         continue;
      }

      m_objects.changeType(id, type);

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, name);

      string strName = attr.getString();
      QString qName(strName.data());

      m_objects.changeName(id, qName);

      asset = asset.nextSibling();
   }
}

//===========================================
// MainWindow::writeProjectFile
//===========================================
void MainWindow::writeProjectFile() {
   XmlDocument xml;

   XmlNode settings = xml.addNode("settings");
   XmlNode assets = xml.addNode("assets");

   for (auto i = m_objects.begin(); i != m_objects.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      XmlNode asset = assets.addNode("asset");

      stringstream ss;
      ss << obj->id();

      asset.addAttribute("id", ss.str());

      switch (obj->type()) {
         case EptObject::INSTANCE:
            asset.addAttribute("type", "instance");
         break;
         case EptObject::PROTOTYPE:
            asset.addAttribute("type", "prototype");
         break;
         default: assert(false);
      }

      asset.addAttribute("name", obj->name().toLocal8Bit().data());
   }

   ofstream fout(m_projFilePath);
   if (!fout.good()) {
      fout.close();
      EXCEPTION("Error writing project file '" << m_projFilePath << "'");
   }

   xml.print(fout);

   fout.close();
}

//===========================================
// MainWindow::onSave
//===========================================
void MainWindow::onSave() {
   if (m_projFilePath.length() == 0)
      onSaveAs();

   writeProjectFile();
}

//===========================================
// MainWindow::onSaveAs
//===========================================
void MainWindow::onSaveAs() {
   QString path = QFileDialog::getExistingDirectory(this, "Choose a directory", "/home", QFileDialog::ShowDirsOnly);
   m_rootPath = string(path.toLocal8Bit().data());

   m_projFilePath = "project.ept";
   m_assetsPath = "assets";

   writeProjectFile();
}

//===========================================
// MainWindow::copyAssets
//===========================================
void MainWindow::copyAssets() {
   createDir(m_rootPath + "/" + m_assetsPath);

   // TODO
}

//===========================================
// MainWindow::onImport
//===========================================
void MainWindow::onImport() {
   string importPath("./import"); // TODO

   m_importer = unique_ptr<Importer>(new Importer(importPath));
   m_importer->import(m_wgtMapSettingsTab->mapSettings(), m_objects);

   m_wgtMapSettingsTab->update();
   updateAssetList("");
   updatePrototypesCombo();
}

//===========================================
// MainWindow::onMapSettingsChange
//===========================================
void MainWindow::onMapSettingsChange() {
   const MapSettings& settings = m_wgtMapSettingsTab->mapSettings();

   m_wgtSpnSegmentX->setMaximum(settings.numSegments.x - 1);
   m_wgtSpnSegmentY->setMaximum(settings.numSegments.y - 1);
}

//===========================================
// MainWindow::onExport
//===========================================
void MainWindow::onExport() {
   m_exporter = unique_ptr<Exporter>(new Exporter("./export")); // TODO
   m_exporter->export_(m_wgtMapSettingsTab->mapSettings(), m_objects);
}

//===========================================
// MainWindow::onSpnSegmentXChanged
//===========================================
void MainWindow::onSpnSegmentXChanged(int value) {
   auto obj = m_current.lock();
   if (!obj) return;

   int y = m_wgtSpnSegmentY->value();

   m_objects.move(obj->name(), value, y);
}

//===========================================
// MainWindow::onSpnSegmentYChanged
//===========================================
void MainWindow::onSpnSegmentYChanged(int value) {
   auto obj = m_current.lock();
   if (!obj) return;

   int x = m_wgtSpnSegmentX->value();

   m_objects.move(obj->name(), x, value);
}

//===========================================
// MainWindow::onChkGlobalChanged
//===========================================
void MainWindow::onChkGlobalChanged(int state) {
   auto obj = m_current.lock();

   switch (state) {
      case Qt::Checked: {
         m_wgtSpnSegmentX->setDisabled(true);
         m_wgtSpnSegmentY->setDisabled(true);
         m_wgtBtnInferSegment->setDisabled(true);

         if (obj) m_objects.move(obj->name(), -1, -1);
      }
      break;
      case Qt::Unchecked: {
         m_wgtSpnSegmentX->setDisabled(false);
         m_wgtSpnSegmentY->setDisabled(false);
         m_wgtBtnInferSegment->setDisabled(false);

         int x = m_wgtSpnSegmentX->value();
         int y = m_wgtSpnSegmentY->value();

         if (obj) m_objects.move(obj->name(), x, y);
      }
      break;
   }
}

//===========================================
// MainWindow::updatePrototypesCombo
//===========================================
void MainWindow::updatePrototypesCombo() {
   m_wgtCboPrototypes->clear();

   auto objs = m_objects.get(EptObject::PROTOTYPE);

   for (auto i = objs.begin(); i != objs.end(); ++i) {
      auto obj = i->lock();
      assert(obj);

      m_wgtCboPrototypes->addItem(obj->name());
   }
}

//===========================================
// MainWindow::onChkPrototypeChanged
//===========================================
void MainWindow::onChkPrototypeChanged(int state) {
   if (!m_current.lock()) return;

   shared_ptr<EptObject> obj = m_current.lock();
   QString name = obj->name();

   int idx = m_wgtCboPrototypes->findText(name);
   if (idx != -1) {
      m_wgtCboPrototypes->removeItem(idx);
   }

   switch (state) {
      case Qt::Checked: {
         m_objects.changeType(obj->id(), EptObject::PROTOTYPE);
         m_wgtCboPrototypes->addItem(name);
      }
      break;
      case Qt::Unchecked: {
         m_objects.changeType(obj->id(), EptObject::INSTANCE);
      }
      break;
      default: assert(false);
   }
}

//===========================================
// MainWindow::onPrototypeSelection
//===========================================
void MainWindow::onPrototypeSelection(const QString& name) {
   // TODO
}

//===========================================
// MainWindow::onObjectNameChange
//===========================================
void MainWindow::onObjectNameChange(QTreeWidgetItem* item, int) {
   auto current = m_current.lock();
   assert(current);

   QString from = current->name();
   QString to = item->text(0);

   if (m_objects.contains(to)) {
      alert("Asset already with that name");

      m_wgtTreAssets->blockSignals(true);
      item->setText(0, from);
      m_wgtTreAssets->blockSignals(false);

      return;
   }

   m_objects.changeName(current->id(), to);

   updateAssetList(to);
}

//===========================================
// MainWindow::onAssetSelection
//===========================================
void MainWindow::onAssetSelection(QTreeWidgetItem* item, int) {
   m_wgtXmlApply->setDisabled(false);

   QString name = item->text(0);

   auto obj = m_objects.get(name);
   m_current = obj;

   shared_ptr<EptObject> pObj = m_current.lock();
   assert(pObj);

   string text;
   auto xml = pObj->xml().lock();
   xml->print(text);

   m_wgtXmlTree->update(xml);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));

   bool isProto = pObj->type() == EptObject::PROTOTYPE;
   m_wgtChkPrototype->setCheckState(isProto ? Qt::Checked : Qt::Unchecked);

   const Vec2i& seg = pObj->segment();
   if (seg == Vec2i(-1, -1)) {
      m_wgtChkGlobal->setCheckState(Qt::Checked);
   }
   else {
      m_wgtChkGlobal->setCheckState(Qt::Unchecked);
      m_wgtSpnSegmentX->setValue(seg.x);
      m_wgtSpnSegmentY->setValue(seg.y);
   }
}

//===========================================
// MainWindow::xmlTreeUpdated
//===========================================
void MainWindow::xmlTreeUpdated() {
   auto obj = m_current.lock();
   assert(obj);

   // In case object's dependencies have changed
   obj->computeDependencies();

   string text;
   obj->xml().lock()->print(text);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));

   updateAssetList(obj->name());
}

//===========================================
// MainWindow::btnNewAssetClick
//===========================================
void MainWindow::btnNewAssetClick() {
   QString str = m_wgtTxtNewAsset->text();

   if (m_objects.contains(str)) {
      alert("Asset with that name already exists");
      return;
   }

   m_wgtTxtNewAsset->clear();

   bool isProto = m_wgtChkNewIsPrototype->isChecked();

   shared_ptr<EptObject> ent(new EptObject(str, isProto ? EptObject::PROTOTYPE : EptObject::INSTANCE));

   m_objects.insert(ent);
   m_current = ent;

   updateAssetList(str);

   m_wgtChkNewIsPrototype->setCheckState(Qt::Unchecked);
}

//===========================================
// MainWindow::updateAssetList
//===========================================
void MainWindow::updateAssetList(const QString& select) {
   m_wgtTreAssets->blockSignals(true);
   m_wgtTreAssets->clear();

   for (ObjectContainer::iterator i = m_objects.begin(); i != m_objects.end(); ++i) {
      auto obj = i->lock();
      const QString& name = obj->name();

      QTreeWidgetItem* item = new QTreeWidgetItem(m_wgtTreAssets, QStringList(name));
      item->setFlags(item->flags() | Qt::ItemIsEditable);

      m_wgtTreAssets->addTopLevelItem(item);
      updateAssetList_r(item, obj);
   }

   QList<QTreeWidgetItem*> items = m_wgtTreAssets->findItems(select, 0, 0);

   if (items.size() > 0) {
      assert(items.size() == 1);

      m_wgtTreAssets->setCurrentItem(items[0]);
      onAssetSelection(items[0], 0);
   }

   m_wgtTreAssets->blockSignals(false);
}

//===========================================
// MainWindow::updateAssetList_r
//===========================================
void MainWindow::updateAssetList_r(QTreeWidgetItem* parent, weak_ptr<EptObject> obj) {
   auto pObj = obj.lock();
   assert(pObj);

   const set<long>& deps = pObj->dependencies();
   for (auto i = deps.begin(); i != deps.end(); ++i) {
      auto objDep = m_objects.get(*i).lock();

      // Dependency doesn't exist (yet).
      if (!objDep) continue;

      // TODO
      // Object cannot link to itself
      if (objDep == pObj) continue;

      QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList(objDep->name()));
      item->setFlags(item->flags() | Qt::ItemIsEditable);

      parent->addChild(item);

      updateAssetList_r(item, objDep);
   }
}

//===========================================
// MainWindow::btnApplyClick
//===========================================
void MainWindow::btnApplyClick() {
   auto obj = m_current.lock();
   assert(obj);

   QString str = m_wgtXmlEdit->toPlainText();

   XmlParseResult res;
   obj->parseXml(str, res);

   if (res.result == XmlParseResult::FAILURE) {
      alert_std("Error in xml: " + res.msg);
      return;
   }

   auto doc = obj->xml().lock();
   assert(doc);

   m_wgtXmlTree->update(doc);
   updateAssetList(obj->name());
}

//===========================================
// MainWindow::~MainWindow
//===========================================
MainWindow::~MainWindow() {
   delete m_mnuFile;
   delete m_actSave;
   delete m_actSaveAs;
   delete m_actExport;
   delete m_actQuit;

   delete m_wgtCentral;
}
