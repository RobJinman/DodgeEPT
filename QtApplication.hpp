#ifndef __QT_APPLICATION_HPP__
#define __QT_APPLICATION_HPP__


#include <QApplication>


class QtApplication : public QApplication {
   public:
      QtApplication(int argc, char** argv)
         : QApplication(argc, argv) {}

      virtual bool notify(QObject* receiver, QEvent* event);
};


#endif
