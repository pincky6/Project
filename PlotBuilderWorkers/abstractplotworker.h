#ifndef ABSTRACTPLOTWORKER_H
#define ABSTRACTPLOTWORKER_H

#include <QObject>
#include <QThread>

#include <memory>

class AbstractPlotWorker: public QObject
{
public:
    Q_OBJECT
public:
    AbstractPlotWorker(QObject* parent = nullptr);
    virtual void work() = 0;
    virtual void setRunning(bool);
    virtual void wait();
    virtual void stopThread();
    virtual ~AbstractPlotWorker();
signals:
    void finished();
public slots:
    void whileRun();
    virtual void quit();
    void run();
protected:
    bool flag_;
    std::unique_ptr<QThread> thread_;
};

#endif // ABSTRACTPLOTWORKER_H
