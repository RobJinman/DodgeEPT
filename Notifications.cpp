#include <QString>
#include "Notifications.hpp"


using namespace std;


//===========================================
// Notifications::Notifications
//===========================================
Notifications::Notifications(const Functor<void, TYPELIST_0()>& onInput)
   : m_onInput(onInput) {}

//===========================================
// Notifications::operator<<
//===========================================
Notifications& Notifications::operator<<(const string& str) {
   m_text.append(str.data());
   m_onInput();

   return *this;
}

//===========================================
// Notifications::clear
//===========================================
void Notifications::clear() {
   m_text.clear();
}

//===========================================
// Notifications::getText
//===========================================
const QString& Notifications::getText() const {
   return m_text;
}

//===========================================
// Notifications::~Notifications
//===========================================
Notifications::~Notifications() {}
