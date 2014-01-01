#ifndef __MAIN_WINDOW_HPP__
#define __MAIN_WINDOW_HPP__


#include <QMainWindow>
#include <string>


class QAction;
class QPushButton;
class QTextEdit;
class QTabWidget;
class QGraphicsView;
class QToolBox;
class QMenu;
class QComboBox;
class QGroupBox;
class QLineEdit;
class XmlTreeView;

class MainWindow : public QMainWindow {
   Q_OBJECT

   public:
      MainWindow(QWidget* parent = NULL);

      virtual ~MainWindow();

   private slots:
      void btnApplyClick();
      void btnNewPrototypeClick();
      void btnNewInstanceClick();
      void xmlTreeUpdated(const std::string& text);

   private:
      QMenu*         m_mnuFile;
      QAction*       m_actSave;
      QAction*       m_actSaveAs;
      QAction*       m_actExport;
      QAction*       m_actQuit;
      QWidget*       m_wgtCentral;
      QTabWidget*    m_wgtLeftColumnTabs;
      QWidget*       m_wgtToolsTab;
      QToolBox*      m_wgtTools;
      QTabWidget*    m_wgtCentralColumnTabs;
      QWidget*       m_wgtXmlEditTab;
      QTextEdit*     m_wgtXmlEdit;
      QPushButton*   m_wgtXmlApply;
      QWidget*       m_wgtDrawScreenTab;
      QGraphicsView* m_wgtDrawScreen;
      QTabWidget*    m_wgtRightColumnTabs;
      QWidget*       m_wgtXmlTreeTab;
      XmlTreeView*   m_wgtXmlTree;
      QWidget*       m_wgtObjectsTab;
      QGroupBox*     m_wgtGrpPrototypes;
      QComboBox*     m_wgtCboPrototypes;
      QLineEdit*     m_wgtTxtNewPrototype;
      QPushButton*   m_wgtBtnNewPrototype;
      QGroupBox*     m_wgtGrpInstances;
      QComboBox*     m_wgtCboInstances;
      QLineEdit*     m_wgtTxtNewInstance;
      QPushButton*   m_wgtBtnNewInstance;
};


#endif