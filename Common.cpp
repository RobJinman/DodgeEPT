#include <QString>
#include <QMessageBox>
#include "Common.hpp"


void alert(const QString& msg) {
   QMessageBox::information(NULL, "Alert", msg);
}
