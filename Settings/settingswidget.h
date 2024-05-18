#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class SettingsWidget;
}

class QLineEdit;
class SettingsModel;

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    SettingsModel getSettingsModel();
    void update();
    ~SettingsWidget();
private:
    std::list<QLineEdit*> getEmptyLineEdits();
    void changeBorderColorToRed(const std::list<QLineEdit*>&);
signals:
    void backButtonClicked();
public slots:
    void on_backButton_clicked();

private slots:
    void on_applyButton_clicked();
    void lineEdit_changed();
    void on_save_3d_checkbox_toggled(bool checked);

    void on_save_2d_checkbox_toggled(bool checked);

    void on_clearButton_clicked();

    void on_exportButton_clicked();

    void on_importButton_clicked();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
