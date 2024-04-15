#include "plotarea3d.h"

#include <QtOpenGL>
#include <QVector3D>

#include "axis.h"

#include "figures.h"

using namespace plot_builder;

PlotArea3D::PlotArea3D(QWidget *parent)
    : QOpenGLWidget(parent),
    indexPlotBuffer_(QOpenGLBuffer::IndexBuffer),
    indexAxesBuffers_{QOpenGLBuffer(QOpenGLBuffer::IndexBuffer),
                       QOpenGLBuffer(QOpenGLBuffer::IndexBuffer),
                       QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)},
    defaultLength_(20.0f), scaleFactor_(1.0f), mZ_(-5.0f)
{
    mZ_ = -5.0f;
    connect(&scheduler_, &PlotScheduler::updatePlot, this, &PlotArea3D::receiveData);
}

void PlotArea3D::setExpressions(const std::vector<QString>& newExpressionsVector)
{
    expressionsVector_ = newExpressionsVector;
    initPlotBuilder(defaultLength_ * scaleFactor_);
}

PlotArea3D::~PlotArea3D()
{
}

void PlotArea3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    initShaders();
}

void PlotArea3D::resizeGL(int w, int h)
{
    float aspect = (float)w / (float)h;
    projectionMatrix_.setToIdentity();
    projectionMatrix_.perspective(45.0f, aspect, 0.1f, 10.0f);
}

void PlotArea3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    QMatrix4x4 viewMatrix;
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f, 0.0f, mZ_);
    viewMatrix.rotate(rotation_);

    QVector3D eyePos(0.0f, 0.0f, mZ_);
    eyePos = rotation_.rotatedVector(eyePos);

    shaderProgram_.bind();

    shaderProgram_.setUniformValue("a_eyePosition", eyePos);
    shaderProgram_.setUniformValue("u_projectionMatrix", projectionMatrix_ );
    shaderProgram_.setUniformValue("u_viewMatrix", viewMatrix);
    shaderProgram_.setUniformValue("u_modelMatrix", modelMatrix);
    shaderProgram_.setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 3.0f, 0.0f));
    shaderProgram_.setUniformValue("u_lightPower", 15.0f);

    paintAxes();
    paintPlot();
}

void PlotArea3D::paintAxes()
{
    if(!arrayAxesBuffers_[0].isCreated())
    {
        return;
    }
    int offset = 0;
    for(int i = 0; i < 3; i++)
    {
        arrayAxesBuffers_[i].bind();
        int vertAxisLoc = shaderProgram_.attributeLocation("a_position");
        shaderProgram_.enableAttributeArray(vertAxisLoc);
        shaderProgram_.setAttributeBuffer(vertAxisLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

        offset += sizeof(QVector3D);
        int normAxisLoc = shaderProgram_.attributeLocation("a_normal");
        shaderProgram_.enableAttributeArray(normAxisLoc);
        shaderProgram_.setAttributeBuffer(normAxisLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

        offset += sizeof(QVector3D);

        int colorAxisLoc = shaderProgram_.attributeLocation("a_color");
        shaderProgram_.enableAttributeArray(colorAxisLoc);
        shaderProgram_.setAttributeBuffer(colorAxisLoc, GL_FLOAT, offset, 3, sizeof(Vertex));


        indexAxesBuffers_[i].bind();
        glDrawElements(GL_TRIANGLES, indexAxesBuffers_[i].size(), GL_UNSIGNED_INT, 0);
        offset = 0;

        arrayAxesBuffers_[i].release();
        indexAxesBuffers_[i].release();
    }

}

void PlotArea3D::paintPlot()
{
    if(!arrayPlotBuffer_.isCreated())
    {
        return;
    }
    arrayPlotBuffer_.bind();

    float offset = 0;

    int vertLoc = shaderProgram_.attributeLocation("a_position");
    shaderProgram_.enableAttributeArray(vertLoc);
    shaderProgram_.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

    offset += sizeof(QVector3D);

    int normLoc = shaderProgram_.attributeLocation("a_normal");
    shaderProgram_.enableAttributeArray(normLoc);
    shaderProgram_.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

    offset += sizeof(QVector3D);

    int colorLoc = shaderProgram_.attributeLocation("a_color");
    shaderProgram_.enableAttributeArray(colorLoc);
    shaderProgram_.setAttributeBuffer(colorLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

    indexPlotBuffer_.bind();
    glDrawElements(GL_TRIANGLES, indexPlotBuffer_.size(), GL_UNSIGNED_INT, 0);

    arrayPlotBuffer_.release();
    indexPlotBuffer_.release();
}

void PlotArea3D::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        mousePosition_ = QVector2D(event->position());
    }
    event->accept();
}

void PlotArea3D::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }
    QVector2D diff = QVector2D(event->position()) - mousePosition_;
    mousePosition_ = QVector2D(event->position());
    float angle = diff.length() / 2.0;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    rotation_ = QQuaternion::fromAxisAndAngle(axis, angle) * rotation_;
    update();
}

