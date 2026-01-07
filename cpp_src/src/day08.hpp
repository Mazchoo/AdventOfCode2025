// Day08
#pragma once

#include <vector>
#include <string_view>
#include <cstdint>
#include "PointCloud.hpp"

namespace day08
{
    // Parses a list of 3D coordinates from a string and returns a PointCloud
    // Format: "162,817,812\n57,618,57\n906,360,560"
    // Each line contains three comma-separated integers representing x, y, z coordinates
    // Returns: PointCloud pointer containing the parsed coordinates
    PointCloud* parse_coordinates(std::string_view payload) {
        std::vector<int32_t> data;
        
        enum class State { SKIP_WHITESPACE, PARSE_X, PARSE_Y, PARSE_Z };
        State state = State::SKIP_WHITESPACE;
        
        int32_t x = 0, y = 0, z = 0;
        size_t point_count = 0;
        
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
                    data.push_back(x);
                    data.push_back(y);
                    data.push_back(z);
                    point_count++;
                    state = State::SKIP_WHITESPACE;
                }
                continue;
            }
        }
        
        // Handle the last coordinate if we ended while parsing Z
        if (state == State::PARSE_Z) {
            data.push_back(x);
            data.push_back(y);
            data.push_back(z);
            point_count++;
        }
        
        return new PointCloud(std::move(data), point_count);
    }
}
