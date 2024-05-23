#ifndef PLOTAREA3D_H
#define PLOTAREA3D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QMutex>

#include "PlotBuilderWorkers/plotscheduler.h"
#include "PlotBuilderWorkers/stlmodelscheduler.h"

#include "abstractplotarea.h"

#include <QSharedPointer>
#include <QMap>
#include <QMatrix4x4>

class PlotArea3D final: public QOpenGLWidget, public QOpenGLFunctions, public AbstractPlotArea
{
    Q_OBJECT

public:
    PlotArea3D(QWidget *parent = 0);
    void setExpressions(const std::vector<QString>&) override;
    void resetPlot(QSharedPointer<std::vector<Vertex>>,
                   QSharedPointer<std::vector<unsigned int>>,
                   const QString&);
    void destroyPlotBuffers();
    void destroyPlotBuffer(const QString&);
    void freeSchedulers();

    void setAxesVisibilityeMode(bool);

    void loadToSTL(const QString&);
    ~PlotArea3D();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


    void paintAxes();
    void paintPlot();


    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
private slots:
    void receiveData(QSharedPointer<std::vector<Vertex>>,
                     QSharedPointer<std::vector<unsigned int>>,
                     QString);
signals:
    void stopThread();
private:
    void initShaders();
    void initPlotBuilder(float length, const QString&);
    void initAxes(float length);
private:
    QMatrix4x4 projectionMatrix_;
    QOpenGLShaderProgram shaderProgram_;
    QMap<QString, QOpenGLBuffer> arrayPlotBuffers_;
    QMap<QString, QOpenGLBuffer> indexPlotBuffers_;

    QOpenGLBuffer arrayAxesBuffers_[3];
    QOpenGLBuffer indexAxesBuffers_[3];
    QVector2D mousePosition_;
    QQuaternion rotation_;

    QMap<QString, QSharedPointer<plot_builder::PlotScheduler>> schedulers_;
    plot_builder::STLModelScheduler modelScheduler_;

    float defaultLength_;
    float scaleFactor_;
    float maxScaleFactor_;
    float mZ_;

    bool isAxesVisible;

    std::vector<QString> expressionsVector_;
};
#endif // PLOTAREA3D_H
