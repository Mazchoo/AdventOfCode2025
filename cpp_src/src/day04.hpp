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
}