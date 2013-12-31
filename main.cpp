#include "Application.hpp"
#include "MainWindow.hpp"


int main(int argc, char** argv) {
   Application app(argc, argv);  
   MainWindow window;
   window.show();

   return app.exec();
}
