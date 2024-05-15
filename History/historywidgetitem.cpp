#include "historywidgetitem.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

#include <QStyleOptionDockWidget>

HistoryWidgetItem::HistoryWidgetItem(const Record& record_, QListWidget* parent): QListWidgetItem(parent), record(record_)
{
    QWidget* widget = new QWidget();
    QLayout* layout = new QHBoxLayout;
    QStyleOptionDockWidget opt;
    removeButton = new QPushButton();
    removeButton->setIcon(removeButton->style()->standardIcon(QStyle::SP_TitleBarCloseButton, &opt));
    removeButton->setFixedSize(25, 25);

    textLabel = new QLabel();
    textLabel->setText(record.expression);

    layout->addWidget(textLabel);
    layout->addWidget(removeButton);
    widget->setLayout(layout);
    this->setSizeHint(widget->sizeHint());
    QObject::connect(removeButton, &QPushButton::clicked, this, &HistoryWidgetItem::itemClicked);
    parent->setItemWidget(this, widget);

}

record::type::RecordType HistoryWidgetItem::getType()
{
    return record.type;
}

record::type::RecordType HistoryWidgetItem::getType() const
{
    return record.type;
}

QString& HistoryWidgetItem::getExpression()
{
    return record.expression;
}

const QString& HistoryWidgetItem::getExpression() const
{
    return record.expression;
}

void HistoryWidgetItem::itemClicked()
{
    emit removeButtonClicked(this);
}
