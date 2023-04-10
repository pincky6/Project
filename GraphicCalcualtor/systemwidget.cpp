#include "systemwidget.h"
#include "ui_systemwidget.h"
#include <QDebug>
#include<QLineEdit>

SystemWidget::SystemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemWidget),
    maxNumOfRows(15),
    activeRow(0)
{
    ui->setupUi(this);
}

void SystemWidget::setRows(const std::size_t & numOfRows_)
{
    if(numOfRows_ > maxNumOfRows)
    {
        throw std::range_error("Max number of rows is 15");
    }
    numOfRows = numOfRows_;
    activeRow = 0;
    for(std::size_t i = 0; i < numOfRows; i++)
    {
        QLineEdit* lineEdit = new QLineEdit;
        if(i == 0)
            lineEdit->setStyleSheet(QString("QLineEdit {border-width: 1px; border-style: solid; border-color: blue;}"));
        else
            lineEdit->setStyleSheet(QString("QLineEdit {border-width: 1px; border-style: solid; border-color: black;}"));
        ui->verticalLayout_3->addWidget(lineEdit);
        rows.push_back(lineEdit);
    }
}

QVector<QString> SystemWidget::getExpressionsOfSystem()
{
    QVector<QString> vectorOfExpressions(numOfRows);
    for(std::size_t i = 0; i < numOfRows; i++)
    {
        vectorOfExpressions.push_back(rows[i]->text());
    }
    return vectorOfExpressions ;
}

void SystemWidget::removeRows()
{
    QLayout* layout = ui->verticalLayout_3->layout();
    for(std::size_t i = 0; i < numOfRows; i++)
    {
        QLayoutItem* item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
    rows.clear();
}

void SystemWidget::setNextActiveRow()
{
    std::size_t oldValue = activeRow;
    ++activeRow;
    if(activeRow >= numOfRows){
        activeRow = 0;
    }
    rows[oldValue]->setStyleSheet(QString("QLineEdit {border-width: 1px; border-style: solid; border-color: black;}"));
    rows[activeRow]->setStyleSheet(QString("QLineEdit {border-width: 1px; border-style: solid; border-color: blue;}"));
}

void SystemWidget::addTextAtActiveRow(const QString& str)
{
    rows[activeRow]->setText(rows[activeRow]->text() + str);
}

SystemWidget::~SystemWidget()
{
    delete ui;
}
