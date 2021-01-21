#include "inc/mainwindow.h"
#include "inc/dialog.h"

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

extern int uniqueAlbumId;
extern int uniqueArtistId;

MainWindow::MainWindow(const QString &artistTable, const QString &albumTable,
                       QFile *albumDetails, QWidget *parent)
void MainWindow::changeArtist(int row)
{
    if (row > 0){
        QModelIndex index = model->relationModel(2)->index(row, 1);
        model->setFilter("artist = '" + index.data().toString() +'\'');
        showArtistProfile(index);
    }else if (row ==0){
        model->setFilter(QString());
        showImageLabel();
    }else{
        return;
    }
}
void MainWindow::showArtistProfile(QModelIndex index)
{
    QSqlRecord record = model->relationModel(2)->record(index.row());
    QString name = record.value("artist").toString();
    QString count = record.value("albumcount").toString();
    profileLabel->setText(tr("Artist :%1 \n" \
                               "Number of Album:%2").arg(name).arg(count));
    profileLabel->show();
    iconLabel->show();
    titleLabel->hide();
    trackList->hide();
    imageLabel->hide();
}
void MainWindow::showAlbumDetails(QModelIndex index)
{
    QSqlRecord record =model->relationModel(2)->record(index.row());
    QString artist =record.value("artist").toString();
    QString title =record.value("title").toString();
    QString year =record.value("year").toString();
    QString albumId =record.value("albumid").toString();

    showArtistProfile(indexOfArtist(artist));
    titleLabel->setText(tr("Title: %1 (%2").arg(title).arg(year));
    titleLabel->show();
    QDomNodeList albums = albumData.elementsByTagName("album");
    for (int i=0; i < albums.count(); ++i){
        QDomNodeList albums = albums.item(i);
        if (albums.toElement().attribute("id") == albumId){
            getTrackList(albums.toElement());
            break;
        }
    }
    if (trackList->count() !=0)
        trackList->show();
}
void MainWindow::getTrackList(QDomNode album)
{
    trackList->clear();

    QDomNodeList tracks = album.childNodes();
    QDomNode track;
    QString trackNumber;

    for (int i = 0; i < tracks.count(); ++i){
        track = tracks.item(i);
        trackNumber = track.toElement().attribute("number");

        QListWidgetItem *item = new QListWidgetItem(tracklist);
        item->setText(trackNumber + ": " + track.toElement().text());
    }
}
void MainWindow::addAlbum()
{
    Dialog *dialog = new Dialog(model, albumData, file, this);
    int accepted = dialog->exec();

    if (accepted == 1){
        int lastRow = model->rowCount() -1;
        albumView->selectRow(lastRow);
        albumView->scrollToBottom();
        showAlbumDetails(model->index(lastRow, 0));
    }
}
void MainWindow::deleteAlbum()
{
    QModelIndexList selection = albumView->selectionModel()->selectedRows(0);

    if (!selection.empty()){
        QModelIndex idIndex = selection.at(0);
        int id = idIndex.data().toInt();
        QString title = idIndex.sibling(idIndex.row(), 1).data().toString();
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Delete Album"),
                                       tr("Are you sure you want to"
)                                           "delete '%1' by '%2'?")
                .arg(title, artist),
                QMessageBox::Yes|QMessageBox::No);
        if (button == QMessageBox::Yes){
            removeAlbumFromFile(id);
            removeAlbumFromDatabase(idIndex);
            decreaseAlbumCount(indexOfArtist(artist));

            showImageLabel();
        }
    }else {
        QMessageBox::information(this, tr("Delete Album"),
                                 tr("Select the album you want to delete."));
    }
}
void MainWindow::removeAlbumFromFile(int id)
{
    QDomNodeList album = albumData.elementByTagName("album");

    for (int i=0; i< album.count(); ++i){
        QDomNode node = album.item(i);
        if (node.toElement().attribute("id").toInt() == id) {
            albumData. elementsByTagName("archive").item(0).removeChild(node);
            break;
        }
    }
}
void MainWindow::removeAlbumFromDatabase(QModelIndex index)
{
    model->removeRow(index.row());
}

void MainWindow::decreaseAlbumCount(QModelIndex artistIndex)
{
    int row = artistIndex.row();
    QModelIndex albumCountIndex =artistIndex.sibling(row,2);
    int albumCount = albumCountIndex.data().toInt();

    QSqlTableModel *artists = model->relationModel(2);

    if(albumCount == 1){
        artists->removeRow(row);
        showImageLabel();
    }else{
        artists->setData(albumCountIndex, QVariant(albumCount -1));
    }
}
void MainWindow::readAlbumData()
{
    if(!file->open(QIODevice::ReadOnly))
        return;

    if(!albumData.setContent(file)){
        file->close();
    }else{
        artists->setData(albumCountIndex, QVariant(albumCount -1));
    }
}
QGroupBox* MainWindow::createArtistGroupBox()
{
    artistView = new QComboBox;
    artistView ->setModel(model->relationModel(2));
    artistView->setModelColumn(1);

    connect(artistView, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::changeArtist);

    QGroupBox *box = new QGroupBox(tr("Artist"));

    QGroupBox *layout = new QGridLayout;
    layout->addWidget(artistView, 0, 0);
    box->setLayout(layout);

    return box;
}
QGroupBox* MainWindow::createAlbumGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("Artist"));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(artistView, 0, 0);
    box->setLayout(layout);

    return box;
}

