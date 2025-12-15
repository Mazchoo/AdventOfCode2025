// Day04
#include <vector>
#include <string_view>

namespace day04
{
    // Binary image class for WASM bindings
    class RollsImage {
    private:
        std::vector<uint8_t> data;
        size_t pitch;
        
    public:
        // Constructor
        RollsImage(std::vector<uint8_t> image_data, size_t image_pitch)
            : data(std::move(image_data)), pitch(image_pitch) {}
        
        // Default constructor
        RollsImage() : data(), pitch(0) {}
        
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
    RollsImage* create_rolls_image(std::string_view payload) {
        auto [data, pitch] = parse_rolls_image(payload);
        return new RollsImage(std::move(data), pitch);
    }
    
    // Factory function to create a BinaryImage with specified size
    RollsImage* create_rolls_image_sized(size_t width, size_t height) {
        std::vector<uint8_t> data(width * height, 0);
        return new RollsImage(std::move(data), width);
    }
    
    // Function to free a BinaryImage
    void free_rolls_image(RollsImage* image) {
        if (image != nullptr) {
            delete image;
        }
    }
    
    // Function to get pitch from image pointer
    size_t get_image_pitch(RollsImage* image) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_pitch();
    }
    
    // Function to get height from image pointer
    size_t get_image_height(RollsImage* image) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_height();
    }
    
    // Function to get size from image pointer
    size_t get_image_size(RollsImage* image) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_size();
    }
    
    // Function to get element via image pointer
    uint8_t get_image_element(RollsImage* image, size_t index) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_element(index);
    }
    
    // Function to set element via image pointer
    bool set_image_element(RollsImage* image, size_t index, uint8_t value) {
        if (image == nullptr) {
            return false;
        }
        return image->set_element(index, value);
    }
    
    // Function to get pixel via image pointer
    uint8_t get_image_pixel(RollsImage* image, size_t row, size_t col) {
        if (image == nullptr) {
            return 0;
        }
        return image->get_pixel(row, col);
    }
    
    // Function to set pixel via image pointer
    bool set_image_pixel(RollsImage* image, size_t row, size_t col, uint8_t value) {
        if (image == nullptr) {
            return false;
        }
        return image->set_pixel(row, col, value);
    }
    
    // Function to get raw data pointer from image
    uint8_t* get_image_data_ptr(RollsImage* image) {
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
    bool remove_generation_from_image(RollsImage* image) {
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

    uint32_t remove_all_dead_cells(RollsImage* image) {
        if (image == nullptr)
            return false;

        return remove_deal_cells(image->get_data(), image->get_pitch());
    }
}