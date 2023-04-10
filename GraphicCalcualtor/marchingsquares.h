#ifndef MARCHINGSQUARES_H
#define MARCHINGSQUARES_H


#include <vector>
#include <array>
#include <functional>
#include <tuple>
#include <inttypes.h>
#include <QString>
#include "Calculator/recursivedescent.h"


namespace marching_squares {

using Point2D = std::array<double, 2>;

using EdgeVertices = std::array<uint32_t, 2>;
using EdgeIndexList = std::vector<size_t>;

using EdgeList = std::vector<std::array<std::array<double, 2>, 2>>;

using Function = QString;
using Limits = std::array<double, 2>;
using Resolution = std::array<int, 2>;

using VerticesList = std::vector<Point2D>;
using IndicesList = std::vector<EdgeVertices>;

class MarchingSquares
{

private:
    const std::vector<EdgeVertices> edge_to_vertices_ = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0}
    };


    const std::vector<EdgeIndexList> lookup_table_ = {
        {},
        {2, 3},
        {1, 2},
        {1, 3},
        {0, 1},
        {0, 1, 2, 3},
        {0, 2},
        {0, 3},
        {0, 3},
        {0, 2},
        {0, 1, 2, 3},
        {0, 1},
        {1, 3},
        {1, 2},
        {2, 3},
        {}
    };

    const size_t total_edges_ = edge_to_vertices_.size();
    const size_t total_cases_ = lookup_table_.size();

    const QString function_;
    const Limits x_limits_;
    const Limits y_limits_;
    const Resolution resolution_;

    const double dx_, dy_;
    mutable double cur_x_, cur_y_;
    const bool x_major_;

    size_t nx1_, nx2_;

    mutable uint32_t vertex_count_ = 0;
    mutable VerticesList vertices_ = {};

    static Resolution verify_resolution(const Resolution& resolution);
    EdgeVertices edge_id_to_nodes(size_t id) const;
    EdgeIndexList case_to_edges(size_t id) const;

    void increment_minor_axis() const;
    void reset_minor_axis() const;
    void increment_major_axis() const;
    void reset_major_axis() const;
    inline void check_vertical_edge(std::vector<double>& arr, uint32_t index, double iso_value, Point2D& last_point, std::vector<uint32_t>& index_map, RecursiveDecsent&) const;
    Point2D get_previous_horizontal_point() const;
    inline void check_horizontal_edge(const Point2D&& values, double iso_value, Point2D&& last_point, uint32_t& index) const;
    void reset() const;
    Double function(RecursiveDecsent& recusiveDecsent, float x, float y) const;
public:
    MarchingSquares(Function function,
                    const Limits& x_limits,
                    const Limits& y_limits,
                    const Resolution& resolution);

    EdgeList compute(double iso_value, RecursiveDecsent&) const;
};

} //namespace marching_squares

#endif // MARCHINGSQUARES_H
