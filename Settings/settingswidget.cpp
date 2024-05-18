#include "settingswidget.h"
#include "ui_settingswidget.h"

#include "settingsmodel.h"
#include "calculatorsettingsconfig.h"

#include "Database/settingstable.h"
#include "Database/recordtable.h"
#include "Database/plottable2d.h"
#include "Database/plottable3d.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include <QIntValidator>

#include <limits.h>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    ui->x_resolution_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    ui->y_resolution_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    ui->z_resolution_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));

    ui->width_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    ui->length_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    ui->height_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));

    ui->history_2d_records_count_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
    ui->history_3d_records_count_lineedit->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), this));
}

SettingsModel SettingsWidget::getSettingsModel()
{
    return SettingsModel(
        ui->x_resolution_lineedit->text().toInt(),
        ui->y_resolution_lineedit->text().toInt(),
        ui->z_resolution_lineedit->text().toInt(),
        ui->width_lineedit->text().toInt(),
        ui->length_lineedit->text().toInt(),
        ui->height_lineedit->text().toInt(),
        ui->history_2d_records_count_lineedit->text().isEmpty() ?
            std::numeric_limits<int>::max() : ui->history_2d_records_count_lineedit->text().toInt(),
        ui->history_3d_records_count_lineedit->text().isEmpty() ?
            std::numeric_limits<int>::max() : ui->history_3d_records_count_lineedit->text().toInt()
        );
}

void SettingsWidget::update()
{
    SettingsTable table;
    SettingsModel model = table.select();
    ui->x_resolution_lineedit->setText(QString::number(model.resolutionX));
    ui->y_resolution_lineedit->setText(QString::number(model.resolutionY));
    ui->z_resolution_lineedit->setText(QString::number(model.resolutionZ));

    ui->width_lineedit->setText(QString::number(model.modelWidth));
    ui->length_lineedit->setText(QString::number(model.modelLength));
    ui->height_lineedit->setText(QString::number(model.modelHeight));

    if(model.model2DRecordsCount != std::numeric_limits<int>::max())
    {
        ui->history_2d_records_count_lineedit->setText(QString::number((model.model2DRecordsCount)));
        ui->save_2d_checkbox->setChecked(false);
    }
    else
    {
        ui->history_2d_records_count_lineedit->setText("");
        ui->save_2d_checkbox->setChecked(true);
    }
    if(model.model3DRecordsCount != std::numeric_limits<int>::max())
    {
        ui->history_3d_records_count_lineedit->setText(QString::number(model.model3DRecordsCount));
        ui->save_3d_checkbox->setChecked(false);
    }
    else
    {
        ui->history_3d_records_count_lineedit->setText("");
        ui->save_3d_checkbox->setChecked(true);
    }
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

std::list<QLineEdit*> SettingsWidget::getEmptyLineEdits()
{
    auto edits = {
                  ui->x_resolution_lineedit,
                  ui->y_resolution_lineedit,
                  ui->z_resolution_lineedit,
                  ui->width_lineedit,
                  ui->length_lineedit,
                  ui->height_lineedit
                };
    std::list<QLineEdit*> wrongLineEdits;
    for(auto&& edit: edits)
    {
        if(edit->text().isEmpty())
        {
            wrongLineEdits.push_back(edit);
        }
    }
    if(!ui->save_2d_checkbox->isChecked() && ui->history_2d_records_count_lineedit->text().isEmpty())
    {
        wrongLineEdits.push_back(ui->history_2d_records_count_lineedit);
    }
    if(!ui->save_3d_checkbox->isChecked() && ui->history_3d_records_count_lineedit->text().isEmpty())
    {
        wrongLineEdits.push_back(ui->history_3d_records_count_lineedit);
    }
    return wrongLineEdits;
}

void SettingsWidget::changeBorderColorToRed(const std::list<QLineEdit*>& wrongLineEdits)
{
    for(auto&& lineEdit: wrongLineEdits)
    {
        if(lineEdit->text().isEmpty())
        {
            lineEdit->setStyleSheet("border: 1px solid red");
        }
    }
}

void SettingsWidget::on_backButton_clicked()
{
    emit backButtonClicked();
}


void SettingsWidget::on_applyButton_clicked()
{
    if(auto&& wrongLineEdits = getEmptyLineEdits();
        !wrongLineEdits.empty())
    {
        changeBorderColorToRed(wrongLineEdits);
        QMessageBox::critical(this, "Error", "Some input fields are empty. \nPlease fill them");
        return;
    }
    SettingsModel model = getSettingsModel();
    if(PlotTable2D plotTable2D; model.model2DRecordsCount < plotTable2D.getRowCount())
    {
        auto answer = QMessageBox::warning(this, "Warning",
                              "Plot2D history overflowed\nDo you want to clean it?",
                              QMessageBox::Ok | QMessageBox::Cancel);
        if(answer == QMessageBox::Ok)
        {
            int deleteCount = plotTable2D.getRowCount() - model.model2DRecordsCount - 1;
            if(deleteCount < 0)
            {
                deleteCount = 0;
            }
            plotTable2D.drop(deleteCount);
        }
        else
        {
            return;
        }
    }
    if(PlotTable3D plotTable3D; model.model3DRecordsCount <= plotTable3D.getRowCount())
    {
        auto answer = QMessageBox::warning(this, "Warning",
                              "Plot3D history overflowed\nDo you want to clean it?",
                              QMessageBox::Ok | QMessageBox::Cancel);
        if(answer == QMessageBox::Ok)
        {
            int deleteCount = plotTable3D.getRowCount() - model.model2DRecordsCount - 1;
            if(deleteCount < 0)
            {
                deleteCount = 0;
            }
            plotTable3D.drop(deleteCount);
        }
        else
        {
            return;
        }
    }
    SettingsTable table;
    table.insertOrUpdate(model);
}

void SettingsWidget::lineEdit_changed()
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(sender());
    lineEdit->setStyleSheet("");
}


