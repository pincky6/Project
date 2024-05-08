#include "xyzplotbuilder.h"
#include <QObject>

using namespace plot_builder;

XYZPlotBuilder::XYZPlotBuilder()
{}

XYZPlotBuilder::XYZPlotBuilder(std::vector<Cube>& cubes,
                               const Resolution&,
                               unsigned int threadsCount)
    : cubes_(cubes), threadsCount_(threadsCount)
{
}

void XYZPlotBuilder::connect()
{
    vertices_.reset(new std::vector<Vertex>());
    indices_.reset(new std::vector<unsigned int>());
    QThread* threadPtr = thread_.get();
    QObject::connect(threadPtr, &QThread::started, this, &XYZPlotBuilder::run);
    QObject::connect(this, &XYZPlotBuilder::finished, this, &XYZPlotBuilder::workFinished);
    moveToThread(thread_.get());
}

void XYZPlotBuilder::start()
{
    thread_->start();
}

void XYZPlotBuilder::work()
{
    if(cubes_.size() == 0) return;
    Resolution screenResolution{
        static_cast<float>(fabs(cubes_[0].originPoint.position.x()) * 2.0f),
        static_cast<float>(fabs(cubes_[0].originPoint.position.y()) * 2.0f),
        static_cast<float>(fabs(cubes_[0].originPoint.position.z()) * 2.0f)
    };
    plotCalculators_.clear();
    plotCalculators_.reserve(cubes_.size());
    for(std::size_t i = 0; i < cubes_.size(); i++)
    {
        const Vertex& point = cubes_[i].originPoint;
        Range xRange = {point.position.x(),
                        point.position.x() + cubes_[i].width};
        Range yRange = {point.position.y(),
                        point.position.y() + cubes_[i].length};
        Range zRange = {point.position.z(),
                        point.position.z() + cubes_[i].height};
        plotCalculators_.emplace_back(new XYZPlotCalculator(
                                            std::cref(expressionsVector_[0]),
                                            xRange, yRange, zRange,
                                            resolution_, screenResolution));
    }
    for(std::size_t i = 0; i < plotCalculators_.size(); i++)
    {
        plotCalculators_[i]->wait();
        std::vector<Vertex> plotVertices = std::move(plotCalculators_[i]->getVertices());
        std::vector<unsigned int> plotIndices = std::move(plotCalculators_[i]->getIndices());
        std::copy(plotVertices.begin(), plotVertices.end(), std::back_inserter(*vertices_));
        std::size_t currentSize = (*indices_).size();
        for(auto index: plotIndices)
        {
            (*indices_).push_back(index + currentSize);
        }
    }
    auto minmax = std::minmax_element(vertices_->cbegin(), vertices_->cend(),
                                      [](auto&& lhs, auto&& rhs){
                                          return lhs.position.z() < rhs.position.z();
                                      });

    double min = minmax.first->position.z();
    double max = minmax.second->position.z();

    double yellow_bound_min = min/2.0f + (max + min) / 2.0f;
    double yellow_bound_max = max/ 2.0f + (max + min) / 2.0f;

    double red_bound_min = min / 4.0f + yellow_bound_min;
    double red_bound_max = max / 4.0f + yellow_bound_max;

    std::for_each(vertices_->begin(), vertices_->end(), [&](Vertex& p){
        if((p.position.z() <= yellow_bound_min && p.position.z() >= red_bound_min)||
            (p.position.z() >= yellow_bound_max && p.position.z() <= red_bound_max))
        {
            p.color = QVector3D(0.5f, 0.5f, 0.0f);
        }
        else if(p.position.z() <= red_bound_min || p.position.z() >= red_bound_max)
        {
            p.color = QVector3D(0.5f, 0.0f, 0.0f);
        }
        else
        {
            p.color = QVector3D(0.0, 0.5f, 0.0f);
        }
    });
}

void XYZPlotBuilder::wait()
{
    for(auto&& plotCalculator: plotCalculators_)
    {
        plotCalculator->wait();
    }
    thread_->wait();
    if(vertices_ == nullptr) return;
    if((*vertices_).size() != 0)
    {
        emit buildingPlotFinish(vertices_, indices_);
    }
}


void XYZPlotBuilder::setCubes(std::vector<Cube>& newCubes)
{
    cubes_ = newCubes;
}

void XYZPlotBuilder::setThreadsNum(unsigned int newThreadsCount)
{
    threadsCount_ = newThreadsCount;
}

void XYZPlotBuilder::setResolution(const Resolution& newResolution)
{
    resolution_ = newResolution;
}

void XYZPlotBuilder::setExpression(const std::vector<QString>& newExpression)
{
    expressionsVector_ = newExpression;
}

XYZPlotBuilder::~XYZPlotBuilder()
{
    for(auto&& plotCalculator: plotCalculators_)
    {
        plotCalculator->wait();
    }
    thread_->quit();
}

void XYZPlotBuilder::workFinished()
{
    thread_->quit();
    if((*vertices_).size() > 0)
    {
        emit buildingPlotFinish(vertices_, indices_);
    }
}
