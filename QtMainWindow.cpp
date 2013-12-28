#include <iostream>
#include <QPushButton>
#include <QAction>
#include <QTreeWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include "QtMainWindow.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// QtMainWindow::QtMainWindow
//===========================================
QtMainWindow::QtMainWindow(QWidget* parent)
   : QMainWindow(parent) {

   resize(600, 400);
   setWindowTitle("Dodge :: Entity Placement Tool");

   QHBoxLayout* hbox = new QHBoxLayout;

   m_wgtCentral = new QWidget(this);
   m_wgtXmlEdit = new QTextEdit(m_wgtCentral);
   m_wgtXmlTree = new QTreeWidget(m_wgtCentral);
   m_wgtButton = new QPushButton("Button", m_wgtCentral);

   setCentralWidget(m_wgtCentral);
   m_wgtCentral->setLayout(hbox);

   hbox->addWidget(m_wgtButton);
   hbox->addWidget(m_wgtXmlEdit);
   hbox->addWidget(m_wgtXmlTree);

   connect(m_wgtButton, SIGNAL(released()), this, SLOT(foo()));
}

//===========================================
// QtMainWindow::populateXmlTree
//===========================================
void QtMainWindow::populateXmlTree(XmlDocument& doc) {
   m_wgtXmlTree->clear();

   XmlNode node = doc.firstNode();

   QStringList list;
   list.push_back(QString(node.name().data()));

   int c = 0;
   XmlAttribute attr = node.firstAttribute();
   while (!attr.isNull()) {
      list.push_back(QString(attr.name().data()));

      ++c;
      attr = attr.nextAttribute();
   }

   QTreeWidgetItem* item = new QTreeWidgetItem(m_wgtXmlTree, list);
   int cols = populateXmlTree_r(item, node);

   if (cols > c) c = cols;

   m_wgtXmlTree->addTopLevelItem(item);
   m_wgtXmlTree->setColumnCount(c + 1);
}

//===========================================
// QtMainWindow::populateXmlTree_r
//===========================================
int QtMainWindow::populateXmlTree_r(QTreeWidgetItem* parent, XmlNode& node) {
   XmlNode ch = node.firstChild();

   int maxCols = 0;

   while (!ch.isNull()) {
      QStringList list;
      list.push_back(QString(ch.name().data()));

      int c = 0;
      XmlAttribute attr = ch.firstAttribute();
      while (!attr.isNull()) {
         list.push_back(QString(attr.name().data()));

         ++c;
         attr = attr.nextAttribute();
      }

      QTreeWidgetItem* item = new QTreeWidgetItem(parent, list);

      int cols = populateXmlTree_r(item, ch);

      if (c > maxCols) maxCols = c;
      if (cols > maxCols) maxCols = cols;

      parent->addChild(item);

      ch = ch.nextSibling();
   }

   return maxCols;
}

//===========================================
// QtMainWindow::foo
//===========================================
void QtMainWindow::foo() {
   QString str = m_wgtXmlEdit->toPlainText();
   QByteArray bytes = str.toLocal8Bit();
   const char* data = bytes.data();
   int len = bytes.length();

   try {
      XmlDocument doc;
      doc.parse(data, len);

      populateXmlTree(doc);
   }
   catch (XmlException& e) {
      cout << e.what() << "\n";
   }
}

//===========================================
// QtMainWindow::~QtMainWindow
//===========================================
QtMainWindow::~QtMainWindow() {
   delete m_wgtCentral;
}
