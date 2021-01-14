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
    query.exec("insert to albums values(2,'A temporary Drive ',1,2005)");
    query.exec("insert into albums values(3,'...The great october sound',2,"
                                          "2002)");
    query.exec("insert into albums values(4,'stray dogs',2,2003)");
    query.exec("insert into albums values(5"
               "'one day you'll dance for me ,new york city',2,2004)");
    query.exec("insert into albums values(6,'Ompa til du D\xc3\xb8r',3,2001)");
    query.exec("insert into albums values(7,'evrig pnt',3,2002)");
    query.exec("insert into albums values(8,'Maestro',3,2005)");

    return true;
}
