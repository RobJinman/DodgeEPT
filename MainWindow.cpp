#include <iostream>
#include <QPushButton>
#include <QAction>
#include <QTextEdit>
#include <QHBoxLayout>
#include <dodge/xml/xml.hpp>
#include "MainWindow.hpp"
#include "XmlTreeView.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MainWindow::MainWindow
//===========================================
MainWindow::MainWindow(QWidget* parent)
   : QMainWindow(parent) {

   resize(600, 400);
   setWindowTitle("Dodge :: Entity Placement Tool");

   QHBoxLayout* hbox = new QHBoxLayout;

   m_wgtCentral = new QWidget(this);
   m_wgtXmlEdit = new QTextEdit(m_wgtCentral);
   m_wgtXmlTree = new XmlTreeView(m_wgtCentral);
   m_wgtButton = new QPushButton("Button", m_wgtCentral);

   QFont font;
   font.setFamily("Courier");
   font.setStyleHint(QFont::Monospace);
   font.setFixedPitch(true);
   font.setPointSize(10);

   m_wgtXmlEdit->setFont(font);

   const int tabStop = 2;

   QFontMetrics metrics(font);
   m_wgtXmlEdit->setTabStopWidth(tabStop * metrics.width(' '));

   setCentralWidget(m_wgtCentral);
   m_wgtCentral->setLayout(hbox);

   hbox->addWidget(m_wgtButton);
   hbox->addWidget(m_wgtXmlEdit);
   hbox->addWidget(m_wgtXmlTree);

   connect(m_wgtButton, SIGNAL(released()), this, SLOT(btnApplyClick()));
   connect(m_wgtXmlTree, SIGNAL(onUpdate(const std::string&)), this, SLOT(xmlTreeUpdated(const std::string&)));
}

//===========================================
// MainWindow::xmlTreeUpdated
//===========================================
void MainWindow::xmlTreeUpdated(const std::string& text) {
   m_wgtXmlEdit->setPlainText(QString(QByteArray(text.data(), text.length())));
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
      XmlDocument* doc = new XmlDocument;
      doc->parse(data, len);

      m_wgtXmlTree->update(unique_ptr<XmlDocument>(doc));
   }
   catch (XmlException& e) {
      cout << e.what() << "\n";
   }
}

//===========================================
// MainWindow::~MainWindow
//===========================================
MainWindow::~MainWindow() {
   delete m_wgtCentral;
}
