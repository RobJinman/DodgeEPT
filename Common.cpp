#include <QString>
#include <QMessageBox>
#ifdef LINUX
#include <sys/stat.h>
#include <dirent.h>
#endif
#include <map>
#include "Common.hpp"


using namespace std;


static std::map<std::string, pCharStream_t> streams;


//===========================================
// ICharStream::~ICharStream
//===========================================
ICharStream::~ICharStream() {}

//===========================================
// CharStream::CharStream
//===========================================
CharStream::CharStream(std::ostream* stream)
   : m_stream(stream) {}

//===========================================
// CharStream::operator<<
//===========================================
CharStream& CharStream::operator<<(const std::string& str) {
   (*m_stream) << str;
   return *this;
}

//===========================================
// alert
//===========================================
void alert(const QString& msg) {
   QMessageBox::information(NULL, "Alert", msg);
}

//===========================================
// alert_std
//===========================================
void alert_std(const string& msg) {
   QString str(msg.data());
   QMessageBox::information(NULL, "Alert", str);
}

//===========================================
// addPrintStream
//===========================================
void addPrintStream(const std::string& name, pCharStream_t stream) {
   streams.insert(make_pair(name, stream));
}

//===========================================
// print
//===========================================
void print(const std::string& stream, const std::string& str) {
   auto i = streams.find(stream);

   if (i != streams.end()) {
      (*i->second) << str << "\n";
   }
}

#ifdef WIN32
//===========================================
// createDir
//===========================================
bool createDir(const string& path) {
   // TODO
   assert(false);

   return false;
}

//===========================================
// filesInDir
//===========================================
void filesInDir(const string& path, vector<string>& files) {
   // TODO
   assert(false);
}
#endif
#ifdef LINUX
//===========================================
// createDir
//===========================================
bool createDir(const string& path) {
   return (mkdir(path.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 || errno == EEXIST) ? true : false;
}

//===========================================
// filesInDir
//===========================================
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
