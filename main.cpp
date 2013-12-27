#include "QtApplication.hpp"
#include "QtGameWindow.hpp"


int main(int argc, char** argv) {
   QtApplication app(argc, argv);  
   QtGameWindow window;
   window.show();

   return app.exec();
}
