#include "stlmodelbuilder.h"
#include <QFile>
#include <QThread>
#include <QDebug>

plot_builder::STLModelBuilder::STLModelBuilder()
{

}

plot_builder::STLModelBuilder::STLModelBuilder(const QString& filename, const QString &expression,
                                               Range xRange, Range yRange, Range zRange,
                                               Resolution resolution, Resolution screenResolution,
                                               AbstractPlotWorker *parent):
    plotCalculator_(new XYZPlotCalculator(expression,
                                         xRange, yRange, zRange,
                                         resolution, screenResolution, parent)),
    filename_(filename)
{

}

plot_builder::STLModelBuilder::STLModelBuilder(STLModelBuilder&& stlModelBuilder) noexcept: plotCalculator_(std::move(stlModelBuilder.plotCalculator_))
{

}

void plot_builder::STLModelBuilder::work()
{
    plotCalculator_->wait();
    emit endCalculations();
    qDebug() << "end calculations";
    if(plotCalculator_->getVertices().size() == 0 ||
        plotCalculator_->getIndices().size() == 0)
    {
        qDebug() << "Wrong calculations";
        emit wrongCalculations();
        return;
    }
    QFile file(filename_);
    file.open(QIODevice::ReadWrite);
//    if(file.exists())
//    {
//        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
//    }
//    else
//    {


//    }

    QString facetString = "facet normal  %0 %1 %2\n"
                          "outer loop\n"
                          "vertex %3 %4 %5\n"
                          "vertex %6 %7 %8\n"
                          "vertex %9 %10 %11\n"
                          "endloop\n"
                          "endfacet\n";
    file.write("solid example\n");
    std::vector<Vertex> vertices = std::move(plotCalculator_->getVertices());
    std::vector<unsigned int> indices = std::move(plotCalculator_->getIndices());

    for(std::size_t i = 0; i < indices.size(); i += 3)
    {
        Vertex& vertex1 = vertices.at(i);
        Vertex& vertex2 = vertices.at(i + 1);
        Vertex& vertex3 = vertices.at(i + 2);
        file.write(facetString.arg(vertex1.normal.x())
                       .arg(vertex1.normal.y())
                       .arg(vertex1.normal.z())
                       .arg(vertex1.position.x())
                       .arg(vertex1.position.y())
                       .arg(vertex1.position.z())
                       .arg(vertex2.position.x())
                       .arg(vertex2.position.y())
                       .arg(vertex2.position.z())
                       .arg(vertex3.position.x())
                       .arg(vertex3.position.y())
                       .arg(vertex3.position.z()).toStdString().data()
                   );

    }
    file.write("endsolid example");
    file.close();
    emit endFileWriting();
    qDebug() << "End File Writing";
}

void plot_builder::STLModelBuilder::connect()
{
    QThread* threadPtr = thread_.get();
    QObject::connect(threadPtr, &QThread::started, this, &STLModelBuilder::run);
    QObject::connect(this, &STLModelBuilder::finished, &*threadPtr, &QThread::quit);
    Resolution screenResolution{
        static_cast<float>(fabs(cube_.originPoint.position.x()) * 2.0f),
        static_cast<float>(fabs(cube_.originPoint.position.y()) * 2.0f),
        static_cast<float>(fabs(cube_.originPoint.position.z()) * 2.0f)
    };
    const Vertex& point = cube_.originPoint;
    Range xRange = {point.position.x(),
                    point.position.x() + cube_.width};
    Range yRange = {point.position.y(),
                    point.position.y() + cube_.length};
    Range zRange = {point.position.z(),
                    point.position.z() + cube_.height};
    plotCalculator_.reset(new XYZPlotCalculator(
        std::cref(expression_),
        xRange, yRange, zRange,
        resolution_, screenResolution));
    moveToThread(thread_.get());
}

void plot_builder::STLModelBuilder::start()
{
    thread_->start();
}

plot_builder::XYZPlotCalculator &plot_builder::STLModelBuilder::getPlotCalculator()
{
    return *plotCalculator_;
}

void plot_builder::STLModelBuilder::setFilename(const QString& filename)
{
    filename_ = filename;
}

void plot_builder::STLModelBuilder::setExpression(const QString& expression)
{
    expression_ = expression;
}

void plot_builder::STLModelBuilder::setResolution(const Resolution &resolution)
{
    resolution_ = resolution;
}

void plot_builder::STLModelBuilder::setCube(const Cube &cube)
{
    cube_ = cube;
}
