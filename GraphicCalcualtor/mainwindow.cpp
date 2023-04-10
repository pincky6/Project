#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Calculator/recursivedescent.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMenuForButton(ui->pushButton_sinFunctions, {"sin", "asin", "sinh", "asinh"}, &MainWindow::functionButtonMenuClicked);
    createMenuForButton(ui->pushButton_cosFunctions, {"cos", "acos", "cosh", "acosh"}, &MainWindow::functionButtonMenuClicked);
    createMenuForButton(ui->pushButton_tanFunctions, {"tan", "atan", "tanh", "atanh"},  &MainWindow::functionButtonMenuClicked);
    createMenuForButton(ui->pushButton_cotFunctions, {"cot", "acot", "coth", "acoth"},  &MainWindow::functionButtonMenuClicked);
    createMenuForButton(ui->pushButton_logFunctions, {"log", "ln", "lg"},  &MainWindow::functionButtonMenuClicked);
    createMenuForButton(ui->pushButton_var, {"x", "y", "z"},  &MainWindow::menuButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClicked()
{
    if(ui->stackedWidget_2->currentIndex() == 0)
        ui->textEdit->setText(ui->textEdit->toPlainText() + ((QPushButton*)sender())->text());
    else
        ui->systemWidget->addTextAtActiveRow(((QPushButton*)sender())->text());
}

void MainWindow::menuButtonClicked()
{
    if(ui->stackedWidget_2->currentIndex() == 0)
        ui->textEdit->setText(ui->textEdit->toPlainText() + ((QAction*)sender())->text());
    else
        ui->systemWidget->addTextAtActiveRow(((QAction*)sender())->text());
}

void MainWindow::functionButtonClicked()
{
    if(ui->stackedWidget_2->currentIndex() == 0)
        ui->textEdit->setText(ui->textEdit->toPlainText() + ((QPushButton*)sender())->text() + "(");
    else
        ui->systemWidget->addTextAtActiveRow(((QPushButton*)sender())->text() + "(");
}

void MainWindow::functionButtonMenuClicked()
{
    QTextEdit& textEdit = *ui->textEdit;
    QString str = (((QAction*)sender())->text() == "log") ?  ((QAction*)sender())->text() + "()("
                                                           : ((QAction*)sender())->text() + "(";
    if(ui->stackedWidget_2->currentIndex() == 0)
        textEdit.setText(textEdit.toPlainText() + str);
    else
        ui->systemWidget->addTextAtActiveRow(str);
}

void MainWindow::on_swapLayoutButton_clicked()
{
    int currentIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(!currentIndex);
    switch (currentIndex)
    {
    case 0:
    {
        ui->swapLayoutButton->setText("123...");
        break;
    }
    case 1:
    {
        ui->swapLayoutButton->setText("func");
        break;
    }
    }
}

void MainWindow::createMenuForButton(QPushButton *button, const std::initializer_list<QString>& initList, void(MainWindow::*mainWindowSlot)())
{
    QMenu *buttonMenu = new QMenu;
    for(auto it = initList.begin(); it != initList.end(); it++)
    {
        buttonMenu->addAction(*it, this, mainWindowSlot);
    }
    button->setMenu(buttonMenu);
}


void MainWindow::on_pushButton_CE_clicked()
{
    ui->textEdit->setText("");
}


void MainWindow::on_pushButton_C_clicked()
{
    QString inputFieldString = ui->textEdit->toPlainText();
    inputFieldString.erase(inputFieldString.end() - 1, inputFieldString.end());
    ui->textEdit->setText(inputFieldString);
}


void MainWindow::on_pushButton_system_clicked()
{
    if(ui->stackedWidget_2->currentIndex() == 1)
    {
        ui->systemWidget->removeRows();
        ui->pushButton_system->setText("{");
        ui->stackedWidget_2->setCurrentIndex(0);
        return;
    }
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter number of system columns"), tr("Number of system columns:"), QLineEdit::Normal, "", &ok);
    if(!ok || text.isEmpty())
    {
        return;
    }
    int numOfRows = text.toInt(&ok);
    if(!ok || numOfRows <= 0)
    {
       QMessageBox::warning(this, "Error", "Enter number above zero");
       return;
    }
    ui->systemWidget->setRows(numOfRows);
    ui->pushButton_system->setText("y=f(x)");
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_pushButton_tab_clicked()
{
    if(ui->stackedWidget_2->currentIndex() == 0)
    {
        return;
    }
    ui->systemWidget->setNextActiveRow();
}

void MainWindow::on_pushButton_equal_clicked()
{
    RecursiveDecsent recursiveDecsent;
    try{
    Operand answer = recursiveDecsent.calculate(ui->textEdit->toPlainText());
    ui->textEdit->setText(QString::fromStdString(answer.toString()));
    }
    catch(const std::exception& exception)
    {
        QMessageBox::warning(this, "Error", exception.what());
    }
}


void MainWindow::on_pushButton_graph_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
    std::vector<QString> expressionsVector;
    expressionsVector.push_back(ui->textEdit->toPlainText());
    ui->graphicArea->setExpressions(expressionsVector);
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

