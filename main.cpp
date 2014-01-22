#include <iostream>
#include <dodge/globals.hpp>
#include "Application.hpp"
#include "MainWindow.hpp"
#include "Common.hpp"


int main(int argc, char** argv) {
   Dodge::gInitialise();

   addPrintStream("warning", pCharStream_t(new CharStream(&std::cout)));
   addPrintStream("error", pCharStream_t(new CharStream(&std::cerr)));

   Application app(argc, argv);  
   app.setStyle("cleanlooks");

   MainWindow window;
   window.show();

   return app.exec();
}
