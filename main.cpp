#include "QtApplication.hpp"
#include "QtMainWindow.hpp"


int main(int argc, char** argv) {
   QtApplication app(argc, argv);  
   QtMainWindow window;
   window.show();

   return app.exec();
}
