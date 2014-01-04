#include <QVBoxLayout>
#include "WgtXmlTreeView.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// WgtXmlTreeView::WgtXmlTreeView
//===========================================
WgtXmlTreeView::WgtXmlTreeView(QWidget* parent)
   : QWidget(parent),
     m_wgtTree(NULL),
     m_populating(false) {

   QVBoxLayout* vbox = new QVBoxLayout;
   setLayout(vbox);

   m_wgtTree = new QTreeWidget(this);
   vbox->addWidget(m_wgtTree);

   connect(m_wgtTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(itemEdited(QTreeWidgetItem*, int)));
}

//===========================================
// WgtXmlTreeView::itemEdited
//===========================================
void WgtXmlTreeView::itemEdited(QTreeWidgetItem* item, int column) {
   if (m_populating) return;

   XmlTreeItem* i = static_cast<XmlTreeItem*>(item);

   QString qstr = item->text(column);
   QByteArray bytes = qstr.toLocal8Bit();
   std::string val(bytes.data(), bytes.length());

   if (i->type() == xmlThing_t::NODE) {
      XmlNode node = i->getNode();
      node.setValue(val);
   }
   else if (i->type() == xmlThing_t::ATTRIBUTE) {
      XmlAttribute attr = i->getAttribute();
      attr.setValue(val);
   }

   emit onUpdate();
}

//===========================================
// WgtXmlTreeView::update
//===========================================
void WgtXmlTreeView::update(weak_ptr<XmlDocument> doc) {
   m_wgtTree->clear();

   m_document = doc;

   populateXmlTree();
   m_wgtTree->setColumnWidth(0, 200);
}

//===========================================
// WgtXmlTreeView::populateXmlTree
//===========================================
void WgtXmlTreeView::populateXmlTree() {
   auto doc = m_document.lock();
   if (!doc) return;

   m_populating = true;

   XmlNode node = doc->firstNode();

   QTreeWidgetItem* item = new XmlTreeItem(m_wgtTree, QStringList(QString(node.name().data())), xmlThing_t(node));

   XmlAttribute attr = node.firstAttribute();
   while (!attr.isNull()) {
      QStringList list;
      list << QString(attr.name().data());
      list << QString(attr.getString().data());

      QTreeWidgetItem* attrItem = new XmlTreeItem(item, list, xmlThing_t(attr));
      attrItem->setFlags(attrItem->flags() | Qt::ItemIsEditable);
      item->addChild(attrItem);

      attr = attr.nextAttribute();
   }

   populateXmlTree_r(item, node);

   m_wgtTree->addTopLevelItem(item);
   m_wgtTree->setColumnCount(2);

   QStringList columns;
   columns << "Object" << "Value";
   m_wgtTree->setHeaderLabels(columns);

   m_populating = false;
}

//===========================================
// WgtXmlTreeView::populateXmlTree_r
//===========================================
void WgtXmlTreeView::populateXmlTree_r(QTreeWidgetItem* parent, const XmlNode& node) {
   XmlNode ch = node.firstChild();

   while (!ch.isNull()) {
      bool blankNode = ch.name().length() == 0;

      QStringList list;

      if (blankNode) {
         list << "-" << QString(ch.getString().data());
      }
      else {
         list << QString(ch.name().data());
      }

      QTreeWidgetItem* item = new XmlTreeItem(parent, list, xmlThing_t(ch));

      if (blankNode) {
         item->setFlags(item->flags() | Qt::ItemIsEditable);
      }
      else {
         XmlAttribute attr = ch.firstAttribute();
         while (!attr.isNull()) {
            QStringList list;
            list << QString(attr.name().data());
            list << QString(attr.getString().data());

            QTreeWidgetItem* attrItem = new XmlTreeItem(item, list, xmlThing_t(attr));
            attrItem->setFlags(attrItem->flags() | Qt::ItemIsEditable);

            item->addChild(attrItem);

            attr = attr.nextAttribute();
         }

         populateXmlTree_r(item, ch);
      }
      parent->addChild(item);

      ch = ch.nextSibling();
   }
}
