#ifndef __MAIN_WINDOW_HPP__
#define __MAIN_WINDOW_HPP__


#include <QMainWindow>
#include <string>


class QAction;
class QPushButton;
class QTextEdit;
class XmlTreeView;

class MainWindow : public QMainWindow {
   Q_OBJECT

   public:
      MainWindow(QWidget* parent = NULL);

      virtual ~MainWindow();

   private slots:
      void btnApplyClick();
      void xmlTreeUpdated(const std::string& text);

   private:
      QWidget*       m_wgtCentral;
      QPushButton*   m_wgtButton;
      XmlTreeView*   m_wgtXmlTree;
      QTextEdit*     m_wgtXmlEdit;
};


#endif
