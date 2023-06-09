#ifndef SYSTEMWIDGET_H
#define SYSTEMWIDGET_H

#include <QWidget>

class QLineEdit;

namespace Ui {
class SystemWidget;
}

class SystemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemWidget(QWidget *parent = nullptr);

    void setRows(const size_t& i);
    QVector<QString> getExpressionsOfSystem();
    void removeRows();

    void setNextActiveRow();
    void addTextAtActiveRow(const QString&);

    ~SystemWidget();
private:
    Ui::SystemWidget *ui;
private:
    std::size_t numOfRows;
    std::size_t maxNumOfRows;
    std::size_t activeRow;
    QVector<QLineEdit*> rows;
};

#endif // SYSTEMWIDGET_H
