#ifndef ABSTRACTPLOTBUILDER_H
#define ABSTRACTPLOTBUILDER_H

#include <QObject>
#include <QThread>

class AbstractPlotBuilder: public QObject
{
public:
    Q_OBJECT
public:
    AbstractPlotBuilder(QObject* parent = nullptr);
    virtual void work() = 0;
    void setRunning(bool);
    void wait();
    void stopThread();
    virtual ~AbstractPlotBuilder();
signals:
    void finished();
public slots:
    void whileRun();
    void run();
protected:
    bool flag;
    QThread thread;
};

#endif // ABSTRACTPLOTBUILDER_H
