#ifndef __EPT_OBJECT_HPP__
#define __EPT_OBJECT_HPP__


#include <memory>
#include <QString>
#include <dodge/xml/xml.hpp>


class EptObject {
   public:
      enum type_t { INSTANCE, PROTOTYPE };

      EptObject(const QString& name, type_t type);

      inline const QString& name() const;
      inline type_t type() const;
      inline std::weak_ptr<Dodge::XmlDocument> xml() const;

   private:
      type_t m_type;
      QString m_name;
      std::shared_ptr<Dodge::XmlDocument> m_xml;
};

inline const QString& EptObject::name() const {
   return m_name;
}

inline EptObject::type_t EptObject::type() const {
   return m_type;
}

inline std::weak_ptr<Dodge::XmlDocument> EptObject::xml() const {
   return m_xml;
}


#endif
