#ifndef __EPT_COMMON_HPP__
#define __EPT_COMMON_HPP__


#include <sstream>
#include <string>
#include <dodge/Exception.hpp>


typedef Dodge::Exception EptException;


#define EXCEPTION(x) { \
   std::stringstream except_ss; \
   except_ss << x; \
   throw EptException(except_ss.str(), __FILE__, __LINE__); \
}


class QString;
void alert(const QString& msg);
void alert_std(const std::string& msg);

// Returns true on success
bool createDir(const std::string& path);


#endif
