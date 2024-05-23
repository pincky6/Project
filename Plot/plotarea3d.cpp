#include "plotarea3d.h"

#include <QtOpenGL>
#include <QVector3D>

#include "axis.h"
#include "Database/plottable3d.h"
#include "Database/settingstable.h"
#include "figures.h"

using namespace plot_builder;

PlotArea3D::PlotArea3D(QWidget *parent)
    : QOpenGLWidget(parent),
    indexAxesBuffers_{QOpenGLBuffer(QOpenGLBuffer::IndexBuffer),
                       QOpenGLBuffer(QOpenGLBuffer::IndexBuffer),
                       QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)},
    defaultLength_(20.0f), scaleFactor_(1.0f), maxScaleFactor_(1.0f), mZ_(-5.0f), isAxesVisible(true)
{
    modelScheduler_.start();
}


void PlotArea3D::setExpressions(const std::vector<QString>& newExpressionsVector)
{

    expressionsVector_ = newExpressionsVector;
    PlotTable3D table;
    for(auto&& expression: expressionsVector_)
    {
        if(expression.isEmpty()) continue;
        if(!table.existExpression(expression))
        {
            initPlotBuilder(defaultLength_ * scaleFactor_, expression);
        }
        else
        {
            Plot3D plot3D = table.selectByExpression(expression);
            maxScaleFactor_ = plot3D.maxScaleFactor;
            scaleFactor_ = plot3D.maxScaleFactor;
            resetPlot(plot3D.vertices, plot3D.indices, expression);
        }
    }
}

void PlotArea3D::resetPlot(QSharedPointer<std::vector<Vertex>> plotVertices,
                           QSharedPointer<std::vector<unsigned int>> plotIndices,
                           const QString& expression)
{
    if(!plotVertices.get() ||
       !plotIndices.get())
    {
        return;
    }
    if(plotVertices->size() == 0 ||
        plotIndices->size() == 0) return;
    qDebug() << "dasd";
    destroyPlotBuffer(expression);
    Plot3D plot3D(expression, plotVertices,
                  plotIndices, maxScaleFactor_);
    PlotTable3D table;
    table.insertOrUpdate(plot3D);

    arrayPlotBuffers_[expression].create();
    arrayPlotBuffers_[expression].bind();
    arrayPlotBuffers_[expression].allocate(plot3D.vertices->data(),
                              plot3D.vertices->size() * sizeof(Vertex));
    arrayPlotBuffers_[expression].release();


    indexPlotBuffers_[expression] = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexPlotBuffers_[expression].create();
    indexPlotBuffers_[expression].bind();
    indexPlotBuffers_[expression].allocate(plot3D.indices->data(),
                              plot3D.indices->size() * sizeof(unsigned int));
    indexPlotBuffers_[expression].release();

    initAxes(defaultLength_ * scaleFactor_);

    update();
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
    initAxes(defaultLength_);
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
    if(isAxesVisible)
    {
        paintAxes();
    }
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
    for(auto&& expression: expressionsVector_)
    {
        if(!arrayPlotBuffers_[expression].isCreated())
        {
            return;
        }
        arrayPlotBuffers_[expression].bind();

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

        indexPlotBuffers_[expression].bind();
        glDrawElements(GL_TRIANGLES, indexPlotBuffers_[expression].size(), GL_UNSIGNED_INT, 0);

        arrayPlotBuffers_[expression].release();
        indexPlotBuffers_[expression].release();
    }
}

void PlotArea3D::destroyPlotBuffers()
{
    for(auto&& expression: expressionsVector_)
    {
        if(arrayPlotBuffers_[expression].isCreated())
        {
            arrayPlotBuffers_[expression].destroy();
            indexPlotBuffers_[expression].destroy();
        }
    }
}

void PlotArea3D::destroyPlotBuffer(const QString& expression)
{
    if(expression.isEmpty())
    {
        return;
    }
    if(arrayPlotBuffers_[expression].isCreated())
    {
        arrayPlotBuffers_[expression].destroy();
        indexPlotBuffers_[expression].destroy();
    }
}

void PlotArea3D::freeSchedulers()
{
    for(auto&& scheduler: schedulers_)
    {
        scheduler->freeQueue();
    }
    modelScheduler_.freeQueue();
}

void PlotArea3D::setAxesVisibilityeMode(bool mode)
{
    isAxesVisible = mode;
    update();
}

void PlotArea3D::loadToSTL(const QString& filename)
{
    SettingsTable table;
    SettingsModel model = table.select();
    Cube field(Vertex(-model.modelWidth, -model.modelLength, -model.modelHeight),
               model.modelWidth * 2, model.modelLength * 2, model.modelHeight * 2);
    auto cubes = field.divide(1);
    if(cubes.size() == 0)
    {
        return;
    }
    std::unique_ptr<STLModelBuilder> builder(new STLModelBuilder());
    if(expressionsVector_.size() != 0)
    {
        builder->setFilename(filename);
        builder->setExpression(expressionsVector_[0]);
        builder->setResolution({static_cast<float>(model.resolutionX),
                                static_cast<float>(model.resolutionY),
                                static_cast<float>(model.resolutionZ)});
        builder->setCube(cubes[0]);
        modelScheduler_.addTask(std::move(builder), this);
    }
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
            for(const QString& expression: expressionsVector_)
            {
                initPlotBuilder(defaultLength_ * scaleFactor_, expression);
            }
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

void PlotArea3D::receiveData(QSharedPointer<std::vector<Vertex>> plotVertices,
                             QSharedPointer<std::vector<unsigned int>> plotIndices,
                             QString expression)
{
    qDebug() << "receive plot";

    resetPlot(plotVertices, plotIndices, expression);
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

void PlotArea3D::initPlotBuilder(float length, const QString& expression)
{
    qDebug() << "init plot";

    Cube field(Vertex(-length, -length, -length), length * 2, length * 2, length * 2);
    auto cubes = field.divide(std::thread::hardware_concurrency() - 1);
    qDebug() << cubes.size();
    QSharedPointer<PlotBuilder> builder(new PlotBuilder());
    if(!expression.isEmpty())
    {
        builder->setCubes(cubes);
        builder->setThreadsNum(std::thread::hardware_concurrency() - 1);
        builder->setResolution({length * 2, length * 2, length * 2});
        builder->setExpression(expression);
        if(schedulers_.find(expression) == schedulers_.end())
        {
            schedulers_[expression].reset(new PlotScheduler);
            connect(schedulers_[expression].get(), &PlotScheduler::updatePlot, this, &PlotArea3D::receiveData);
            schedulers_[expression]->start();
        }
        schedulers_[expression]->addTask(builder);
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


//Построение оболочки: Один из популярных методов закрытия дыр в трехмерных моделях — это построение выпуклой оболочки (convex hull) вокруг модели. Этот алгоритм позволяет найти внешние грани модели, которые могут быть использованы для закрытия дыр.
//Триангуляция: Другой метод — это триангуляция поверхности модели. Путем разбиения поверхности на треугольники можно обнаружить и заполнить дыры.
//Алгоритмы реконструкции поверхности: Некоторые алгоритмы, такие как Marching Cubes или Poisson Surface Reconstruction, могут восстановить поверхность модели, включая закрытие дыр.
//Методы, основанные на графах: Графовые алгоритмы могут помочь найти связи между вершинами модели и определить, какие грани следует добавить для закрытия дыр.
