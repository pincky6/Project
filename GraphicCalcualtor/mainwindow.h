
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

private:
    void createMenuForButton(QPushButton*, const std::initializer_list<QString>&, void(MainWindow::*)());
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
