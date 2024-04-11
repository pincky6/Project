#include "plotarea3d.h"

#include <QtOpenGL>
#include <QVector3D>

#include "axis.h"

#include "figures.h"

PlotArea3D::PlotArea3D(QWidget *parent)
    : QOpenGLWidget(parent),
    m_indexPlotBuffer(QOpenGLBuffer::IndexBuffer),
    m_indexAxesBuffers{QOpenGLBuffer(QOpenGLBuffer::IndexBuffer),
                       QOpenGLBuffer(QOpenGLBuffer::IndexBuffer),
                       QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)},
    defaultLength(20.0f), scaleFactor(1.0f), m_z(-5.0f)
{
    m_z = -5.0f;
}

void PlotArea3D::setExpressions(const std::vector<QString>& newExpressionsVector)
{
    expressionsVector = newExpressionsVector;
    initPlot(defaultLength * scaleFactor);
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
 //   initPlot(defaultLength);
}

void PlotArea3D::resizeGL(int w, int h)
{
    float aspect = (float)w / (float)h;
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f, aspect, 0.1f, 10.0f);
}

void PlotArea3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    QMatrix4x4 viewMatrix;
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f, 0.0f, m_z);
    viewMatrix.rotate(m_rotation);

    QVector3D eyePos(0.0f, 0.0f, m_z);
    eyePos = m_rotation.rotatedVector(eyePos);

    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("a_eyePosition", eyePos);
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectionMatrix );
    m_shaderProgram.setUniformValue("u_viewMatrix", viewMatrix);
    m_shaderProgram.setUniformValue("u_modelMatrix", modelMatrix);
    m_shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 3.0f, 0.0f));
    m_shaderProgram.setUniformValue("u_lightPower", 15.0f);

    paintAxes();
    paintPlot();
}

void PlotArea3D::paintAxes()
{
    if(!m_arrayAxesBuffers[0].isCreated())
    {
        return;
    }
    int offset = 0;
    for(int i = 0; i < 3; i++)
    {
        m_arrayAxesBuffers[i].bind();
        int vertAxisLoc = m_shaderProgram.attributeLocation("a_position");
        m_shaderProgram.enableAttributeArray(vertAxisLoc);
        m_shaderProgram.setAttributeBuffer(vertAxisLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

        offset += sizeof(QVector3D);
        int normAxisLoc = m_shaderProgram.attributeLocation("a_normal");
        m_shaderProgram.enableAttributeArray(normAxisLoc);
        m_shaderProgram.setAttributeBuffer(normAxisLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

        offset += sizeof(QVector3D);

        int colorAxisLoc = m_shaderProgram.attributeLocation("a_color");
        m_shaderProgram.enableAttributeArray(colorAxisLoc);
        m_shaderProgram.setAttributeBuffer(colorAxisLoc, GL_FLOAT, offset, 3, sizeof(Vertex));


        m_indexAxesBuffers[i].bind();
        glDrawElements(GL_TRIANGLES, m_indexAxesBuffers[i].size(), GL_UNSIGNED_INT, 0);
        offset = 0;

        m_arrayAxesBuffers[i].release();
        m_indexAxesBuffers[i].release();
    }

}

void PlotArea3D::paintPlot()
{
    if(!m_arrayPlotBuffer.isCreated())
    {
        return;
    }
    m_arrayPlotBuffer.bind();

    float offset = 0;

    int vertLoc = m_shaderProgram.attributeLocation("a_position");
    m_shaderProgram.enableAttributeArray(vertLoc);
    m_shaderProgram.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

    offset += sizeof(QVector3D);

    int normLoc = m_shaderProgram.attributeLocation("a_normal");
    m_shaderProgram.enableAttributeArray(normLoc);
    m_shaderProgram.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

    offset += sizeof(QVector3D);

    int colorLoc = m_shaderProgram.attributeLocation("a_color");
    m_shaderProgram.enableAttributeArray(colorLoc);
    m_shaderProgram.setAttributeBuffer(colorLoc, GL_FLOAT, offset, 3, sizeof(Vertex));

    m_indexPlotBuffer.bind();
    glDrawElements(GL_TRIANGLES, m_indexPlotBuffer.size(), GL_UNSIGNED_INT, 0);

    m_arrayPlotBuffer.release();
    m_indexPlotBuffer.release();
}

void PlotArea3D::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        m_mousePosition = QVector2D(event->position());
    }
    event->accept();
}

