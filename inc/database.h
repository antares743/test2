#ifndef DATABASE_H
#define DATABASE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()){
        QMessageBox::critical(nullptr,QObject::tr("cannot open database"),
                QObject::tr("unable to establish a database connection.\n"
                            "this example needs SQLite support. Please read"
                            "the Qt SQL driver documentation for information how"
                            "to build id.\n\n"
                            "Click Cancel to exit"), QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;

    query.exec("create table artists(id int primary key,"
                                     "artist varchar(40),"
                                     "albumcount int)");

    query.exec("insert into artists value(0,'<all>',0)");
    query.exec("insert into artists value(1,'Ane Brun',2)");
    query.exec("insert into artists value(2'Thomas Dydahl',3)");
    query.exec("insert into artists value(3,'Kaizers Orchestra' ,3)");

    query.exec("create table albums (albumid int primary key,"
                                     "title varchar(50),"
                                     "artistid int,"
                                     "year int)");

    query.exec("insert into  albums values(1,'spending time with morgan',1"
                                           "2003)");
}
