#ifndef HISTORYWIDGETITEM_H
#define HISTORYWIDGETITEM_H

#include <QListWidgetItem>
#include <QObject>
#include "Database/recordtable.h"

class QPushButton;
class QLabel;

class HistoryWidgetItem: public QObject, public QListWidgetItem
{
    Q_OBJECT
public:
    HistoryWidgetItem(const Record&, QListWidget* parent = nullptr);

    record::type::RecordType getType();
    record::type::RecordType getType() const;

    QString& getExpression();
    const QString& getExpression() const;
public slots:
    void itemClicked();
signals:
    void removeButtonClicked(HistoryWidgetItem*);
private:
    QPushButton* removeButton;
    QLabel* textLabel;

    Record record;
};

#endif // HISTORYWIDGETITEM_H