void PlotArea3D::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
    {
        return;
    }
    QVector2D diff = QVector2D(event->position()) - m_mousePosition;
    m_mousePosition = QVector2D(event->position());
    float angle = diff.length() / 2.0;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
    update();
}

void PlotArea3D::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() < 0)
    {
        m_z -= 0.25f;
        scaleFactor += 0.1;
        if(scaleFactor > maxScaleFactor)
        {
            maxScaleFactor = scaleFactor;
            if(builder.get() != nullptr)
                builder->wait();
            initPlot(defaultLength * scaleFactor);
        }

    }
    else if(event->angleDelta().y() > 0)
    {
        scaleFactor -= 0.1;
        m_z += 0.25;
        if(scaleFactor <= 1.0)
        {
            scaleFactor = 1;
        }
    }
    update();
}

void PlotArea3D::receiveData()
{
    qDebug() << "receive plot";
    if(builder->getVertices().size() == 0 ||
        builder->getIndices().size() == 0) return;
    if(m_arrayPlotBuffer.isCreated())
    {
        m_arrayPlotBuffer.destroy();
        m_indexPlotBuffer.destroy();
    }
    std::vector<Vertex>& vertices = builder->getVertices();
    std::vector<unsigned int>& indices = builder->getIndices();

    m_arrayPlotBuffer.create();
    m_arrayPlotBuffer.bind();
    m_arrayPlotBuffer.allocate(vertices.data(), vertices.size() * sizeof(Vertex));
    m_arrayPlotBuffer.release();

    m_indexPlotBuffer.create();
    m_indexPlotBuffer.bind();
    m_indexPlotBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));
    m_indexPlotBuffer.release();

    initAxes(defaultLength * scaleFactor);
    builder->clearBuffers();
    update();
}



void PlotArea3D::initShaders()
{
    if(!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
    {
        close();
    }
    if(!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
    {
        close();
    }
    if(!m_shaderProgram.link())
    {
        close();
    }
}

void PlotArea3D::initPlot(float length)
{
    qDebug() << "init plot";
    Cube field(Vertex(-length, -length, -length), length * 2, length * 2, length * 2);
    auto cubes = field.divide(std::thread::hardware_concurrency() - 1);
    if(builder.get() != nullptr)
    {
        builder->wait();
    }
    if(expressionsVector.size() != 0)
    {
        builder.reset(new plot_builder::XYZPlotBuilder());
        connect(&(*builder), &plot_builder::XYZPlotBuilder::buildingPlotFinish,
                this, &PlotArea3D::receiveData);

        builder->setCubes(cubes);
        builder->setThreadsNum(std::thread::hardware_concurrency() - 1);
        builder->setResolution({length * 2, length * 2, length * 2});
        builder->setExpression(expressionsVector);

        builder->start();
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
        m_arrayAxesBuffers[i].create();
        m_arrayAxesBuffers[i].bind();
        m_arrayAxesBuffers[i].allocate(axes[i].axisPoints.data(), axes[i].axisPoints.size() * sizeof(Vertex));
        m_arrayAxesBuffers[i].release();

        m_indexAxesBuffers[i].create();
        m_indexAxesBuffers[i].bind();
        m_indexAxesBuffers[i].allocate(axes[i].axisIndices.data(), axes[i].axisIndices.size() * sizeof(unsigned int));
        m_indexAxesBuffers[i].release();
    }

}
