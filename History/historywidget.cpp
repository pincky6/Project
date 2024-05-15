#include "historywidget.h"
#include "ui_historywidget.h"

#include "historywidgetitem.h"
#include "Database/recordtable.h"
#include "Database/tablefactory.h"

#include<QMessageBox>

#include <QStringList>
#include <QKeyEvent>

HistoryWidget::HistoryWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryWidget)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

void HistoryWidget::makeItem(const Record& record, QListWidget* listWidget)
{
    HistoryWidgetItem* historyWidget = new HistoryWidgetItem(record, listWidget);
    QObject::connect(historyWidget, &HistoryWidgetItem::removeButtonClicked, this, &HistoryWidget::removeItem);
    QObject::connect(listWidget, &QListWidget::itemDoubleClicked, this, &HistoryWidget::itemDoubleClicked, Qt::UniqueConnection);
}

void HistoryWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
        foreach(QListWidgetItem* item, selectedItems)
        {
            removeItem(dynamic_cast<HistoryWidgetItem*>(item));
        }
    }
}

void HistoryWidget::update()
{
    RecordTable recordTable;
    QVector<Record> records = recordTable.selectAll();
    foreach(const Record& record, records)
    {
        makeItem(record,  ui->listWidget );
    }
}

void HistoryWidget::removeItem(HistoryWidgetItem* item)
{
    auto table = TableFactory::produce(item->getType());
    table->removeByExpression(item->getExpression());
    delete item;
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}


void HistoryWidget::on_selectAllButton_clicked()
{
    ui->listWidget->selectAll();
}

void HistoryWidget::itemDoubleClicked(QListWidgetItem* item)
{
    HistoryWidgetItem* historyItem = dynamic_cast<HistoryWidgetItem*>(item);
    emit doubleClicked(historyItem->getExpression(),
                  historyItem->getType());
}

