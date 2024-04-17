#ifndef PLOTAREA3D_H
#define PLOTAREA3D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QMutex>

#include "PlotBuilderWorkers/plotscheduler.h"
#include <QMatrix4x4>

class PlotArea3D final: public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    PlotArea3D(QWidget *parent = 0);
    void setExpressions(const std::vector<QString>&);
    void resetPlot(std::shared_ptr<std::vector<Vertex>>,
                   std::shared_ptr<std::vector<unsigned int>>);
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
    void receiveData(std::shared_ptr<std::vector<Vertex>>,
                     std::shared_ptr<std::vector<unsigned int>>);
signals:
    void stopThread();
private:
    void initShaders();
    void initPlotBuilder(float length);
    void initAxes(float length);
private:
    QMatrix4x4 projectionMatrix_;
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLBuffer arrayPlotBuffer_;
    QOpenGLBuffer indexPlotBuffer_;

    QOpenGLBuffer arrayAxesBuffers_[3];
    QOpenGLBuffer indexAxesBuffers_[3];
    QVector2D mousePosition_;
    QQuaternion rotation_;

    plot_builder::PlotScheduler scheduler_;

    float defaultLength_;
    float scaleFactor_;
    float maxScaleFactor_;
    float mZ_;

    std::vector<QString> expressionsVector_;
};
#endif // PLOTAREA3D_H
