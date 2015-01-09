#include <utils/Utils.hpp>
#include "Common.hpp"


class Notifications : public ICharStream {
   public:
      Notifications(const Functor<void, TYPELIST_0()>& onInput);

      virtual Notifications& operator<<(const std::string& str);

      void clear();

      const QString& getText() const;

      virtual ~Notifications();

   private:
      QString m_text;
      Functor<void, TYPELIST_0()> m_onInput;
};
