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
#include <QListWidget>
#include <QCheckBox>
#include "MainWindow.hpp"
#include "WgtXmlTreeView.hpp"
#include "WgtMapSettings.hpp"
#include "Common.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MainWindow::MainWindow
//===========================================
MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent) {

   m_root = "./export";

   resize(600, 400);
   setWindowTitle("Dodge :: Entity Placement Tool");

   m_actSave = new QAction("Save", this);
   m_actSaveAs = new QAction("Save As", this);
   m_actExport = new QAction("Export", this);
   m_actQuit = new QAction("Quit", this);

   m_mnuFile = menuBar()->addMenu("File");
   m_mnuFile->addAction(m_actSave);
   m_mnuFile->addAction(m_actSaveAs);
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
   m_wgtObjectsTab = new QWidget(m_wgtRightColumnTabs);
   m_wgtLstAssets = new QListWidget(m_wgtObjectsTab);
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
   m_wgtXmlTreeTab->setLayout(xmlTreeTabLayout);

   QVBoxLayout* objectsTabLayout = new QVBoxLayout;
   objectsTabLayout->addWidget(m_wgtLstAssets);
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
   connect(m_actExport, SIGNAL(triggered()), this, SLOT(onExport()));
   connect(m_wgtXmlApply, SIGNAL(released()), this, SLOT(btnApplyClick()));
   connect(m_wgtBtnNewAsset, SIGNAL(released()), this, SLOT(btnNewAssetClick()));
   connect(m_wgtXmlTree, SIGNAL(onUpdate()), this, SLOT(xmlTreeUpdated()));
   connect(m_wgtCboPrototypes, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onPrototypeSelection(const QString&)));
   connect(m_wgtLstAssets, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onAssetSelection(QListWidgetItem*)));
   connect(m_wgtChkPrototype, SIGNAL(stateChanged(int)), this, SLOT(onChkPrototypeChanged(int)));
}

//===========================================
// MainWindow::exportMapSettings
//===========================================
void MainWindow::exportMapSettings() {
   const MapSettings& settings = m_wgtMapSettingsTab->mapSettings();
   XmlDocument xml = settings.toXml();

   string path = m_root + "/" + settings.filePath;

   ofstream fout(path);
   if (!fout.good()) {
      fout.close();
      EXCEPTION("Error opening file '" << path << "'");
   }

   fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
   xml.print(fout);

   fout.close();
}

//===========================================
// MainWindow::exportPrototypes
//===========================================
void MainWindow::exportPrototypes() {
   const ObjectContainer::wkPtrSet_t& objs = m_objects.get(EptObject::PROTOTYPE);

   for (auto i = objs.begin(); i != objs.end(); ++i) {
      shared_ptr<EptObject> obj = i->lock();
      assert(obj);

      stringstream ss;
      ss << m_root << "/" << obj->name().toLocal8Bit().data() << ".xml";

      ofstream fout(ss.str());
      if (!fout.good()) {
         fout.close();
         cerr << "Error writing to file '" << ss.str() << "'\n";

         continue;
      }

      auto xml = obj->xml().lock();
      assert(xml);

      fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
      fout << "<ASSETFILE>\n";
      fout << "<assets>\n";
      xml->print(fout);
      fout << "</assets>\n";
      fout << "</ASSETFILE>\n";

      fout.close();
   }
}

//===========================================
// MainWindow::exportInstances
//===========================================
void MainWindow::exportInstances() {
   // TODO
}

//===========================================
// MainWindow::onExport
//===========================================
void MainWindow::onExport() {
   exportMapSettings();
   exportPrototypes();
   exportInstances();
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
         m_objects.changeType(name, EptObject::PROTOTYPE);
         m_wgtCboPrototypes->addItem(name);
      }
      break;
      case Qt::Unchecked: {
         m_objects.changeType(name, EptObject::INSTANCE);
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
// MainWindow::onAssetSelection
//===========================================
void MainWindow::onAssetSelection(QListWidgetItem* item) {
   m_wgtXmlApply->setDisabled(false);

   QString name = item->text();

   auto obj = m_objects.get(name);
   m_current = obj;

   shared_ptr<EptObject> pObj = m_current.lock();
   assert(pObj);

   string text;
   auto xml = pObj->xml().lock();
   xml->print(text);

   m_wgtXmlTree->update(xml);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));

   bool b = pObj->type() == EptObject::PROTOTYPE;
   m_wgtChkPrototype->setCheckState(b ? Qt::Checked : Qt::Unchecked);
}

//===========================================
// MainWindow::xmlTreeUpdated
//===========================================
void MainWindow::xmlTreeUpdated() {
   string text;
   m_current.lock()->xml().lock()->print(text);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));
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

   QListWidgetItem* item = new QListWidgetItem(str, m_wgtLstAssets);

   m_wgtLstAssets->addItem(item);
   m_wgtLstAssets->setCurrentItem(item);

   m_wgtChkNewIsPrototype->setCheckState(Qt::Unchecked);

   onAssetSelection(item);
}

//===========================================
// MainWindow::btnApplyClick
//===========================================
void MainWindow::btnApplyClick() {
   QString str = m_wgtXmlEdit->toPlainText();
   QByteArray bytes = str.toLocal8Bit();
   const char* data = bytes.data();
   int len = bytes.length();

   try {
      weak_ptr<XmlDocument> doc = m_current.lock()->xml();
      auto p = doc.lock();

      if (p) {
         p->parse(data, len);

         m_wgtXmlTree->update(doc);
      }
   }
   catch (XmlException& e) {
      cout << e.what() << "\n";
   }
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
