#include <QString>
#include <QMessageBox>
#include "Common.hpp"


void alert(const QString& msg) {
   QMessageBox::information(NULL, "Alert", msg);
}

void alert_std(const std::string& msg) {
   QString str(msg.data());
   QMessageBox::information(NULL, "Alert", str);
}
