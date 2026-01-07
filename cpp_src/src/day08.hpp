// Day08
#pragma once

#include <vector>
#include <string_view>
#include <cstdint>
#include "PointCloud.hpp"

namespace day08
{
    // Structure to hold a 3D coordinate
    struct Coordinate3D {
        int32_t x;
        int32_t y;
        int32_t z;
        
        Coordinate3D(int32_t x_, int32_t y_, int32_t z_) : x(x_), y(y_), z(z_) {}
    };

    // Parses a list of 3D coordinates from a string and returns a PointCloud
    // Format: "162,817,812\n57,618,57\n906,360,560"
    // Each line contains three comma-separated integers representing x, y, z coordinates
    // Returns: PointCloud pointer containing the parsed coordinates
    PointCloud* parse_coordinates(std::string_view payload) {
        std::vector<Coordinate3D> result;
        
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
        
        // Convert to flat int32_t vector for PointCloud
        std::vector<int32_t> data;
        data.reserve(result.size() * 3);
        for (const auto& coord : result) {
            data.push_back(coord.x);
            data.push_back(coord.y);
            data.push_back(coord.z);
        }
        
        return new PointCloud(std::move(data), result.size());
    }
}
