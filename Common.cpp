#include <QString>
#include <QMessageBox>
#ifdef LINUX
#include <sys/stat.h>
#include <dirent.h>
#endif
#include "Common.hpp"


using namespace std;


void alert(const QString& msg) {
   QMessageBox::information(NULL, "Alert", msg);
}

void alert_std(const string& msg) {
   QString str(msg.data());
   QMessageBox::information(NULL, "Alert", str);
}

#ifdef WIN32
bool createDir(const string& path) {
   // TODO
   assert(false);

   return false;
}

void filesInDir(const string& path, vector<string>& files) {
   // TODO
   assert(false);
}
#endif
#ifdef LINUX
bool createDir(const string& path) {
   return (mkdir(path.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 || errno == EEXIST) ? true : false;
}

void filesInDir(const string& path, vector<string>& files) {
   DIR* dirp = opendir(path.data());
   dirent* p = NULL;

   while ((p = readdir(dirp)) != NULL) {
      string fn(p->d_name);
      files.push_back(fn);
   }

   closedir(dirp);
}
#endif
