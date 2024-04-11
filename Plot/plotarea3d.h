#ifndef PLOTAREA3D_H
#define PLOTAREA3D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QMutex>

#include "PlotBuilderWorkers/xyzplotbuilder.h"

#include <QMatrix4x4>

#include "axis.h"

class PlotArea3D final: public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    PlotArea3D(QWidget *parent = 0);
    void setExpressions(const std::vector<QString>&);
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
    void receiveData();
signals:
    void stopThread();
private:
    void initShaders();
    void initPlot(float length);
    void initAxes(float length);
private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLBuffer m_arrayPlotBuffer;
    QOpenGLBuffer m_indexPlotBuffer;

    QOpenGLBuffer m_arrayAxesBuffers[3];
    QOpenGLBuffer m_indexAxesBuffers[3];
    QVector2D m_mousePosition;
    QQuaternion m_rotation;

    QMutex mutex_;
    std::unique_ptr<plot_builder::XYZPlotBuilder> builder;

    float defaultLength;
    float scaleFactor;
    float maxScaleFactor;
    float m_z;

    std::vector<QString> expressionsVector;
};
#endif // PLOTAREA3D_H
