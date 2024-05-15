
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Database/recordtype.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class CalculatorArea
    {
        CALCULATOR_AREA = 0,
        PLOT_AREA,
        HISTORY_AREA
    };
    enum class KeyBoardArea
    {
        ARITHMETIC_AREA = 0,
        FUNCTION_AREA
    };

    enum class PlotAreas
    {
        PLOT_AREA_2D = 0,
        PLOT_AREA_3D
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void buttonClicked();
    void menuButtonClicked();
    void functionButtonClicked();
    void functionButtonMenuClicked();

    void on_pushButton_CE_clicked();
    void on_pushButton_C_clicked();

    void on_swapLayoutButton_clicked();
    void on_pushButton_system_clicked();

    void on_pushButton_tab_clicked();

    void on_pushButton_equal_clicked();

    void on_pushButton_graph_clicked();

    void on_pushButton_clicked();

    void on_backButton_clicked();

    void on_2DModeChanged();
    void on_3DModeChanged();


    void on_generateSTLButton_clicked();

    void on_historyButton_clicked();

private:
    void createMenuForButton(QPushButton*, const std::initializer_list<QString>&, void(MainWindow::*)());

public slots:
    void historySwitch(const QString&, record::type::RecordType);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
