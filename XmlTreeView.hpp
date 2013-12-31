#ifndef __XML_TREE_VIEW_HPP__
#define __XML_TREE_VIEW_HPP__


#include <dodge/xml/xml.hpp>
#include <QWidget>
#include <QTreeWidget>
#include <boost/variant.hpp>
#include <memory>


struct xmlThing_t {
   enum type_t { NODE, ATTRIBUTE };

   explicit xmlThing_t(const Dodge::XmlNode& node)
      : type(NODE), thing(node) {}

   explicit xmlThing_t(const Dodge::XmlAttribute& attr)
      : type(ATTRIBUTE), thing(attr) {}

   xmlThing_t(const xmlThing_t& cpy) {
      type = cpy.type;
      thing = cpy.thing;
   }

   ~xmlThing_t() {}

   type_t type;

   boost::variant<
      Dodge::XmlNode,
      Dodge::XmlAttribute
   > thing;
};


class XmlTreeItem : public QTreeWidgetItem {
   public:
      XmlTreeItem(QTreeWidget* parent, const QStringList& strings, const xmlThing_t& xml)
         : QTreeWidgetItem(parent, strings),
           m_xml(xml) {}

      XmlTreeItem(QTreeWidgetItem* parent, const QStringList& strings, const xmlThing_t& xml)
         : QTreeWidgetItem(parent, strings),
           m_xml(xml) {}

      xmlThing_t::type_t type() const {
         return m_xml.type;
      }

      Dodge::XmlNode getNode() {
         try {
            return boost::get<Dodge::XmlNode>(m_xml.thing);
         }
         catch (boost::bad_get& e) {
            return Dodge::XmlNode();
         }
      }

      Dodge::XmlAttribute getAttribute() {
         try {
            return boost::get<Dodge::XmlAttribute>(m_xml.thing);
         }
         catch (boost::bad_get& e) {
            return Dodge::XmlAttribute();
         }
      }

      virtual ~XmlTreeItem() {}

   private:
      xmlThing_t m_xml;
};


class XmlTreeView : public QWidget {
   Q_OBJECT

   public:
      XmlTreeView(QWidget* parent = 0);

      void update(std::unique_ptr<Dodge::XmlDocument> doc);

      virtual ~XmlTreeView() {}

   private slots:
      void itemEdited(QTreeWidgetItem* item, int column);

   signals:
      void onUpdate(const std::string& text);

   private:
      void populateXmlTree();
      void populateXmlTree_r(QTreeWidgetItem* parent, const Dodge::XmlNode& node);

      std::unique_ptr<Dodge::XmlDocument> m_document;
      QTreeWidget* m_wgtTree;
      bool m_populating;
};


#endif
