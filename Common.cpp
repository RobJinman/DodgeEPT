#include <QString>
#include <QMessageBox>
#ifdef LINUX
#include <sys/stat.h>
#endif
#include "Common.hpp"


void alert(const QString& msg) {
   QMessageBox::information(NULL, "Alert", msg);
}

void alert_std(const std::string& msg) {
   QString str(msg.data());
   QMessageBox::information(NULL, "Alert", str);
}

#ifdef WIN32
bool createDir(const std::string& path) {
   assert(false);

   return false;
}
#endif
#ifdef LINUX
bool createDir(const std::string& path) {
   return (mkdir(path.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 || errno == EEXIST) ? true : false;
}
#endif
