// Day07
#pragma once

#include "Image.hpp"
#include <vector>
#include <string_view>
#include <utility>

int MAX_ITERATIONS = 1'000'000;

namespace day07
{
    // Pixel state enum with zero-cost abstraction (stored as uint8_t)
    enum class PixelState : uint8_t {
        Space = 0,
        Splitter = 1,
        Start = 2,
        Laser = 3
    };

    // Parses a laser grid image string into a single-dimension vector with pitch
    // Format: ".......S.......\n...............\n.......^......." where \n separates lines
    // Characters: '.' = 0 (Space), '^' = 1 (Splitter), 'S' = 2 (Start), '|' = 3 (Laser)
    // Returns: pair of (flattened image data, pitch/width)
    std::pair<std::vector<uint8_t>, size_t> parse_laser_image(std::string_view payload) {
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
                if (c == '.' || c == '^' || c == 'S' || c == '|') {
                    // Map characters to PixelState enum values
                    uint8_t pixel_value;
                    switch (c) {
                        case '.':
                            pixel_value = static_cast<uint8_t>(PixelState::Space);
                            break;
                        case '^':
                            pixel_value = static_cast<uint8_t>(PixelState::Splitter);
                            break;
                        case 'S':
                            pixel_value = static_cast<uint8_t>(PixelState::Start);
                            break;
                        case '|':
                            pixel_value = static_cast<uint8_t>(PixelState::Laser);
                            break;
                        default:
                            pixel_value = static_cast<uint8_t>(PixelState::Space);
                            break;
                    }
                    
                    result.push_back(pixel_value);
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
    
    // Find the first location of PixelState::Start in the image
    // Returns the index of the first Start pixel, or -1 if not found
    inline size_t find_start_position(const std::vector<uint8_t>& data) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] == static_cast<uint8_t>(PixelState::Start))
                return i;
        }
        return -1;
    }
    
    // Factory function to create an Image from parsed data
    Image* create_laser_image(std::string_view payload) {
        auto [data, pitch] = parse_laser_image(payload);
        return new Image(std::move(data), pitch);
    }
    
    // Factory function to create an Image with specified size
    Image* create_laser_image_sized(size_t width, size_t height) {
        std::vector<uint8_t> data(width * height, static_cast<uint8_t>(PixelState::Space));
        return new Image(std::move(data), width);
    }

    // Keep on iterating on the image until at rest or the max iterations has been reached
    // return the number of times the laser was split
    uint32_t simulate_splitting_lasers(Image* laser_image) {
        uint32_t result = 0;
        std::vector<size_t> current_idx = {find_start_position(laser_image->get_data())};
        std::vector<size_t> new_idx = {};

        if (current_idx[0] == -1)
            return result;

        int iteration = 0;
        while (iteration < MAX_ITERATIONS) {
            new_idx.clear();
            bool any_change = false;

            for (auto& ind: current_idx) {
                auto new_ind = ind + laser_image->get_pitch();
                if (new_ind >= laser_image->get_size())
                    continue;
    
                auto new_pixel = laser_image->get_element(new_ind);
                if (new_pixel == static_cast<uint8_t>(PixelState::Space)) {
                    new_idx.push_back(new_ind);
                    laser_image->set_element(new_ind, static_cast<uint8_t>(PixelState::Laser));
                    any_change = true;
                } else if (new_pixel == static_cast<uint8_t>(PixelState::Splitter)) {
                    auto new_column = new_ind % laser_image->get_pitch();
                    result++;
                    if (new_column > 0 &&
                        laser_image->get_element(new_ind - 1) == static_cast<uint8_t>(PixelState::Space)
                    )
                    {
                        new_idx.push_back(new_ind - 1);
                        laser_image->set_element(new_ind - 1, static_cast<uint8_t>(PixelState::Laser));
                        any_change = true;
                    }
                    if (new_column < laser_image->get_pitch() - 1 &&
                        laser_image->get_element(new_ind + 1) == static_cast<uint8_t>(PixelState::Space)
                    )
                    {
                        new_idx.push_back(new_ind + 1);
                        laser_image->set_element(new_ind + 1, static_cast<uint8_t>(PixelState::Laser));
                        any_change = true;
                    }
                }
            }

            if (new_idx.size() == 0 || any_change == false)
                break;

            current_idx = new_idx;
            iteration++;
        }

        return result;
    }
}
