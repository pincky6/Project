#include "mainwindow.h"

#include <QApplication>
#include <QtSql/QSqlDatabase>
#include "Database/plottable2d.h"
#include "Database/plottable3d.h"

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
    if(plotTable2D.create())
    {
        qDebug() << "Table plot 2d didn't created";
    }
    if(plotTable3D.create())
    {
        qDebug() << "Table plot 3d didn't created";
    }
    MainWindow w;
    w.show();
    return a.exec();
}