void SettingsWidget::on_save_3d_checkbox_toggled(bool checked)
{
    ui->history_3d_records_count_lineedit->setEnabled(!checked);
    if(checked)
    {
        ui->history_3d_records_count_lineedit->setStyleSheet("");
        ui->history_3d_records_count_lineedit->setText("");
    }
}


void SettingsWidget::on_save_2d_checkbox_toggled(bool checked)
{
    ui->history_2d_records_count_lineedit->setEnabled(!checked);
    if(checked)
    {
        ui->history_2d_records_count_lineedit->setStyleSheet("");
        ui->history_2d_records_count_lineedit->setText("");
    }
}




void SettingsWidget::on_clearButton_clicked()
{
    RecordTable table;
    table.clearTable();
}


void SettingsWidget::on_exportButton_clicked()
{
    bool checkOk = false;
    QString userInput = QInputDialog::getText(this, "Enter file name",
                                              "If you do not enter a filename, the default name ‘settings.json’ will be used.\n\n"
                                              "Filename:",
                                              QLineEdit::Normal, "", &checkOk);
    if(!checkOk)
    {
        return;
    }
    if(userInput.isEmpty())
    {
        userInput = "settings.json";
    } else
    {
        userInput += ".json";
    }
    try
    {
        CalculatorSettingsConfig::exportSettings(userInput, getSettingsModel());
    }
    catch (const std::logic_error& exception)
    {
        QMessageBox::warning(this, "Error", exception.what());
    }

}


void SettingsWidget::on_importButton_clicked()
{
    try
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Choose File", QDir::homePath(), "Json Files (*.json)");
        if(fileName.isEmpty())
        {
            return;
        }
        SettingsModel model = CalculatorSettingsConfig::importSettings(fileName);
        SettingsTable table;
        table.insertOrUpdate(model);
        update();
    }
    catch (const std::logic_error& exception)
    {
        QMessageBox::warning(this, "Error", exception.what());
    }
}

