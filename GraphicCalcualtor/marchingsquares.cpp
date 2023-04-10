

// Internal Includes
#include "marchingsquares.h"
#include "node.h"
#include "Calculator/recursivedescent.h"

#include "edge.h"

// Standard includes
#include <iostream>

namespace marching_squares {

inline size_t ToInt(const bool arr[], const size_t size)
{
    size_t result = 0;

    for (size_t i = 0; i < size; ++i)
        result |= static_cast<size_t>(arr[i]) << (size - i - 1);

    return result;
}

inline Point2D LinearInterpolate(const Point2D& origin,
                                 const Point2D&& target,
                                 const Point2D& values,
                                 const double iso_value)
{
    const auto t = (iso_value - values[0]) / (values[1] - values[0]);

    return {
        (1 - t) * origin[0] + t * target[0],
                (1 - t) * origin[1] + t * target[1]
    };
}

MarchingSquares::MarchingSquares(Function function,
                                 const Limits& x_limits,
                                 const Limits& y_limits,
                                 const Resolution& resolution):
    function_(std::move(function)),
    x_limits_(x_limits),
    y_limits_(y_limits),
    resolution_(verify_resolution(resolution)),
    dx_((x_limits[1] - x_limits[0]) / resolution_[0]),
    dy_((y_limits[1] - y_limits[0]) / resolution_[1]),
    cur_x_(x_limits[0]),
    cur_y_(y_limits[0]),
    x_major_(resolution[0] > resolution[1])
{
    nx1_ = resolution[!x_major_];
    nx2_ = resolution[x_major_];
}

Resolution MarchingSquares::verify_resolution(const Resolution& resolution)
{
    auto new_resolution = resolution;
    if (resolution[0] < 1 )
    {
        std::cerr << "MarchingSquares::Constructor: The resolution in x direction must be greater than 0. Automatically setting it to 1\n";
        new_resolution[0] = 1;
    }
    if (resolution[1] < 1)
    {
        std::cerr << "MarchingSquares::Constructor: The resolution in y direction must be greater than 0. Automatically setting it to 1\n";
        new_resolution[1] = 1;
    }

    return new_resolution;}

EdgeVertices MarchingSquares::edge_id_to_nodes(const size_t id) const
{
    if (id >= total_edges_)
        return {};

    return edge_to_vertices_[id];
}

EdgeIndexList MarchingSquares::case_to_edges(const size_t id) const
{
    if (id >= total_cases_)
        return {};

    return lookup_table_[id];
}

void MarchingSquares::increment_minor_axis() const
{
    if (x_major_)
        cur_y_ += dy_;
    else
        cur_x_ += dx_;
}

void MarchingSquares::reset_minor_axis() const
{
    if (x_major_)
        cur_y_ = y_limits_[0];
    else
        cur_x_ = x_limits_[0];
}

void MarchingSquares::increment_major_axis() const
{
    if (x_major_)
        cur_x_ += dx_;
    else
        cur_y_ += dy_;
}
void MarchingSquares::reset_major_axis() const
{
    if (x_major_)
        cur_x_ = x_limits_[0];
    else
        cur_y_ = y_limits_[0];
}



EdgeList MarchingSquares::compute(const double iso_value, RecursiveDecsent& recursiveDecsent) const
{
    EdgeList edge_list;
    // Extract the entries for better readability
    const auto x_lower = x_limits_[0];
    const auto x_upper = x_limits_[1];
    const auto y_lower = y_limits_[0];
    const auto y_upper = y_limits_[1];

    const auto dx = (x_upper - x_lower) / resolution_[0];
    const auto dy = (y_upper - y_lower) / resolution_[1];



    // Go over all the cells
    for(size_t i = 0; i < resolution_[0]; ++i)
    {
        const auto x = x_lower + i * dx;

        for(size_t j = 0; j < resolution_[1]; ++j)
        {
            const auto y = y_lower + j * dy;

            // Calculate the node coordinates
            std::array<Node, 4> nodes = { {
                    {(double)function(recursiveDecsent, x_lower +  i      * dx, y_lower + (j + 1) * dy), x,      y + dy},        //Node 1
                    {(double)function(recursiveDecsent, x_lower + (i + 1) * dx, y_lower + (j + 1) * dy), x + dx, y + dy},        //Node 2
                    {(double)function(recursiveDecsent, x_lower + (i + 1) * dx, y_lower +  j      * dy), x + dx, y     },        //Node 3
                    {(double)function(recursiveDecsent, x_lower +  i      * dx, y_lower +  j      * dy), x,      y     },        //Node 4
                } };

            // Compute the function signs based on iso value
            bool func_signs[4];

            for(size_t k = 0; k < 4; ++k)
            {
                func_signs[k] = (iso_value - nodes[k].Value()) > 0;
            }

            // Compute the corresponding key from the function signs
            const auto key = ToInt(func_signs, 4);

            // Get the edges corresponding to the key
            const auto edges = case_to_edges(key);

            // Compute and add the edge after interpolation
            if (edges.size() == 2)
            {
                // Get the coordinates of the first edge
                auto nodes_id = edge_id_to_nodes(edges[0]);
                const auto origin = Edge(&nodes[nodes_id[0]], &nodes[nodes_id[1]]).GetIsoCoordinates(iso_value);

                // Get the coordinates of the second edge
                nodes_id = edge_id_to_nodes(edges[1]);
                const auto target = Edge(&nodes[nodes_id[0]], &nodes[nodes_id[1]]).GetIsoCoordinates(iso_value);

                edge_list.push_back({ {{origin[0], origin[1]},
                                       {target[0], target[1]}} });
            }
            else if(edges.size() == 4)
            {
                auto nodes_id = edge_id_to_nodes(edges[0]);
                auto point1 = Edge(&nodes[nodes_id[0]], &nodes[nodes_id[1]]).GetIsoCoordinates(iso_value);

                nodes_id = edge_id_to_nodes(edges[1]);
                const auto point2 = Edge(&nodes[nodes_id[0]], &nodes[nodes_id[1]]).GetIsoCoordinates(iso_value);

                nodes_id = edge_id_to_nodes(edges[2]);
                auto point3 = Edge(&nodes[nodes_id[0]], &nodes[nodes_id[1]]).GetIsoCoordinates(iso_value);

                nodes_id = edge_id_to_nodes(edges[3]);
                const auto point4 = Edge(&nodes[nodes_id[0]], &nodes[nodes_id[1]]).GetIsoCoordinates(iso_value);

                // Sort based on the x-component of the array
                if (point1[0] > point3[0])
                    std::swap(point1, point3);

                edge_list.push_back({ {{point1[0], point1[1]},
                                       {point4[0], point4[1]}} });

                edge_list.push_back({ {{point2[0], point2[1]},
                                       {point3[0], point3[1]}} });
            }
        }
    }

    return edge_list;
}

void MarchingSquares::check_vertical_edge(std::vector<double>& arr, const uint32_t index,
                                          const double iso_value, Point2D& last_point, std::vector<uint32_t>& index_map, RecursiveDecsent& recursiveDecsent) const
{
    Operand& opX = recursiveDecsent.getElementFromArgumentTable("x");
    Operand& opY = recursiveDecsent.getElementFromArgumentTable("y");
    opX = Double(cur_x_);
    opY = Double(cur_y_);
    arr[static_cast<size_t>(index) + 1] = dynamic_cast<Double&>(*recursiveDecsent.calculate(function_).getOperand());

    // If function value signs are opposite
    if ((arr[index] - iso_value) *
            (arr[static_cast<size_t>(index) + 1] - iso_value) <= 0)
    {
        vertices_.emplace_back(LinearInterpolate(last_point,
                                                 { cur_x_, cur_y_ },
                                                 { arr[index], arr[static_cast<size_t>(index) + 1] },
                                                 iso_value));

        index_map[index] = vertex_count_;

        ++vertex_count_;
    }
    last_point = { cur_x_, cur_y_ };
}

void MarchingSquares::check_horizontal_edge(const Point2D&& values, double iso_value,
                                            Point2D&& last_point, uint32_t& index) const{
    if ((values[0] - iso_value) *
            (values[1] - iso_value) <= 0)
    {
        vertices_.emplace_back(LinearInterpolate(last_point,
                                                 { cur_x_, cur_y_ },
                                                 values,
                                                 iso_value));
        index = vertex_count_;
        ++vertex_count_;
    }
}

void MarchingSquares::reset() const
{
    reset_major_axis();
    reset_minor_axis();
}

Double MarchingSquares::function(RecursiveDecsent& recursiveDescent, float x, float y) const
{
    recursiveDescent.getElementFromArgumentTable("x") = Double(x);
    recursiveDescent.getElementFromArgumentTable("y") = Double(y);

    return dynamic_cast<Double&>(*recursiveDescent.calculate(function_).getOperand());
}

Point2D MarchingSquares::get_previous_horizontal_point() const
{
    if (x_major_)
        return Point2D{ cur_x_ - dx_, cur_y_ };

    return Point2D{ cur_x_, cur_y_ - dy_};
}

// In order to store minimum number of calculations, we play smart
// We will return a vertex array object and an element index buffer
\
} // namespace marching_squares
