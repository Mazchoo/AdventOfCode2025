// Common Image class for binary/multi-state image data
#pragma once

#include <vector>
#include <cstdint>

// Generic image class for storing 2D grid data
class Image {
private:
    std::vector<uint8_t> data;
    size_t pitch;
    
public:
    // Constructor
    Image(std::vector<uint8_t> image_data, size_t image_pitch)
        : data(std::move(image_data)), pitch(image_pitch) {}
    
    // Default constructor
    Image() : data(), pitch(0) {}
    
    // Get the image data
    const std::vector<uint8_t>& get_data() const {
        return data;
    }
    
    // Get mutable image data
    std::vector<uint8_t>& get_data() {
        return data;
    }
    
    // Get pitch (width)
    size_t get_pitch() const {
        return pitch;
    }
    
    // Get height
    size_t get_height() const {
        if (pitch == 0) return 0;
        return data.size() / pitch;
    }
    
    // Get total size
    size_t get_size() const {
        return data.size();
    }
    
    // Get element at index
    uint8_t get_element(size_t index) const {
        if (index >= data.size()) {
            return 0;
        }
        return data[index];
    }
    
    // Set element at index
    bool set_element(size_t index, uint8_t value) {
        if (index >= data.size()) {
            return false;
        }
        data[index] = value;
        return true;
    }
    
    // Get element at row, column
    uint8_t get_pixel(size_t row, size_t col) const {
        if (pitch == 0 || col >= pitch) {
            return 0;
        }
        size_t index = row * pitch + col;
        if (index >= data.size()) {
            return 0;
        }
        return data[index];
    }
    
    // Set element at row, column
    bool set_pixel(size_t row, size_t col, uint8_t value) {
        if (pitch == 0 || col >= pitch) {
            return false;
        }
        size_t index = row * pitch + col;
        if (index >= data.size()) {
            return false;
        }
        data[index] = value;
        return true;
    }
    
    // Get raw data pointer (for advanced use)
    uint8_t* get_data_ptr() {
        return data.data();
    }
    
    // Get const raw data pointer
    const uint8_t* get_data_ptr() const {
        return data.data();
    }
};

// Generic function to free an Image
inline void free_image(Image* image) {
    if (image != nullptr) {
        delete image;
    }
}
