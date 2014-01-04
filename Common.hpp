#ifndef __EPT_COMMON_HPP__
#define __EPT_COMMON_HPP__


#include <sstream>
#include <dodge/Exception.hpp>


typedef Dodge::Exception EptException;


#define EXCEPTION(x) { \
   std::stringstream ss; \
   ss << x; \
   throw EptException(ss.str(), __FILE__, __LINE__); \
}


#endif
