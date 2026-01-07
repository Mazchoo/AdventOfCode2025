// Day08
#pragma once

#include <vector>
#include <string_view>
#include <cstdint>

namespace day08
{
    // Structure to hold a 3D coordinate
    struct Coordinate3D {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        
        Coordinate3D(uint32_t x_, uint32_t y_, uint32_t z_) : x(x_), y(y_), z(z_) {}
    };

    // Parses a list of 3D coordinates from a string
    // Format: "162,817,812\n57,618,57\n906,360,560"
    // Each line contains three comma-separated integers representing x, y, z coordinates
    // Returns: vector of Coordinate3D structures
    std::vector<Coordinate3D> parse_coordinates(std::string_view payload) {
        std::vector<Coordinate3D> result;
        
        enum class State { SKIP_WHITESPACE, PARSE_X, PARSE_Y, PARSE_Z };
        State state = State::SKIP_WHITESPACE;
        
        int32_t x = 0, y = 0, z = 0;
        bool is_negative = false;
        
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
                continue;
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
                    result.emplace_back(x, y, z);
                    state = State::SKIP_WHITESPACE;
                }
                continue;
            }
        }
        
        // Handle the last coordinate if we ended while parsing Z
        if (state == State::PARSE_Z) {
            result.emplace_back(x, y, z);
        }
        
        return result;
    }
}
