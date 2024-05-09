#include "mainwindow.h"

#include <QApplication>
#include <QtSql/QSqlDatabase>
#include "Database/calculationstable.h"
#include "Database/plottable2d.h"
#include "Database/plottable3d.h"
#include "Database/typetable.h"
#include "Database/recordtable.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db =
        QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database");
    if(!db.open()){
        return -1;
    }
    PlotTable2D plotTable2D;
    PlotTable3D plotTable3D;
    TypeTable typeTable;
    CalculationsTable calculationsTable;
    RecordTable recordTable;
    if(!typeTable.create())
    {
        qDebug() << "Table type didn't created";
    }
    if(!recordTable.create())
    {
        qDebug() << "Table records didn't created";
    }
    if(!plotTable2D.create())
    {
        qDebug() << "Table plot 2d didn't created";
    }
    if(!plotTable3D.create())
    {
        qDebug() << "Table plot 3d didn't created";
    }
    if(!calculationsTable.create())
    {
        qDebug() << "Table calculations history didn't created";
    }
    MainWindow w;
    w.show();
    return a.exec();
}
