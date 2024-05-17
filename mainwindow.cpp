#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Database/calculationstable.h"

#include "Calculator/recursivedescent.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->historyWidget, &HistoryWidget::doubleClicked, this, &MainWindow::historySwitch);
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

void MainWindow::plotModeChanged(QAction* activeAction,
                                 QAction* disableAction,
                                 AbstractPlotArea* plotArea,
                                 PlotAreas plotAreaEnum)
{
    std::vector<QString> expressionsVector;
    expressionsVector.push_back(ui->textEdit->toPlainText());
    if(!activeAction->isChecked() && !disableAction->isChecked())
    {
        activeAction->setChecked(true);
    }
    if(activeAction->isChecked())
    {
        disableAction->setChecked(false);
        if(ui->stackedWidget_3->currentIndex() != (int)CalculatorArea::CALCULATOR_AREA)
        {
            plotArea->setExpressions(expressionsVector);
            ui->stackedWidget_4->setCurrentIndex((int)plotAreaEnum);
        }
    }
}

void MainWindow::changeAreas()
{
    std::vector<QString> expressionsVector;
    expressionsVector.push_back(ui->textEdit->toPlainText());

    if(ui->action2D->isChecked())
    {
        ui->stackedWidget_4->setCurrentIndex((int)PlotAreas::PLOT_AREA_2D);
        ui->graphicArea->setExpressions(expressionsVector);
    }
    else
    {
        ui->stackedWidget_4->setCurrentIndex((int)PlotAreas::PLOT_AREA_3D);
        ui->graphicArea3D->setExpressions(expressionsVector);
    }
    ui->stackedWidget_3->setCurrentIndex((int)CalculatorArea::PLOT_AREA);
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
    case (int)KeyBoardArea::ARITHMETIC_AREA:
    {
        ui->swapLayoutButton->setText("123...");
        break;
    }
    case (int)KeyBoardArea::FUNCTION_AREA:
    {
        ui->swapLayoutButton->setText("func");
        break;
    }
    }
}

void MainWindow::createMenuForButton(QPushButton *button,
                                     const std::initializer_list<QString>& initList,
                                     void(MainWindow::*mainWindowSlot)())
{
    QMenu *buttonMenu = new QMenu;
    for(auto it = initList.begin(); it != initList.end(); it++)
    {
        buttonMenu->addAction(*it, this, mainWindowSlot);
    }
    button->setMenu(buttonMenu);
}

void MainWindow::historySwitch(QString expression,
                               record::type::RecordType type)
{
    ui->textEdit->setText(expression);
    if(type == record::type::RecordType::EXPRESSION)
    {
        return;
    }
    (type == record::type::RecordType::PLOT2D) ?
        ui->action2D->setChecked(true) :
        ui->action3D->setChecked(true);
    changeAreas();
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
    QString text = QInputDialog::getText(this,
                                         tr("Enter number of system columns"),
                                         tr("Number of system columns:"),
                                         QLineEdit::Normal, "", &ok);
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
        CalculationsTable table;
        CalculationResult calculationResult;
        calculationResult.expression = ui->textEdit->toPlainText();
        Operand answer = recursiveDecsent.calculate(ui->textEdit->toPlainText());
        calculationResult.result = QString::fromStdString(answer.toString());
        ui->textEdit->setText(calculationResult.result);
        if(table.existExpression(calculationResult.result))
        {
            table.update(calculationResult);
        }
        else
        {
            table.insert(calculationResult);
        }
    }
    catch(const std::exception& exception)
    {
        QMessageBox::warning(this, "Error", exception.what());
    }
}


void MainWindow::on_pushButton_graph_clicked()
{
    changeAreas();
//    std::vector<QString> expressionsVector;
//    expressionsVector.push_back(ui->textEdit->toPlainText());

//    if(ui->action2D->isChecked())
//    {
//        ui->stackedWidget_4->setCurrentIndex((int)PlotAreas::PLOT_AREA_2D);
//        ui->graphicArea->setExpressions(expressionsVector);
//    }
//    else
//    {
//        ui->stackedWidget_4->setCurrentIndex((int)PlotAreas::PLOT_AREA_3D);
//        ui->graphicArea3D->setExpressions(expressionsVector);
//    }
//    ui->stackedWidget_3->setCurrentIndex((int)CalculatorArea::PLOT_AREA);
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget_3->setCurrentIndex((int)CalculatorArea::CALCULATOR_AREA);
}


void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget_3->setCurrentIndex((int)CalculatorArea::CALCULATOR_AREA);
    if(ui->stackedWidget_4->currentIndex() == (int)PlotAreas::PLOT_AREA_3D)
    {
        ui->graphicArea3D->destroyPlotBuffer();
        ui->graphicArea3D->freeSchedulers();
    }
}

void MainWindow::on_2DModeChanged()
{
    plotModeChanged(ui->action2D, ui->action3D, ui->graphicArea, PlotAreas::PLOT_AREA_2D);
//    std::vector<QString> expressionsVector;
//    expressionsVector.push_back(ui->textEdit->toPlainText());
//    if(ui->action2D->isChecked())
//    {
//        ui->action3D->setChecked(false);

//        if(ui->stackedWidget_3->currentIndex() != (int)CalculatorArea::CALCULATOR_AREA)
//        {
//            ui->graphicArea->setExpressions(expressionsVector);
//            ui->stackedWidget_4->setCurrentIndex((int)PlotAreas::PLOT_AREA_2D);
//        }
//    }
//    if(!ui->action2D->isChecked())
//    {
//        ui->action2D->setChecked(true);
//    }
}

void MainWindow::on_3DModeChanged()
{
    plotModeChanged(ui->action3D, ui->action2D, ui->graphicArea3D, PlotAreas::PLOT_AREA_3D);
//    std::vector<QString> expressionsVector;
//    expressionsVector.push_back(ui->textEdit->toPlainText());
//    if(ui->action3D->isChecked())
//    {
//        ui->action2D->setChecked(false);
//        if(ui->stackedWidget_3->currentIndex() != (int)CalculatorArea::CALCULATOR_AREA)
//        {
//            ui->graphicArea3D->setExpressions(expressionsVector);
//            ui->stackedWidget_4->setCurrentIndex((int)PlotAreas::PLOT_AREA_3D);
//        }
//    }
//    if(!ui->action3D->isChecked())
//    {
//        ui->action3D->setChecked(true);
//    }
}


void MainWindow::on_generateSTLButton_clicked()
{
    QString userInput = QInputDialog::getText(this, "Введите название файла", "Название файла:", QLineEdit::Normal, "");
    if(userInput.isEmpty()){
        userInput = "3dModel.stl";
    } else {
        userInput += ".stl";
    }
    ui->graphicArea3D->loadToSTL(userInput);
}


void MainWindow::on_historyButton_clicked()
{
    ui->historyWidget->update();
    ui->stackedWidget_3->setCurrentIndex((int)CalculatorArea::HISTORY_AREA);
}

