#ifndef __EPT_OBJECT_HPP__
#define __EPT_OBJECT_HPP__


#include <memory>
#include <QString>
#include <set>
#include <dodge/xml/xml.hpp>
#include <dodge/math/Vec2i.hpp>


struct XmlParseResult {
   enum result_t { SUCCESS, FAILURE };

   XmlParseResult()
      : result(SUCCESS) {}

   result_t result;
   std::string msg;
};

class EptObject {
   public:
      friend class EptObjAccessor;

      enum type_t { INSTANCE, PROTOTYPE };

      EptObject(const Dodge::XmlNode& node, type_t type);
      EptObject(const QString& name, type_t type);
      EptObject(type_t type);

      inline const Dodge::Vec2i& segment() const;
      inline long id() const;
      inline const QString& name() const;
      inline type_t type() const;
      inline const std::weak_ptr<Dodge::XmlDocument> xml() const;
      void parseXml(const QString& text, XmlParseResult& result);
      inline const std::set<long>& dependencies() const;

      void computeDependencies();

   private:
      EptObject(const EptObject&) { assert(false); }

      void computeDependencies_r(const Dodge::XmlNode& node, int depth = 0);

      long m_id;
      type_t m_type;
      QString m_name;
      Dodge::Vec2i m_segment;
      std::shared_ptr<Dodge::XmlDocument> m_xml;
      std::set<long> m_dependencies;

      static long m_nextId;
};

class EptObjAccessor {
   public:
      static Dodge::Vec2i& segment(EptObject& obj) {
         return obj.m_segment;
      }

      static EptObject::type_t& type(EptObject& obj) {
         return obj.m_type;
      }

      static QString& name(EptObject& obj) {
         return obj.m_name;
      }
};

inline long EptObject::id() const {
   return m_id;
}

inline const QString& EptObject::name() const {
   return m_name;
}

inline EptObject::type_t EptObject::type() const {
   return m_type;
}

inline const Dodge::Vec2i& EptObject::segment() const {
   return m_segment;
}

inline const std::weak_ptr<Dodge::XmlDocument> EptObject::xml() const {
   return m_xml;
}

inline const std::set<long>& EptObject::dependencies() const {
   return m_dependencies;
}


#endif
