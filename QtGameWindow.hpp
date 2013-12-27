#ifndef __QT_GAME_WINDOW_HPP__
#define __QT_GAME_WINDOW_HPP__


#include <QMainWindow>


class QAction;
class QPushButton;

class QtGameWindow : public QMainWindow {
   Q_OBJECT

   public:
      QtGameWindow(QWidget* parent = NULL);

      virtual ~QtGameWindow();

   private slots:
      void foo();

   private:
      QPushButton* m_btn;
};


#endif