QGroupBox* MainWindow::createAlbumGroupBox()
{
QGroupBox *box = new QGroupBox(tr("album"));

albumView = new QTableView;
albumView->setEditTriggers(QAbstractItemView::NoEditTriggers);
albumView->setSortingEnabled(true);
albumView->setSelectionBehavior(QAbstractItemView::SelectRows);
 albumView->setSelectionMode(QAbstractItemView::SingleSelction);
 albumView->setShowGrid(false);
 albumView->verticalHeader()->hide();
 albumView->setAlternatingRowColors(true);
 albumView->setModel(model);
 ajustHeader();

 QLocale locale = albumView->locale();
 locale.setNumberOptions(Qlocale::OmitGroupSeparator);
 albumView->setLocale(locale);

connect(albumView, &QTableView::clicked
        this, &MainWindow::showAlbumDetails);
connect(albumView, &QTableView::activated
        this, &MainWindow::showAlbumDetails);

QGridLayout *layout = new QVBoxLayout;
layout->addWidget(artistView, 0, 0);
box->setLayout(layout);

return box;
}
QGroupBox* MainWindow::createDetailsGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("Details"));

    profileLabel = new QLabel;
    profileLabel->setWordWrap(true);
    profileLabel->setAlignement(Qt::AlignBottom);

    titleLabel = new QLabel;
    titleLabel->setWordWrap(true);
    titleLabel->setAlignement(Qt::AlignBottom);

    iconLabel = new QLabel;
    iconLabel->setWordWrap(Qt::AlignBottom|Qt::AlignRight);
    iconLabel->setPixmap(QPixmap(":images/icon.png"));

    imageLabel = new QLabel;
    imageLabel->setWordWrap(true);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setPixmap(QPixmap("/:images/image.png"));

    trackList = new QListWidget;

            QGridLayout *layout = new QGridLayout;

    QGridLayout *Layout = new QGridLayout;
    layout->addWidget(imageLabel,0,0,3,2);
    layout->addWidget(profileLabel,0,0);
    layout->addWidget(iconLabel,0,1);
    layout->addWidget(titleLabel,1,0,1,2);
    layout->addWidget(trackList,2,0,1,2);
    layout->setRowStretch(2, 1);
    box->setLayout(layout);
    return box;
}

void MainWindow::createMenuBar()
{
    QAction *addAction = new QAction(tr("&add album..."), this);
    QAction *deleteAction = new QAction(tr("&Delete album..."), this);
    QAction *quitAction = new QAction(tr("&Quit"), this);
    QAction *aboutAction = new QAction(tr("&About"), this);
    QAction *aboutQtAction = new QAction(tr("&About &Qt"), this);

    addAction->setShortcut(tr("ctrl+A"));
     deleteAction->setShortcut(tr("ctrl+D"));
     quitAction->setShortcuts(QKeySequence::Quit);

     QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(addAction);
     fileMenu->addAction(deleteAction);
     fileMenu->addSeparator();
     fileMenu->addAction(quitAction);

     QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
     helpMenu->addAction(aboutAction);
     helpMenu->addAction(aboutQtAction);

     connect (addAction, &QAction::triggered,
              this, &MainWindow::addAlbum);
     connect (deleteAction, &QAction::triggered,
              this, &MainWindow::deleteAlbum);
     connect (quitAction, &QAction::triggered,
              this, &MainWindow::close);
     connect (aboutAction, &QAction::triggered,
              this, &MainWindow::about);
     connect (aboutQtAction, &QAction::triggered,
              qApp, &QApplication::aboutQt);
}
void MainWindow::showImageLabel()
{
    profileLabel->hide();
    titleLabel->hide();
    iconLabel->hide();
    trackList->hide();

    imageLabel->show();
}
QModelIndex MainWindow::indexOfArtist(const QString &artist)
{
    QSqlTableModel *artistModel = model->relationModel(2);

    for (int i=0; i < artistModel->rowCount(); i++){
        QSqlRecord record = artistModel->record(i);
        if (record.value("artist") == artist)
            return artistModel->index(i,1);
    }
    return QModelIndex();
}
void MainWindow::updateHeader(QModelIndex, int, int)
{
    adjustHeader();
}

void MainWindow::adjustHeader()
{
    albumView->hideColumn(0);
    albumView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    albumView->resizeColumnToContents(2);
    albumView->resizeColumnToContents(3);
}
void MainWindow::about()
{
  QMessageBox::about(this, tr("About Music Archive"),
                     tr("<p>The <b>Music Archive</b> example shows how to present"
                        "data from different data sources in the same applucation."
                        "The album titles, and the corresponding artists and release dates,"
                        "are kept in a database, while each album's tracks are stored"
                        "in an XML file. </p><p>The example also shows how to add as"
                        "well as remove data from both the database and the"
                        "associated XML file using the API provided by the Qt SQL and"
                        "Qt XML modules, respectively.</p>"));
}
