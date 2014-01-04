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

   m_root = ".";
   m_current = NULL;

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
   m_wgtCentralColumnTabs = new QTabWidget(m_wgtCentral);
   m_wgtDrawScreenTab = new QTabWidget(m_wgtCentralColumnTabs);
   m_wgtDrawScreen = new QGraphicsView(m_wgtDrawScreenTab);
   m_wgtXmlEditTab = new QWidget(m_wgtCentralColumnTabs);
   m_wgtXmlEdit = new QTextEdit(m_wgtXmlEditTab);
   m_wgtXmlApply = new QPushButton("Apply", m_wgtXmlEditTab);
   m_wgtRightColumnTabs = new QTabWidget(m_wgtCentral);
   m_wgtXmlTreeTab = new QWidget(m_wgtRightColumnTabs);
   m_wgtXmlTree = new WgtXmlTreeView(m_wgtXmlTreeTab);
   m_wgtObjectsTab = new QWidget(m_wgtRightColumnTabs);
   m_wgtGrpPrototypes = new QGroupBox("Prototypes", m_wgtObjectsTab);
   m_wgtCboPrototypes = new QComboBox(m_wgtGrpPrototypes);
   m_wgtTxtNewPrototype = new QLineEdit(m_wgtGrpPrototypes);
   m_wgtBtnNewPrototype = new QPushButton("Add", m_wgtGrpPrototypes);
   m_wgtGrpInstances = new QGroupBox("Instances", m_wgtObjectsTab);
   m_wgtCboInstances = new QComboBox(m_wgtGrpInstances);
   m_wgtTxtNewInstance = new QLineEdit(m_wgtGrpInstances);
   m_wgtBtnNewInstance = new QPushButton("Add", m_wgtGrpInstances);
   m_wgtMapSettingsTab = new WgtMapSettings(m_wgtRightColumnTabs);


   // LEFT COLUMN

   m_wgtLeftColumnTabs->addTab(m_wgtToolsTab, "Tools");


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
   m_wgtXmlTreeTab->setLayout(xmlTreeTabLayout);

   QVBoxLayout* objectsTabLayout = new QVBoxLayout;
   objectsTabLayout->addWidget(m_wgtGrpPrototypes);
   objectsTabLayout->addWidget(m_wgtGrpInstances);
   m_wgtObjectsTab->setLayout(objectsTabLayout);

   QGridLayout* newPrototypeLayout = new QGridLayout;
   newPrototypeLayout->addWidget(m_wgtCboPrototypes, 0, 0, 1, 2);
   newPrototypeLayout->addWidget(m_wgtTxtNewPrototype, 1, 1);
   newPrototypeLayout->addWidget(m_wgtBtnNewPrototype, 1, 2);
   m_wgtGrpPrototypes->setLayout(newPrototypeLayout);

   QGridLayout* newInstanceLayout = new QGridLayout;
   newInstanceLayout->addWidget(m_wgtCboInstances, 0, 0, 1, 2);
   newInstanceLayout->addWidget(m_wgtTxtNewInstance, 1, 1);
   newInstanceLayout->addWidget(m_wgtBtnNewInstance, 1, 2);
   m_wgtGrpInstances->setLayout(newInstanceLayout);


   QHBoxLayout* mainLayout = new QHBoxLayout;

   setCentralWidget(m_wgtCentral);
   m_wgtCentral->setLayout(mainLayout);

   mainLayout->addWidget(m_wgtLeftColumnTabs, 1);
   mainLayout->addWidget(m_wgtCentralColumnTabs, 6);
   mainLayout->addWidget(m_wgtRightColumnTabs, 3);

   connect(m_actQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
   connect(m_actExport, SIGNAL(triggered()), this, SLOT(onExport()));
   connect(m_wgtXmlApply, SIGNAL(released()), this, SLOT(btnApplyClick()));
   connect(m_wgtBtnNewPrototype, SIGNAL(released()), this, SLOT(btnNewPrototypeClick()));
   connect(m_wgtBtnNewInstance, SIGNAL(released()), this, SLOT(btnNewInstanceClick()));
   connect(m_wgtXmlTree, SIGNAL(onUpdate()), this, SLOT(xmlTreeUpdated()));
   connect(m_wgtCboPrototypes, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onPrototypeSelection(const QString&)));
   connect(m_wgtCboInstances, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onInstanceSelection(const QString&)));
}

//===========================================
// MainWindow::onExport
//===========================================
void MainWindow::onExport() {
   const MapSettings& settings = m_wgtMapSettingsTab->mapSettings();
   XmlDocument xml = settings.toXml();

   string path = m_root + "/" + settings.filePath;

   ofstream fout(path);
   if (!fout.good()) {
      fout.close();
      EXCEPTION("Error opening file '" << path << "'");
   }

   xml.print(fout);

   fout.close();
}

//===========================================
// MainWindow::onPrototypeSelection
//===========================================
void MainWindow::onPrototypeSelection(const QString& name) {
   m_wgtXmlApply->setDisabled(false);

   auto i = m_objects.find(name);
   assert(i != m_objects.end());

   m_current = i->second.get();

   auto p = m_current->xml().lock();
   string text;
   p->print(text);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));
}

//===========================================
// MainWindow::onInstanceSelection
//===========================================
void MainWindow::onInstanceSelection(const QString& name) {
   m_wgtXmlApply->setDisabled(false);

   auto i = m_objects.find(name);
   assert(i != m_objects.end());

   m_current = i->second.get();

   string text;
   m_current->xml().lock()->print(text);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));
}

//===========================================
// MainWindow::xmlTreeUpdated
//===========================================
void MainWindow::xmlTreeUpdated() {
   string text;
   m_current->xml().lock()->print(text);

   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));
}

//===========================================
// MainWindow::btnNewPrototypeClick
//===========================================
void MainWindow::btnNewPrototypeClick() {
   QString str = m_wgtTxtNewPrototype->text();

   auto i = m_objects.find(str);
   if (i != m_objects.end()) {
      // TODO
   }
   else {
      m_wgtTxtNewPrototype->clear();

      EptObject* ent = new EptObject(str, EptObject::PROTOTYPE);

      m_objects[str] = unique_ptr<EptObject>(ent);
      m_current = ent;

      m_wgtCboPrototypes->addItem(str);
      m_wgtCboPrototypes->setCurrentIndex(m_wgtCboPrototypes->count() - 1);
   }
}

//===========================================
// MainWindow::btnNewInstanceClick
//===========================================
void MainWindow::btnNewInstanceClick() {
   QString str = m_wgtTxtNewInstance->text();

   auto i = m_objects.find(str);
   if (i != m_objects.end()) {
      // TODO
   }
   else {
      m_wgtTxtNewInstance->clear();

      EptObject* ent = new EptObject(str, EptObject::INSTANCE);

      m_objects[str] = unique_ptr<EptObject>(ent);
      m_current = ent;

      m_wgtCboInstances->addItem(str);
      m_wgtCboInstances->setCurrentIndex(m_wgtCboInstances->count() - 1);
   }
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
      weak_ptr<XmlDocument> doc = m_current->xml();
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
