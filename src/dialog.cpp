#include "inc/dialog.h"

int uniqueAlbumId;
int uniqueArtistId;

Dialog::Dialog(QSqlRelationalTableModel *albums, QDomDocument details,
							 QFile *output, QWidget *parent)
		: QDialog(parent)
{
    model = albums;
    albumDetails = details;
    outputFile = output;

    QGroupBox *inputWidgetBox =createInputWidgets();
    QDialogButtonBox *buttonBox =createButtons();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(inputWidgetBox);
    layout->addWidget(buttonBox);
    setLayout(layout);

    setWindowTitle(tr("add Album"));
}
void Dialog::submit()
{
    QString artist =artistEditor->text();
    QString title = titleEditor->text();

    if(artist.isEmpty()|| title.isEmpty()){
        QString message(tr("please provide both the name of the artist"
                           "and the title of the album."));
        QMessageBox::information(this, tr ("add album"), message);
    }else{
        int artistId = findArtistId(artist);
        int albumId = addNewAlbum(title,artistId);

        QStringList tracks;
        tracks = tracksEditor->text().split(',',QString::SkipEmptyParts);
        addTracks(albumId, tracks);

        increaseAlbumCount(indexofArtist(artist));
        accept();
    }
}
int Dialog::findArtistId(const QString &artist)
{
    QSqlTableModel *artistModel = model->relationModel(2);
    int row = 0;

    while (row < artistModel->rowCount()){
        QSqlRecord record = artistModel->record(row);
        if(record.value("artist")== artist)
            return record.value("id").toInt();
                else
                row++;
    }
    return addNewArtist(artist);
    }

int Dialog::addNewArtist(const QString &name)
{
    QSqlTableModel *artistModel = model->relationModel(2);
    QSqlRecord record;

    int id= generateAlbumId();

    QSqlField f1("id", QVariant::Int);
    QSqlField f2("artist",QVariant::Int);
    QSqlField f3("albumcount", QVariant::Int);






}
