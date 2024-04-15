#include "xyzplotbuilder.h"

using namespace plot_builder;

XYZPlotBuilder::XYZPlotBuilder()
{}

XYZPlotBuilder::XYZPlotBuilder(std::vector<Cube>& cubes,
                               const Resolution&,
                               unsigned int threadsCount)
    : cubes_(cubes), threadsCount_(threadsCount)
{
}

void XYZPlotBuilder::start()
{
    vertices_ = new std::vector<Vertex>();
    indices_  = new std::vector<unsigned int>();
    connect(&(*thread_), &QThread::started, this, &XYZPlotBuilder::run);
    connect(this, &XYZPlotBuilder::finished, this, &XYZPlotBuilder::workFinished);
    moveToThread(&(*thread_));
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
        emit buildingPlotFinish(this, vertices_, indices_);
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
        emit buildingPlotFinish(this, vertices_, indices_);
    }
}
