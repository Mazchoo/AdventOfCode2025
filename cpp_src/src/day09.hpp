// Day09
#pragma once

#include <vector>
#include <string_view>
#include <cstdint>

namespace day09
{
    using Coordinate = std::pair<int32_t, int32_t>;
    using Rect = std::pair<Coordinate, Coordinate>;

    // Returns true if three coordinates form a clockwise triangle, false if counter-clockwise
    // Uses the cross product of vectors (p2-p1) and (p3-p1)
    // If cross product > 0: counter-clockwise, < 0: clockwise, = 0: collinear
    inline bool is_clockwise(const Coordinate& p1, const Coordinate& p2, const Coordinate& p3) {
        int64_t cross_product = static_cast<int64_t>(p2.first - p1.first) * (p3.second - p1.second)
                              - static_cast<int64_t>(p2.second - p1.second) * (p3.first - p1.first);
        return cross_product < 0;
    }

    // Helper function to get a Rect from three coordinates that form two lines at right angles
    // The middle coordinate is at the right angle corner
    // The rect is inside the three points and doesn't touch the lines between them
    inline Rect bounded_rect(const Coordinate& p1, const Coordinate& p2, const Coordinate& p3) {
        // p2 is the corner at right angles
        // Determine which coordinates form the bounding box
        int32_t min_x = std::min({p1.first, p2.first, p3.first});
        int32_t max_x = std::max({p1.first, p2.first, p3.first});
        int32_t min_y = std::min({p1.second, p2.second, p3.second});
        int32_t max_y = std::max({p1.second, p2.second, p3.second});
        
        // Adjust inward by 1 to not touch the lines, but only on sides where p2 is at the boundary
        // If p2 is at min_x, adjust min_x inward (increase it)
        if (p2.first == min_x) {
            min_x += 1;
        } else if (p2.first == max_x) {
            // If p2 is at max_x, adjust max_x inward (decrease it)
            max_x -= 1;
        }
        
        // If p2 is at min_y, adjust min_y inward (increase it)
        if (p2.second == min_y) {
            min_y += 1;
        } else if (p2.second == max_y) {
            // If p2 is at max_y, adjust max_y inward (decrease it)
            max_y -= 1;
        }
        
        return {{min_x, min_y}, {max_x, max_y}};
    }

    // Find the bounding rectangle from three coordinates
    inline Rect find_bounding_rect(const Coordinate& p1, const Coordinate& p2, const Coordinate& p3) {
        int32_t min_x = std::min({p1.first, p2.first, p3.first});
        int32_t max_x = std::max({p1.first, p2.first, p3.first});
        int32_t min_y = std::min({p1.second, p2.second, p3.second});
        int32_t max_y = std::max({p1.second, p2.second, p3.second});
        
        return {{min_x, min_y}, {max_x, max_y}};
    }

    // Parses a list of 2D coordinates from a string and returns a vector of pairs
    // Format: "7,1\n11,1\n11,7\n9,7\n9,5\n2,5\n2,3\n7,3"
    // Each line contains two comma-separated integers representing x, y coordinates
    // Returns: Vector of pairs containing the parsed coordinates
    std::vector<Coordinate> parse_coordinates(std::string_view payload) {
        std::vector<Coordinate> data;
        
        enum class State { SKIP_WHITESPACE, PARSE_X, PARSE_Y };
        State state = State::SKIP_WHITESPACE;
        
        int32_t x = 0, y = 0;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (state == State::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    continue;
                
                // Start parsing a new coordinate
                x = 0;
                y = 0;
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
                } else if (c == '\n' || c == '\r') {
                    data.push_back({x, y});
                    state = State::SKIP_WHITESPACE;
                }
                continue;
            }
        }
        
        // Handle the last coordinate if we ended while parsing Y
        if (state == State::PARSE_Y) {
            data.push_back({x, y});
        }
        
        return data;
    }

    // Find the largest rectangle from a pair of coordinates
    uint64_t largest_rectangle_area(std::string_view payload) {
        auto coordinates = parse_coordinates(payload);

        uint64_t result = 0;
        for (size_t i = 0; i < coordinates.size(); i++) {
            for (size_t j = i + 1; j < coordinates.size(); j++) {
                uint64_t dx = static_cast<uint64_t>(std::abs(coordinates[i].first - coordinates[j].first)) + 1;
                uint64_t dy = static_cast<uint64_t>(std::abs(coordinates[i].second - coordinates[j].second)) + 1;
                result = std::max<uint64_t>(result, dx * dy);
            }
        }

        return result;
    }
}
