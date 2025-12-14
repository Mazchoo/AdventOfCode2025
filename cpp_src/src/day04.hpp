// Day04
#include <vector>
#include <string_view>
#include <utility>
#include <cstdint>

namespace day04
{
    // Parses a binary image string into a single-dimension vector with pitch
    // Format: "..@@.@@@@.\n@@@.@.@.@@" where \n separates lines
    // Characters: '.' = 0 (off), '@' = 1 (on)
    // Returns: pair of (flattened image data, pitch/width)
    // e.g. "..@@\n@@.." would parse to ({0,0,1,1,1,1,0,0}, 4)
    std::pair<std::vector<uint8_t>, size_t> parse_binary_image(std::string_view payload) {
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
                        // ToDo - throw exception here
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

    // Remove binary objects with fewer than four neighbours
    // Image is edited in place
    void remove_generation(std::vector<uint8_t>& image, size_t pitch) {
        if (pitch == 0)
            return;

        size_t row = 0;
        size_t column = 0;
        size_t height = image.size() / pitch;

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

                if (nr_neighbours < 4)
                    pixel = 2;
            }


            column += 1;
            if (column == pitch) {
                column = 0;
                row += 1;
            }
        }
    }
}