#include "inc/mainwindow.h"
#include "inc/dialog.h"

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

extern int uniqueAlbumId;
extern int uniqueArtistId;

MainWindow::MainWindow(const QString &artistTable, const QString &albumTable,
											 QFile *albumDetails, QWidget *parent)
		: QMainWindow(parent)
{
}
