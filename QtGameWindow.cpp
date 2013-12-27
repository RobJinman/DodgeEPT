#include <iostream>
#include <QPushButton>
#include <QAction>
#include "QtGameWindow.hpp"


using namespace std;


//===========================================
// QtGameWindow::QtGameWindow
//===========================================
QtGameWindow::QtGameWindow(QWidget* parent)
   : QMainWindow(parent),
     m_btn(NULL) {

   resize(200, 200);
   setWindowTitle("Dodge :: Entity Placement Tool

   m_btn = new QPushButton("Button");

   setCentralWidget(m_btn);

   connect(m_btn, SIGNAL(released()), this, SLOT(foo()));
}

//===========================================
// QtGameWindow::foo
//===========================================
void QtGameWindow::foo() {
   cout << "Click!\n";
}

//===========================================
// QtGameWindow::~QtGameWindow
//===========================================
QtGameWindow::~QtGameWindow() {
   delete m_btn;
}
