
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class CalculatorArea
    {
        CALCULATOR_AREA,
        PLOT_AREA
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

private:
    void createMenuForButton(QPushButton*, const std::initializer_list<QString>&, void(MainWindow::*)());
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
