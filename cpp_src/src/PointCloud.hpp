// Common PointCloud class for 3D point cloud data
#pragma once

#include <vector>
#include <cstdint>
#include <nanoflann.hpp>
#include <cmath>

// Generic point cloud class for storing 3D point data (int32_t coordinates)
class PointCloud {
private:
    std::vector<int32_t> data;  // Stores x, y, z coordinates sequentially
    size_t num_points;
    
public:
    // Constructor
    PointCloud(std::vector<int32_t> cloud_data, size_t point_count)
        : data(std::move(cloud_data)), num_points(point_count) {
        // Ensure data size is a multiple of 3
        if (data.size() != num_points * 3) {
            data.resize(num_points * 3, 0);
        }
    }
    
    // Default constructor
    PointCloud() : data(), num_points(0) {}
    
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
        return num_points;
    }
    
    // Get total size (num_points * 3)
    size_t get_size() const {
        return data.size();
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
        if (point_index >= num_points || coord >= 3) {
            return 0;
        }
        size_t index = point_index * 3 + coord;
        if (index >= data.size()) {
            return 0;
        }
        return data[index];
    }
    
    // Set coordinate of a point (coord: 0=x, 1=y, 2=z)
    bool set_point(size_t point_index, size_t coord, int32_t value) {
        if (point_index >= num_points || coord >= 3) {
            return false;
        }
        size_t index = point_index * 3 + coord;
        if (index >= data.size()) {
            return false;
        }
        data[index] = value;
        return true;
    }
    
    // Get raw data pointer (for advanced use)
    int32_t* get_data_ptr() {
        return data.data();
    }
    
    // Get const raw data pointer
    const int32_t* get_data_ptr() const {
        return data.data();
    }
        inline size_t kdtree_get_point_count() const {
        return data.size() / 3;
    }

    inline float kdtree_get_pt(size_t idx, size_t dim) const {
        return float(data[idx * 3 + dim]);
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
