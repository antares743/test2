#include "inc/mainwindow.h"
#include "inc/database.h"
#include <QApplication>


#include <QApplication>
#include <QFile>

#include <stdlib.h>

int main(int argc, char *argv[])
{
 Q_INIT_RESOURCE(masterdetail);

 QApplication app(argc, argv);

 if (!createConnection())
  return EXIT_FAILURE;

 QFile albumDetails("albumdetails.xml");
 MainWindow window("artists", "albums", &albumDetails);
 window.show();
 return app.exec();
}
