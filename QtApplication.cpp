#include <stdexcept>
#include <iostream>
#include "QtApplication.hpp"


using namespace std;


//===========================================
// QtApplication::QtApplication
//===========================================
bool QtApplication::notify(QObject* receiver, QEvent* event) {
   try {
      return QApplication::notify(receiver, event);
   } 
   catch (exception& e) {
      cerr << "Encountered fatal error: " << e.what() << "\n";
      exit(EXIT_FAILURE);
   }
   catch (...) {
      cerr << "Encountered fatal error: Cause unknown\n";
      exit(EXIT_FAILURE);
   }

   return false;
}
