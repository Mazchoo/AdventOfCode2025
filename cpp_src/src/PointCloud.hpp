// Common PointCloud class for 3D point cloud data
#pragma once

#include <vector>
#include <algorithm>
#include <cstdint>
#include <nanoflann.hpp>
#include <cmath>

// Generic point cloud class for storing 3D point data (int32_t coordinates)
// Assumes size_t fits in uint32_t for WASM 32 target
class PointCloud {
private:
    std::vector<int32_t> data;  // Stores x, y, z coordinates sequentially
    std::vector<std::pair<uint32_t, uint32_t>> edges;
    
public:
    // Constructor
    PointCloud(std::vector<int32_t> cloud_data)
        : data(std::move(cloud_data)){
        // Ensure data size is a multiple of 3
        auto remainder = data.size() % 3;
        if (remainder > 0) {
            data.resize(data.size() + 3 - remainder, 0);
        }
        edges = {};
    }
    
    // Default constructor
    PointCloud() : data() {}
    
    // Get the point cloud data
    const std::vector<int32_t>& get_data() const {
        return data;
    }
    
    // Get mutable point cloud data
    std::vector<int32_t>& get_data() {
        return data;
    }
    
    // Get number of points
    size_t get_num_points() const {
        return data.size() / 3;
    }

    // Get number of points
    size_t get_num_edges() const {
        return edges.size();
    }
    
    // Get element at index (linear access to x, y, z values)
    int32_t get_element(size_t index) const {
        if (index >= data.size()) {
            return 0;
        }
        return data[index];
    }
    
    // Set element at index
    bool set_element(size_t index, int32_t value) {
        if (index >= data.size()) {
            return false;
        }
        data[index] = value;
        return true;
    }
    
    // Get coordinate of a point (coord: 0=x, 1=y, 2=z)
    int32_t get_point(size_t point_index, size_t coord) const {
        if (point_index >= data.size() / 3 || coord >= 3) {
            return 0;
        }
        size_t index = point_index * 3 + coord;
        return data[index];
    }
    
    // Set coordinate of a point (coord: 0=x, 1=y, 2=z)
    bool set_point(size_t point_index, size_t coord, int32_t value) {
        if (point_index >= data.size() / 3 || coord >= 3) {
            return false;
        }
        size_t index = point_index * 3 + coord;
        if (index >= data.size()) {
            return false;
        }
        data[index] = value;
        return true;
    }
    
    bool add_edge(uint32_t i, uint32_t j) {
        if (i >= data.size() || j >= data.size()) {
            return false;
        }

        edges.emplace_back(std::make_pair(i, j));
        return true;
    }

    bool has_edge(uint32_t i, uint32_t j) {
        for (auto& edge: edges) {
            if (edge.first == i && edge.second == j)
                return true;
            if (edge.first == j && edge.second == i)
                return true;
        }
        return false;
    }

    std::vector<std::pair<uint32_t, uint32_t>>& get_edges() {
        return edges;
    }

    // Map every node to the index of the maximum node that is accessible from it by some edge
    std::vector<uint32_t> get_component_ids() {
        auto nr_points = this->get_num_points();

        // Initialize
        std::vector<uint32_t> component_ids(nr_points);
        for (size_t i = 0; i < nr_points; i++) {
            component_ids[i] = static_cast<uint32_t>(i);
        }

        bool any_change = true;
        while (any_change) {
            any_change = false;
            for (const auto& edge: this->get_edges()) {
                if (component_ids[edge.first] != component_ids[edge.second]) {
                    component_ids[edge.first] = std::max<uint32_t>(
                        component_ids[edge.first],
                        component_ids[edge.second]
                    );
                    component_ids[edge.second] = std::max<uint32_t>(
                        component_ids[edge.first],
                        component_ids[edge.second]
                    );
                    any_change = true;
                }
            }
        }

        return component_ids;
    }

    bool is_fully_connected() {
        const auto nr_edges = this->get_num_edges();
        const auto nr_points = this->get_num_points();

        if (nr_points == 0)
            return true;

        if (nr_edges < nr_points - 1)
            return false; // early return

        std::vector<bool> covered_points(nr_points, false);
        std::vector<bool> covered_edges(nr_edges, false);
        covered_points[0] = true;
        bool any_change = true;
        while (any_change) {
            any_change = false;
            for (size_t i = 0; i < nr_edges; i++) {
                if (covered_edges[i])
                    continue;

                const auto edge = this->edges[i];
                if (covered_points[edge.first] ^ covered_points[edge.second]) {
                    covered_points[edge.first] = true;
                    covered_points[edge.second] = true;
                    covered_edges[i] = true;
                    any_change = true;
                }
            }
        }

        return std::all_of(covered_points.begin(), covered_points.end(), [](bool x) {return x;});
    }

    const int32_t* get_data_ptr() const {
        return data.data();
    }

    inline size_t kdtree_get_point_count() const {
        return this->get_num_points();
    }

    inline double kdtree_get_pt(size_t idx, size_t dim) const {
        return double(data[idx * 3 + dim]);
    }

    template <class BBOX>
    bool kdtree_get_bbox(BBOX&) const { return false; }
};

// Generic function to free a PointCloud
inline void free_point_cloud(PointCloud* cloud) {
    if (cloud != nullptr) {
        delete cloud;
    }
}
