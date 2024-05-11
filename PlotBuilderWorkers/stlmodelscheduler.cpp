#include "stlmodelscheduler.h"

#include <QProgressDialog>

plot_builder::STLModelScheduler::STLModelScheduler(QObject *parent): AbstractPlotWorker(parent)
{
    connect(&(*thread_), &QThread::started, this, &STLModelScheduler::whileRun);
    connect(this, &STLModelScheduler::finished, thread_.get(), &QThread::quit);
    moveToThread(&(*thread_));
    setRunning(true);
}

void plot_builder::STLModelScheduler::work()
{
    if(tasks.size() == 0)
    {
        return;
    }
    auto task = std::move(tasks.front());
    tasks.pop_front();
    task->start();
    task->wait();
}

void plot_builder::STLModelScheduler::addTask(std::unique_ptr<STLModelBuilder>&& newBuilder, QWidget* widget)
{
    STLModelBuilder& builder = *newBuilder;
    QProgressDialog* progressDialog = new QProgressDialog(widget);


    progressDialog->setLabelText("Генерация модели...");
    QObject::connect(&builder, &STLModelBuilder::endCalculations, widget, [progressDialog](){
        progressDialog->setValue(75);
        progressDialog->setLabelText("Сохранение модели...");
    });
    QObject::connect(&builder, &STLModelBuilder::endFileWriting, widget, [progressDialog](){
        progressDialog->setValue(100);
        progressDialog->setLabelText("Модель сгенерирована!");
    });
    QObject::connect(progressDialog, &QProgressDialog::canceled, &builder, &STLModelBuilder::quit);
    QObject::connect(&builder, &STLModelBuilder::wrongCalculations, widget, [progressDialog](){
        progressDialog->cancel();
    });
    builder.connect();
    QObject::connect(&builder.getPlotCalculator(), &XYZPlotCalculator::sendProgress, widget, [progressDialog](float progress){
        qDebug() << progress;
        progressDialog->setValue(static_cast<int>(progress * 75));
    });

    progressDialog->setMinimum(0);
    progressDialog->setMaximum(100);
    progressDialog->setValue(0);
    progressDialog->show();

    tasks.push_back(std::move(newBuilder));
}
//(x^2+(9/4)*y^2+z^2-1)^3-(x^2)*(z^3)-(9/80)*(y^2)*(z^3)
void plot_builder::STLModelScheduler::wait()
{
    stopThread();
    thread_->wait();
}

void plot_builder::STLModelScheduler::start()
{
    thread_->start();
}

void plot_builder::STLModelScheduler::freeQueue()
{
    tasks.clear();
}