void PlotArea3D::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() < 0)
    {
        mZ_ -= 0.1f;
        scaleFactor_ += 0.1;
        if(scaleFactor_ > maxScaleFactor_)
        {
            maxScaleFactor_ = scaleFactor_;
            initPlotBuilder(defaultLength_ * scaleFactor_);
        }
    }
    else if(event->angleDelta().y() > 0)
    {
        mZ_ += 0.1;
        scaleFactor_ -= 0.1;
        if(scaleFactor_ <= 1.0)
        {
            scaleFactor_ = 1;
        }
    }
    update();
}

void PlotArea3D::receiveData(std::vector<Vertex>* plotVertices,
                             std::vector<unsigned int>* plotIndices)
{
    qDebug() << "receive plot";
    if(plotVertices == nullptr ||
        plotIndices == nullptr)
    {
        return;
    }
    if(plotVertices->size() == 0 ||
        plotIndices->size() == 0) return;
    if(arrayPlotBuffer_.isCreated())
    {
        arrayPlotBuffer_.destroy();
        indexPlotBuffer_.destroy();
    }
    std::vector<Vertex> vertices = std::move(*plotVertices);
    std::vector<unsigned int> indices = std::move(*plotIndices);

    arrayPlotBuffer_.create();
    arrayPlotBuffer_.bind();
    arrayPlotBuffer_.allocate(vertices.data(), vertices.size() * sizeof(Vertex));
    arrayPlotBuffer_.release();

    indexPlotBuffer_.create();
    indexPlotBuffer_.bind();
    indexPlotBuffer_.allocate(indices.data(), indices.size() * sizeof(unsigned int));
    indexPlotBuffer_.release();

    initAxes(defaultLength_ * scaleFactor_);

    if(plotVertices != nullptr)
        delete plotVertices;
    if(plotIndices != nullptr)
        delete plotIndices;

    update();
}



void PlotArea3D::initShaders()
{
    if(!shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
    {
        close();
    }
    if(!shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
    {
        close();
    }
    if(!shaderProgram_.link())
    {
        close();
    }
}

void PlotArea3D::initPlotBuilder(float length)
{
    qDebug() << "init plot";
    Cube field(Vertex(-length, -length, -length), length * 2, length * 2, length * 2);
    auto cubes = field.divide(std::thread::hardware_concurrency() - 1);
    std::unique_ptr<XYZPlotBuilder> builder(new XYZPlotBuilder());
    if(expressionsVector_.size() != 0)
    {
        builder->setCubes(cubes);
        builder->setThreadsNum(std::thread::hardware_concurrency() - 1);
        builder->setResolution({length * 2, length * 2, length * 2});
        builder->setExpression(expressionsVector_);
        scheduler_.addTask(std::move(builder));
    }
}

void PlotArea3D::initAxes(float length)
{
    Axis axes[3] =     {{Vertex(0.0f, 0.0f, (-length - 3.0f)), 0.5, (length + 3.0f) * 2, 2, 16},
                        {Vertex(0.0f, 0.0f, (-length - 3.0f)), 0.5, (length + 3.0f) * 2, 2, 16},
                        {Vertex(0.0f, 0.0f, (-length - 3.0f)), 0.5, (length + 3.0f) * 2, 2, 16}};
    for(int i = 0; i < 3; i++)
    {
        std::for_each(axes[i].axisPoints.begin(), axes[i].axisPoints.end(), [length](auto&& point){
            point.position.setX(point.position.x() / length);
            point.position.setY(point.position.y() / length);
            point.position.setZ(point.position.z() / length);
        });
    }
    QQuaternion rotateX = QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f),
        rotateY = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 90.0f);
    std::for_each(axes[1].axisPoints.begin(), axes[1].axisPoints.end(), [&rotateX](Vertex& point){
        point.position = rotateX.rotatedVector(point.position);
        point.color = QVector3D(1.0f, 0.0f, 0.0f);
    });
    std::for_each(axes[2].axisPoints.begin(), axes[2].axisPoints.end(), [&rotateY](Vertex& point){
        point.position = rotateY.rotatedVector(point.position);
        point.color = QVector3D(0.0f, 0.0f, 1.0f);
    });
    for(int i = 0; i < 3; i++)
    {
        arrayAxesBuffers_[i].create();
        arrayAxesBuffers_[i].bind();
        arrayAxesBuffers_[i].allocate(axes[i].axisPoints.data(), axes[i].axisPoints.size() * sizeof(Vertex));
        arrayAxesBuffers_[i].release();

        indexAxesBuffers_[i].create();
        indexAxesBuffers_[i].bind();
        indexAxesBuffers_[i].allocate(axes[i].axisIndices.data(), axes[i].axisIndices.size() * sizeof(unsigned int));
        indexAxesBuffers_[i].release();
    }

}
