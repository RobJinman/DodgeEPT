#include <dodge/globals.hpp>
#include "Application.hpp"
#include "MainWindow.hpp"


int main(int argc, char** argv) {
   Dodge::gInitialise();

   Application app(argc, argv);  
   MainWindow window;
   window.show();

   return app.exec();
}
