#ifndef __MAIN_WINDOW_HPP__
#define __MAIN_WINDOW_HPP__


#include <QMainWindow>
#include <map>
#include <vector>
#include <set>
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
class QCheckBox;
class QTreeWidget;
class QTreeWidgetItem;
class WgtXmlTreeView;
class WgtMapSettings;
class Importer;
class Exporter;


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
      void onAssetSelection(QTreeWidgetItem* item, int column);
      void onImport();
      void onExport();
      void onChkPrototypeChanged(int state);
      void onChkGlobalChanged(int state);
      void onSpnSegmentXChanged(int value);
      void onSpnSegmentYChanged(int value);
      void onMapSettingsChange();

   private:
      void buildMapFile();
      void exportPrototypes();
      void exportInstances();
      void computeDependencies();
      void updateAssetList(const QString& select);
      void updateAssetList_r(QTreeWidgetItem* parent, std::weak_ptr<EptObject> obj);

      std::unique_ptr<Importer>  m_importer;
      std::unique_ptr<Exporter>  m_exporter;
      ObjectContainer            m_objects;
      std::weak_ptr<EptObject>   m_current;

      QMenu*                     m_mnuFile;
      QAction*                   m_actSave;
      QAction*                   m_actSaveAs;
      QAction*                   m_actImport;
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
      QGroupBox*                 m_wgtGrpSegment;
      QLabel*                    m_wgtLblSegmentX;
      QLabel*                    m_wgtLblSegmentY;
      QSpinBox*                  m_wgtSpnSegmentX;
      QSpinBox*                  m_wgtSpnSegmentY;
      QCheckBox*                 m_wgtChkGlobal;
      QPushButton*               m_wgtBtnInferSegment;
      QWidget*                   m_wgtObjectsTab;
      QTreeWidget*               m_wgtTreAssets;
      QGroupBox*                 m_wgtGrpAssets;
      QLineEdit*                 m_wgtTxtNewAsset;
      QPushButton*               m_wgtBtnNewAsset;
      QCheckBox*                 m_wgtChkNewIsPrototype;
      WgtMapSettings*            m_wgtMapSettingsTab;
};


#endif
