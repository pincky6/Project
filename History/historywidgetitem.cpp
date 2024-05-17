#include "historywidgetitem.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QFrame>

#include <QStyleOptionDockWidget>

HistoryWidgetItem::HistoryWidgetItem(const Record& record_, QListWidget* parent): QListWidgetItem(parent), record(record_)
{
    QWidget* widget = new QWidget();
    QLayout* layout = new QHBoxLayout;

    setLabel(textLabel, record.expression, layout);
    setSeparator(layout);
    setLabel(typeTextLabel,
             record::type::toStringRepresentation(record.type),
             layout);
    setRemoveButton(layout);

    widget->setLayout(layout);
    this->setSizeHint(widget->sizeHint());
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

void HistoryWidgetItem::setLabel(QLabel* label,
                                 const QString& text,
                                 QLayout* layout)
{
    label = new QLabel();
    label->setText(text);
    layout->addWidget(label);
}

void HistoryWidgetItem::setSeparator(QLayout* layout)
{
    auto separator = new QFrame;
    //    separator->setFixedHeight(3);
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(1);
    layout->addWidget(separator);
}

void HistoryWidgetItem::setRemoveButton(QLayout* layout)
{
    QStyleOptionDockWidget opt;
    removeButton = new QPushButton();
    removeButton->setIcon(removeButton->style()->standardIcon(QStyle::SP_TitleBarCloseButton, &opt));
    removeButton->setFixedSize(25, 25);
    QObject::connect(removeButton, &QPushButton::clicked, this, &HistoryWidgetItem::itemClicked);
    layout->addWidget(removeButton);
}

void HistoryWidgetItem::itemClicked()
{
    emit removeButtonClicked(this);
}
