// Day08
#pragma once

#include <vector>
#include <string_view>
#include <cstdint>
#include <algorithm>

#include "PointCloud.hpp"

using KDTree = nanoflann::KDTreeSingleIndexAdaptor<
    nanoflann::L2_Simple_Adaptor<
    double,     // scalar
    PointCloud, // data source
    double,     // distance result type
    size_t      // index type
    >,
    PointCloud,
    3
>;

namespace day08
{
    struct EdgeInfo {
        std::pair<size_t, size_t> edge;
        double distance;
    };

    // Parses a list of 3D coordinates from a string and returns a PointCloud
    // Format: "162,817,812\n57,618,57\n906,360,560"
    // Each line contains three comma-separated integers representing x, y, z coordinates
    // Returns: PointCloud pointer containing the parsed coordinates
    PointCloud* parse_coordinates(std::string_view payload) {
        std::vector<int32_t> data;
        
        enum class State { SKIP_WHITESPACE, PARSE_X, PARSE_Y, PARSE_Z };
        State state = State::SKIP_WHITESPACE;
        
        int32_t x = 0, y = 0, z = 0;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (state == State::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    continue;
                
                // Start parsing a new coordinate
                x = 0;
                y = 0;
                z = 0;
                state = State::PARSE_X;
            }
            
            if (state == State::PARSE_X) {
                if (c >= '0' && c <= '9') {
                    x = x * 10 + (c - '0');
                } else if (c == ',') {
                    state = State::PARSE_Y;
                }
                continue;
            }
            
            if (state == State::PARSE_Y) {
                if (c >= '0' && c <= '9') {
                    y = y * 10 + (c - '0');
                } else if (c == ',') {
                    state = State::PARSE_Z;
                }
                continue;
            }
            
            if (state == State::PARSE_Z) {
                if (c >= '0' && c <= '9') {
                    z = z * 10 + (c - '0');
                } else if (c == '\n' || c == '\r') {
                    data.push_back(x);
                    data.push_back(y);
                    data.push_back(z);
                    state = State::SKIP_WHITESPACE;
                }
                continue;
            }
        }
        
        // Handle the last coordinate if we ended while parsing Z
        if (state == State::PARSE_Z) {
            data.push_back(x);
            data.push_back(y);
            data.push_back(z);
        }
        
        return new PointCloud(std::move(data));
    }

    uint32_t connect_closest_points(PointCloud* point_cloud, uint32_t nr_connections) {
        size_t nr_points = point_cloud->get_num_points();
        if (nr_points <= 1)
            return 0;

        KDTree index(
            3, *point_cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10)
        );
        index.buildIndex();

        std::vector<size_t> idx(nr_points);
        std::vector<double> dists(nr_points);
        nanoflann::KNNResultSet<double> rs(nr_points);

        std::vector<EdgeInfo> neighbour_distances = {};
        neighbour_distances.reserve(nr_points * (nr_points - 1) / 2);

        const int32_t* point_data = point_cloud->get_data_ptr();
        double query_pt[3] = {0.0, 0.0, 0.0};

        for (size_t i = 0; i < nr_points - 1; i++) {
            rs.init(idx.data(), dists.data());
            query_pt[0] = static_cast<double>(point_data[i * 3]);
            query_pt[1] = static_cast<double>(point_data[i * 3 + 1]);
            query_pt[2] = static_cast<double>(point_data[i * 3 + 2]);

            index.findNeighbors(rs, query_pt);
            for (size_t j = 1; j < nr_points; j++) {
                neighbour_distances.emplace_back(EdgeInfo{{i, idx[j]}, dists[j]});
            }
        }

        std::sort(neighbour_distances.begin(),
            neighbour_distances.end(),
            [](const auto& a, const auto& b) {
                return a.distance < b.distance;
            });

        uint32_t nr_edges_found = 0;
        for (size_t i = 0; i < neighbour_distances.size(); i++) {
            auto edge_info = neighbour_distances[i];
            if (!point_cloud->has_edge(edge_info.edge.first, edge_info.edge.second)) {
                point_cloud->add_edge(edge_info.edge.first, edge_info.edge.second);
                nr_edges_found++;

                if (nr_edges_found >= nr_connections)
                    break;
            }
        }

        return 0;
    }
}
