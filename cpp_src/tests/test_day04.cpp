#include "doctest.h"
#include "day04.hpp"

using namespace day04;

// Test suite for parse_binary_image function
TEST_SUITE("Day04 Tests") {
    TEST_CASE("parse_binary_image - basic example from documentation") {
        std::string_view input = "..@@\n@@..";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 8);
        REQUIRE(pitch == 4);
        
        // First line: ..@@
        CHECK(result[0] == 0);
        CHECK(result[1] == 0);
        CHECK(result[2] == 1);
        CHECK(result[3] == 1);
        
        // Second line: @@..
        CHECK(result[4] == 1);
        CHECK(result[5] == 1);
        CHECK(result[6] == 0);
        CHECK(result[7] == 0);
    }
    
    TEST_CASE("parse_binary_image - single line") {
        std::string_view input = ".@.@.";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 5);
        REQUIRE(pitch == 5);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 0);
        CHECK(result[3] == 1);
        CHECK(result[4] == 0);
    }
    
    TEST_CASE("parse_binary_image - empty string") {
        std::string_view input = "";
        auto [result, pitch] = parse_binary_image(input);
        
        CHECK(result.size() == 0);
        CHECK(pitch == 0);
    }
    
    TEST_CASE("parse_binary_image - single character dot") {
        std::string_view input = ".";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 1);
        REQUIRE(pitch == 1);
        CHECK(result[0] == 0);
    }
    
    TEST_CASE("parse_binary_image - leading whitespace") {
        std::string_view input = "  .@@\n  @@.";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 6);
        REQUIRE(pitch == 3);
        
        // First line: .@@
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        
        // Second line: @@.
        CHECK(result[3] == 1);
        CHECK(result[4] == 1);
        CHECK(result[5] == 0);
    }
    
    TEST_CASE("parse_binary_image - trailing newlines") {
        std::string_view input = ".@\n@.\n\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - Windows line endings (CRLF)") {
        std::string_view input = ".@\r\n@.\r\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - tabs and spaces") {
        std::string_view input = "\t.@\n  @.\t\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - multiple consecutive newlines") {
        std::string_view input = ".@\n\n\n@.";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - mixed whitespace") {
        std::string_view input = "  \n\t\n  .@@\n\n  @@.  \n\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 6);
        REQUIRE(pitch == 3);
        
        // First line: .@@
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        
        // Second line: @@.
        CHECK(result[3] == 1);
        CHECK(result[4] == 1);
        CHECK(result[5] == 0);
    }
    
    TEST_CASE("parse_binary_image - inconsistent pitch") {
        std::string_view input = "..\n@@@";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 0);
        REQUIRE(pitch == 0);
    }
    
    // Test suite for remove_generation function
    TEST_CASE("remove_generation - isolated pixel (0 neighbors)") {
        std::vector<uint8_t> image = {
            0, 0, 0,
            0, 1, 0,
            0, 0, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // Isolated pixel should be removed (set to 2)
        CHECK(changed == true);
        CHECK(image[4] == 2);
    }
    
    TEST_CASE("remove_generation - pixel with 1 neighbor") {
        std::vector<uint8_t> image = {
            0, 0, 0,
            0, 1, 1,
            0, 0, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // Both pixels have only 1 neighbor, should be removed
        CHECK(changed == true);
        CHECK(image[4] == 2);
        CHECK(image[5] == 2);
    }
    
    TEST_CASE("remove_generation - pixel with 2 neighbors") {
        std::vector<uint8_t> image = {
            0, 1, 0,
            0, 1, 1,
            0, 0, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // All pixels have < 4 neighbors, should be removed
        CHECK(changed == true);
        CHECK(image[1] == 2);  // top pixel (1 neighbor)
        CHECK(image[4] == 2);  // middle pixel (2 neighbors)
        CHECK(image[5] == 2);  // right pixel (2 neighbors)
    }
    
    TEST_CASE("remove_generation - pixel with 3 neighbors") {
        std::vector<uint8_t> image = {
            0, 1, 0,
            1, 1, 1,
            0, 0, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // All pixels have < 4 neighbors, should be removed
        CHECK(changed == true);
        CHECK(image[1] == 2);  // top pixel (2 neighbors)
        CHECK(image[3] == 2);  // left pixel (2 neighbors)
        CHECK(image[4] == 2);  // center pixel (3 neighbors)
        CHECK(image[5] == 2);  // right pixel (2 neighbors)
    }
    
    TEST_CASE("remove_generation - pixel with exactly 4 neighbors") {
        std::vector<uint8_t> image = {
            0, 1, 0,
            1, 1, 1,
            0, 1, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // Center pixel has exactly 4 neighbors, should NOT be removed
        CHECK(changed == true);  // Edge pixels are removed
        CHECK(image[4] == 1);
        // Edge pixels have < 4 neighbors, should be removed
        CHECK(image[1] == 2);  // top (2 neighbors)
        CHECK(image[3] == 2);  // left (2 neighbors)
        CHECK(image[5] == 2);  // right (2 neighbors)
        CHECK(image[7] == 2);  // bottom (2 neighbors)
    }
    
    TEST_CASE("remove_generation - pixel with 5+ neighbors") {
        std::vector<uint8_t> image = {
            1, 1, 1,
            1, 1, 1,
            0, 1, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // Center pixel has 5 neighbors, should NOT be removed
        CHECK(changed == true);  // Corner pixels are removed
        CHECK(image[4] == 1);
        // Top-left has 3 neighbors
        CHECK(image[0] == 2);
        // Top-center has 4 neighbors, should NOT be removed
        CHECK(image[1] == 1);
        // Top-right has 3 neighbors
        CHECK(image[2] == 2);
    }
    
    TEST_CASE("remove_generation - all 8 neighbors (maximum)") {
        std::vector<uint8_t> image = {
            1, 1, 1,
            1, 1, 1,
            1, 1, 1
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // Center pixel has 8 neighbors, should NOT be removed
        CHECK(changed == true);  // Corner pixels are removed
        CHECK(image[4] == 1);
        // Corner pixels have 3 neighbors each, should be removed
        CHECK(image[0] == 2);
        CHECK(image[2] == 2);
        CHECK(image[6] == 2);
        CHECK(image[8] == 2);
        // Edge pixels have 5 neighbors each, should NOT be removed
        CHECK(image[1] == 1);
        CHECK(image[3] == 1);
        CHECK(image[5] == 1);
        CHECK(image[7] == 1);
    }
    
    TEST_CASE("remove_generation - edge cases at boundaries") {
        std::vector<uint8_t> image = {
            1, 1, 0, 0,
            1, 1, 0, 0,
            0, 0, 1, 1,
            0, 0, 1, 1
        };
        size_t pitch = 4;
        
        bool changed = remove_generation(image, pitch);
        
        // Top-left corner: each pixel has 3 neighbors
        CHECK(changed == true);  // Corner pixels are removed
        CHECK(image[0] == 2);
        CHECK(image[1] == 2);
        CHECK(image[4] == 2);

        // four neighbors
        CHECK(image[5] == 1);
        CHECK(image[10] == 1);
        
        // Bottom-right block: each pixel has 3 neighbors
        CHECK(image[11] == 2);
        CHECK(image[14] == 2);
        CHECK(image[15] == 2);
    }
    
    TEST_CASE("remove_generation - diagonal neighbors count") {
        std::vector<uint8_t> image = {
            1, 0, 1, 0, 1,
            0, 1, 0, 1, 0,
            1, 0, 1, 0, 1,
            0, 1, 0, 1, 0,
            1, 0, 1, 0, 1
        };
        size_t pitch = 5;
        
        bool changed = remove_generation(image, pitch);
        
        // Center pixel at (2,2) has 4 diagonal neighbors
        CHECK(changed == true);  // Corner pixels are removed
        CHECK(image[12] == 1);  // Should NOT be removed
        
        // Corner pixels have 1 neighbor each
        CHECK(image[0] == 2);
        CHECK(image[4] == 2);
        CHECK(image[20] == 2);
        CHECK(image[24] == 2);
    }
    
    TEST_CASE("remove_generation - empty image") {
        std::vector<uint8_t> image = {};
        size_t pitch = 0;
        
        // Should not crash
        bool changed = remove_generation(image, pitch);
        
        CHECK(changed == false);  // No changes possible in empty image
        CHECK(image.size() == 0);
    }
    
    TEST_CASE("remove_generation - single pixel") {
        std::vector<uint8_t> image = {1};
        size_t pitch = 1;
        
        bool changed = remove_generation(image, pitch);
        
        // Single pixel has 0 neighbors, should be removed
        CHECK(changed == true);
        CHECK(image[0] == 2);
    }
    
    TEST_CASE("remove_generation - all zeros") {
        std::vector<uint8_t> image = {
            0, 0, 0,
            0, 0, 0,
            0, 0, 0
        };
        size_t pitch = 3;
        
        bool changed = remove_generation(image, pitch);
        
        // All pixels remain 0
        CHECK(changed == false);  // No changes made
        for (auto pixel : image) {
            CHECK(pixel == 0);
        }
    }
    
    TEST_CASE("remove_generation - rectangular image (non-square)") {
        std::vector<uint8_t> image = {
            1, 1, 1, 1, 1,
            1, 1, 1, 1, 1,
            1, 1, 1, 1, 1
        };
        size_t pitch = 5;
        
        bool changed = remove_generation(image, pitch);
        
        // Interior pixels have 8 neighbors, should NOT be removed
        CHECK(changed == true);  // Corner pixels are removed
        CHECK(image[6] == 1);   // (1,1)
        CHECK(image[7] == 1);   // (1,2)
        CHECK(image[8] == 1);   // (1,3)
        
        // Corner pixels have 3 neighbors, should be removed
        CHECK(image[0] == 2);   // top-left
        CHECK(image[4] == 2);   // top-right
        CHECK(image[10] == 2);  // bottom-left
        CHECK(image[14] == 2);  // bottom-right
        
        // Edge pixels have 5 neighbors, should NOT be removed
        CHECK(image[1] == 1);   // top edge
        CHECK(image[5] == 1);   // left edge
        CHECK(image[9] == 1);   // right edge
        CHECK(image[11] == 1);  // bottom edge
    }
}