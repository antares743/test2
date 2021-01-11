#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

class Dialog : public QDialog
{
 Q_OBJECT

 public:
 Dialog(QSqlRelationalTableModel *albums, QDomDocument details,
        QFile *output, QWidget *parent = nullptr);
};

#endif // DIALOG_H
