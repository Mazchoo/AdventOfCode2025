#include "doctest.h"
#include "day09.hpp"

using namespace day09;

// Test suite for parse_coordinates function
TEST_SUITE("Day09 Tests") {
    TEST_CASE("parse_coordinates - sample input") {
        std::string_view input = "7,1\n11,1\n11,7\n9,7\n9,5\n2,5\n2,3\n7,3";
        auto result = largest_rectangle_within_contour(input);

        REQUIRE(result == 24);

    }

    TEST_CASE("parse_coordinates - single coordinate") {
        std::string_view input = "7,1";
        auto coords = parse_coordinates(input);
        
        REQUIRE(coords.size() == 1);
        CHECK(coords[0].first == 7);   // x
        CHECK(coords[0].second == 1);  // y
    }
    
    TEST_CASE("parse_coordinates - multiple coordinates") {
        std::string_view input = "7,1\n11,1\n11,7";
        auto coords = parse_coordinates(input);
        
        REQUIRE(coords.size() == 3);
        
        // First coordinate
        CHECK(coords[0].first == 7);
        CHECK(coords[0].second == 1);
        
        // Second coordinate
        CHECK(coords[1].first == 11);
        CHECK(coords[1].second == 1);
        
        // Third coordinate
        CHECK(coords[2].first == 11);
        CHECK(coords[2].second == 7);
    }
    
    TEST_CASE("parse_coordinates - sample input") {
        std::string_view input = "7,1\n11,1\n11,7\n9,7\n9,5\n2,5\n2,3\n7,3";
        auto coords = parse_coordinates(input);
        
        REQUIRE(coords.size() == 8);
        
        CHECK(coords[0].first == 7);
        CHECK(coords[0].second == 1);
        
        CHECK(coords[7].first == 7);
        CHECK(coords[7].second == 3);
    }
    
    TEST_CASE("parse_coordinates - no trailing newline") {
        std::string_view input = "9,7\n9,5\n2,5";
        auto coords = parse_coordinates(input);
        
        REQUIRE(coords.size() == 3);
        
        // Verify last coordinate is parsed correctly
        CHECK(coords[2].first == 2);
        CHECK(coords[2].second == 5);
    }
    
    TEST_CASE("parse_coordinates - with whitespace") {
        std::string_view input = "  \n7,1\n\n11,7\n  ";
        auto coords = parse_coordinates(input);
        
        REQUIRE(coords.size() == 2);
        
        CHECK(coords[0].first == 7);
        CHECK(coords[0].second == 1);
        
        CHECK(coords[1].first == 11);
        CHECK(coords[1].second == 7);
    }
    
    TEST_CASE("parse_coordinates - zero values") {
        std::string_view input = "0,0\n1,2";
        auto coords = parse_coordinates(input);
        
        REQUIRE(coords.size() == 2);
        
        CHECK(coords[0].first == 0);
        CHECK(coords[0].second == 0);
        
        CHECK(coords[1].first == 1);
        CHECK(coords[1].second == 2);
    }
    
    TEST_CASE("is_clockwise - counter-clockwise  triangle") {
        // Triangle with vertices at (0,0), (4,0), (2,2)
        // When traversed in this order, forms a clockwise triangle
        Coordinate p1 = {0, 0};
        Coordinate p2 = {4, 0};
        Coordinate p3 = {2, 2};
        
        CHECK(is_clockwise(p1, p2, p3) == false);
    }
    
    TEST_CASE("is_clockwise - clockwise triangle") {
        // Triangle with vertices at (0,0), (2,2), (4,0)
        // When traversed in this order, forms a counter-clockwise triangle
        Coordinate p1 = {0, 0};
        Coordinate p2 = {2, 2};
        Coordinate p3 = {4, 0};
        
        CHECK(is_clockwise(p1, p2, p3) == true);
    }
    
    TEST_CASE("bounded_rect: x+ y+") {
        // Small right angle forming a 3x3 bounding box
        Coordinate p1 = {0, 0};
        Coordinate p2 = {2, 0};
        Coordinate p3 = {2, 2};
        
        auto rect = bounded_rect(p1, p2, p3);
        
        // Bounding box is (0,1) to (1,2), adjusted inward by 1
        CHECK(rect.first.first == 0);    // min_x
        CHECK(rect.first.second == 1);   // min_y
        CHECK(rect.second.first == 1);   // max_x
        CHECK(rect.second.second == 2);  // max_y
    }

    TEST_CASE("bounded_rect: - y- x+") {
        // Small right angle forming a 3x3 bounding box
        Coordinate p1 = { 2, 2 };
        Coordinate p2 = { 2, 0 };
        Coordinate p3 = { 4, 0 };

        auto rect = bounded_rect(p1, p2, p3);

        // Bounding box is (3,1) to (4,2), adjusted inward by 1
        CHECK(rect.first.first == 3);    // min_x
        CHECK(rect.first.second == 1);   // min_y
        CHECK(rect.second.first == 4);   // max_x
        CHECK(rect.second.second == 2);  // max_y
    }

    TEST_CASE("bounded_rect: - y- x-") {
        // Small right angle forming a 3x3 bounding box
        Coordinate p1 = { 4, 0 };
        Coordinate p2 = { 2, 0 };
        Coordinate p3 = { 2, -2 };

        auto rect = bounded_rect(p1, p2, p3);

        // Bounding box is (3,-2) to (4,-1), adjusted inward by 1
        CHECK(rect.first.first == 3);    // min_x
        CHECK(rect.first.second == -2);   // min_y
        CHECK(rect.second.first == 4);   // max_x
        CHECK(rect.second.second == -1);  // max_y
    }

    TEST_CASE("bounded_rect: - y+ x-") {
        // Small right angle forming a 3x3 bounding box
        Coordinate p1 = { 2, -2 };
        Coordinate p2 = { 2, 0 };
        Coordinate p3 = { 0, 0 };

        auto rect = bounded_rect(p1, p2, p3);

        // Bounding box is (0,-2) to (1,-1), adjusted inward by 1
        CHECK(rect.first.first == 0);    // min_x
        CHECK(rect.first.second == -2);   // min_y
        CHECK(rect.second.first == 1);   // max_x
        CHECK(rect.second.second == -1);  // max_y
    }
    
    TEST_CASE("rects_intersect - overlapping rectangles") {
        // Rectangle 1: (0,0) to (10,10)
        Rect r1 = {{0, 0}, {10, 10}};
        
        // Rectangle 2: (5,5) to (15,15) - overlaps with r1
        Rect r2 = {{5, 5}, {15, 15}};
        
        CHECK(rects_intersect(r1, r2) == true);
        CHECK(rects_intersect(r2, r1) == true);  // Should be symmetric
    }
    
    TEST_CASE("rects_intersect - non-overlapping rectangles") {
        // Rectangle 1: (0,0) to (5,5)
        Rect r1 = {{0, 0}, {5, 5}};
        
        // Rectangle 2: (10,10) to (15,15) - does not overlap with r1
        Rect r2 = {{10, 10}, {15, 15}};
        
        CHECK(rects_intersect(r1, r2) == false);
        CHECK(rects_intersect(r2, r1) == false);  // Should be symmetric
    }

    TEST_CASE("rects_intersect - touching returns false") {
        // Rectangle 1: (0,0) to (5,5)
        Rect r1 = { {0, 0}, {5, 5} };

        // Rectangle 2: (10,10) to (15,15) - does not overlap with r1
        Rect r2 = { {6, 0}, {10, 5} };

        CHECK(rects_intersect(r1, r2) == false);
        CHECK(rects_intersect(r2, r1) == false);  // Should be symmetric
    }
}
