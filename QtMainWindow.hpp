#ifndef __QT_MAIN_WINDOW_HPP__
#define __QT_MAIN_WINDOW_HPP__


#include <QMainWindow>
#include <dodge/xml/xml.hpp>


class QAction;
class QPushButton;
class QTextEdit;
class QTreeWidget;
class QTreeWidgetItem;

class QtMainWindow : public QMainWindow {
   Q_OBJECT

   public:
      QtMainWindow(QWidget* parent = NULL);

      virtual ~QtMainWindow();

   private slots:
      void foo();

   private:
      void populateXmlTree(Dodge::XmlDocument& doc);
      int populateXmlTree_r(QTreeWidgetItem* parent, Dodge::XmlNode& node);

      QWidget*       m_wgtCentral;
      QPushButton*   m_wgtButton;
      QTreeWidget*   m_wgtXmlTree;
      QTextEdit*     m_wgtXmlEdit;
};


#endif
