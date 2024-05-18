#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();
signals:
    void backButtonClicked();
public slots:
    void on_backButton_clicked();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
