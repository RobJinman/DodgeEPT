#ifndef __MAIN_WINDOW_HPP__
#define __MAIN_WINDOW_HPP__


#include <QMainWindow>
#include <map>
#include "ObjectContainer.hpp"


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
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QListWidget;
class QListWidgetItem;
class QCheckBox;
class WgtXmlTreeView;
class WgtMapSettings;


class MainWindow : public QMainWindow {
   Q_OBJECT

   public:
      MainWindow(QWidget* parent = NULL);

      virtual ~MainWindow();

   private slots:
      void btnApplyClick();
      void btnNewAssetClick();
      void xmlTreeUpdated();
      void onPrototypeSelection(const QString& name);
      void onAssetSelection(QListWidgetItem* item);
      void onExport();
      void onChkPrototypeChanged(int state);

   private:
      void exportMapSettings();
      void exportPrototypes();
      void exportInstances();

      ObjectContainer            m_objects;

      std::string                m_root;
      std::weak_ptr<EptObject>   m_current;

      QMenu*                     m_mnuFile;
      QAction*                   m_actSave;
      QAction*                   m_actSaveAs;
      QAction*                   m_actExport;
      QAction*                   m_actQuit;
      QWidget*                   m_wgtCentral;
      QTabWidget*                m_wgtLeftColumnTabs;
      QWidget*                   m_wgtToolsTab;
      QToolBox*                  m_wgtTools;
      QComboBox*                 m_wgtCboPrototypes;
      QTabWidget*                m_wgtCentralColumnTabs;
      QWidget*                   m_wgtXmlEditTab;
      QTextEdit*                 m_wgtXmlEdit;
      QPushButton*               m_wgtXmlApply;
      QWidget*                   m_wgtDrawScreenTab;
      QGraphicsView*             m_wgtDrawScreen;
      QTabWidget*                m_wgtRightColumnTabs;
      QWidget*                   m_wgtXmlTreeTab;
      WgtXmlTreeView*            m_wgtXmlTree;
      QCheckBox*                 m_wgtChkPrototype;
      QWidget*                   m_wgtObjectsTab;
      QListWidget*               m_wgtLstAssets;
      QGroupBox*                 m_wgtGrpAssets;
      QLineEdit*                 m_wgtTxtNewAsset;
      QPushButton*               m_wgtBtnNewAsset;
      QCheckBox*                 m_wgtChkNewIsPrototype;
      WgtMapSettings*            m_wgtMapSettingsTab;

};


#endif
