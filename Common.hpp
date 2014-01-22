#ifndef __EPT_COMMON_HPP__
#define __EPT_COMMON_HPP__


#include <sstream>
#include <string>
#include <vector>
#include <ostream>
#include <memory>
#include <dodge/Exception.hpp>


class ICharStream {
   public:
      virtual ICharStream& operator<<(const std::string& str) = 0;

      virtual ~ICharStream() = 0;
};

class CharStream : public ICharStream {
   public:
      CharStream(std::ostream* stream);

      virtual CharStream& operator<<(const std::string& str);

   private:
      std::ostream* m_stream;
};


typedef std::shared_ptr<ICharStream> pCharStream_t;
typedef Dodge::Exception EptException;


#define EXCEPTION(x) { \
   std::stringstream local_ss; \
   local_ss << x; \
   throw EptException(local_ss.str(), __FILE__, __LINE__); \
}


#define PRINT_STRING(stream, msg) { \
   stringstream local_ss; \
   local_ss << msg; \
   print(stream, local_ss.str()); \
}


class QString;
void alert(const QString& msg);
void alert_std(const std::string& msg);

// Returns true on success
bool createDir(const std::string& path);

void filesInDir(const std::string& path, std::vector<std::string>& files);

void addPrintStream(const std::string& name, pCharStream_t stream);
void print(const std::string& stream, const std::string& msg);


#endif
