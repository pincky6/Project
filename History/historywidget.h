#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class HistoryWidget; }
QT_END_NAMESPACE

class Record;
namespace record::type
{
    enum class RecordType;
}
class HistoryWidgetItem;

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:
    HistoryWidget(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void deleteAllItems();
    void update();
    ~HistoryWidget();
public slots:
    void removeItem(HistoryWidgetItem*);

private slots:
    void on_selectAllButton_clicked();
    void itemDoubleClicked(QListWidgetItem*);
    void on_backButton_clicked();

signals:
    void doubleClicked(QString, record::type::RecordType);
    void backButtonClicked();
private:
    void makeItem(const Record&, QListWidget* lstWgt );
private:
    Ui::HistoryWidget *ui;
};
#endif // HISTORYWIDGET_H
