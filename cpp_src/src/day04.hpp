// Day04
#include "Image.hpp"
#include <vector>
#include <string_view>

namespace day04
{
    // NB - In a real application all operations can fail and raising errors
    // means adding more controls, probably better returning error codes
    // for everything and mapping error codes in a seperate file
    //
    // Parses a binary image string into a single-dimension vector with pitch
    // Format: "..@@.@@@@.\n@@@.@.@.@@" where \n separates lines
    // Characters: '.' = 0 (off), '@' = 1 (on)
    // Returns: pair of (flattened image data, pitch/width)
    // e.g. "..@@\n@@.." would parse to ({0,0,1,1,1,1,0,0}, 4)
    std::pair<std::vector<uint8_t>, size_t> parse_rolls_image(std::string_view payload) {
        std::vector<uint8_t> result;
        size_t pitch = 0;
        
        enum class State { SKIP_WHITESPACE, PARSE_LINE };
        State state = State::SKIP_WHITESPACE;
        size_t current_line_length = 0;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (state == State::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    continue;
                
                current_line_length = 0;
                state = State::PARSE_LINE;
            }
            
            if (state == State::PARSE_LINE) {
                if (c == '.' || c == '@') {
                    result.push_back(c == '@' ? 1 : 0);
                    current_line_length++;
                    continue;
                } else if (c == '\n') {
                    // End of line - set pitch from first line
                    if (pitch == 0 && current_line_length > 0) {
                        pitch = current_line_length;
                    } else if (pitch != current_line_length) {
                        return {{}, 0};
                    }
                    
                    state = State::SKIP_WHITESPACE;
                }
            }
        }
        
        // Handle the last line if we ended while parsing
        if (state == State::PARSE_LINE && current_line_length > 0) {
            if (pitch == 0) {
                pitch = current_line_length;
            } else if (pitch != current_line_length) {
                // ToDo - throw exception here
                return {{}, 0};
            }
        }
        
        return {result, pitch};
    }
    
    // Factory function to create a BinaryImage from parsed data
    Image* create_rolls_image(std::string_view payload) {
        auto [data, pitch] = parse_rolls_image(payload);
        return new Image(std::move(data), pitch);
    }
    
    // Factory function to create a BinaryImage with specified size
    Image* create_rolls_image_sized(size_t width, size_t height) {
        std::vector<uint8_t> data(width * height, 0);
        return new Image(std::move(data), width);
    }
    
    // Function to free a BinaryImage (uses generic free_image)
    inline void free_rolls_image(Image* image) {
        free_image(image);
    }
    
    // Function to get pitch from image pointer
    size_t get_image_pitch(Image* image) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_pitch();
    }
    
    // Function to get height from image pointer
    size_t get_image_height(Image* image) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_height();
    }
    
    // Function to get size from image pointer
    size_t get_image_size(Image* image) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_size();
    }
    
    // Function to get element via image pointer
    uint8_t get_image_element(Image* image, size_t index) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_element(index);
    }
    
    // Function to set element via image pointer
    bool set_image_element(Image* image, size_t index, uint8_t value) {
        if (image == nullptr) {
            return false;
        }
        return image->set_element(index, value);
    }
    
    // Function to get pixel via image pointer
    uint8_t get_image_pixel(Image* image, size_t row, size_t col) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_pixel(row, col);
    }
    
    // Function to set pixel via image pointer
    bool set_image_pixel(Image* image, size_t row, size_t col, uint8_t value) {
        if (image == nullptr) {
            return false;
        }
        return image->set_pixel(row, col, value);
    }
    
    // Function to get raw data pointer from image
    uint8_t* get_image_data_ptr(Image* image) {
        if (image == nullptr) {
            return nullptr;
        }
        return image->get_data_ptr();
    }

    // Remove cells with fewer than four neighbours
    // Image is edited in place
    // Returns true if any change was made
    bool remove_generation(std::vector<uint8_t>& image, size_t pitch) {
        if (pitch == 0)
            return false;

        size_t row = 0;
        size_t column = 0;
        size_t height = image.size() / pitch;
        bool removal_made = false;

        for (auto& pixel : image) {
            if (pixel == 1) {
                int nr_neighbours = 0;
                
                // Check all 8 neighbors (up, down, left, right, and 4 diagonals)
                // Top-left
                if (row > 0 && column > 0 && image[(row - 1) * pitch + (column - 1)] > 0)
                    nr_neighbours++;
                // Top
                if (row > 0 && image[(row - 1) * pitch + column] > 0)
                    nr_neighbours++;
                // Top-right
                if (row > 0 && column < pitch - 1 && image[(row - 1) * pitch + (column + 1)] > 0)
                    nr_neighbours++;
                // Left
                if (column > 0 && image[row * pitch + (column - 1)] > 0)
                    nr_neighbours++;
                // Right
                if (column < pitch - 1 && image[row * pitch + (column + 1)] > 0)
                    nr_neighbours++;
                // Bottom-left
                if (row < height - 1 && column > 0 && image[(row + 1) * pitch + (column - 1)] > 0)
                    nr_neighbours++;
                // Bottom
                if (row < height - 1 && image[(row + 1) * pitch + column] > 0)
                    nr_neighbours++;
                // Bottom-right
                if (row < height - 1 && column < pitch - 1 && image[(row + 1) * pitch + (column + 1)] > 0)
                    nr_neighbours++;

                if (nr_neighbours < 4) {
                    pixel = 2;
                    removal_made = true;
                }
            }


            column += 1;
            if (column == pitch) {
                column = 0;
                row += 1;
            }
        }
        return removal_made;
    }
    
    // Wrapper function to apply remove_generation to a BinaryImage object
    bool remove_generation_from_image(Image* image) {
        if (image == nullptr)
            return false;

        return remove_generation(image->get_data(), image->get_pitch());
    }

    // Remove cells designated for removal
    // Returns number of cells removed
    uint32_t remove_deal_cells(std::vector<uint8_t>& image, size_t pitch) {
        uint32_t result = 0;
        for (auto& pixel : image) {
            if (pixel == 2) {
                pixel = 0;
                result++;
            }
        }
        return result;
    }

    uint32_t remove_all_dead_cells(Image* image) {
        if (image == nullptr)
            return false;

        return remove_deal_cells(image->get_data(), image->get_pitch());
    }
}