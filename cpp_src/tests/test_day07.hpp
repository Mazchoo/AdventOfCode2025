#include "doctest.h"
#include "day07.hpp"

using namespace day07;

// Test suite for parse_laser_image function
TEST_SUITE("Day07 Tests") {
    TEST_CASE("parse_laser_image - single line") {
        std::string_view input = ".^S|";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 4);
        REQUIRE(data.size() == 4);
        
        CHECK(data[0] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[1] == static_cast<uint8_t>(PixelState::Splitter));
        CHECK(data[2] == static_cast<uint8_t>(PixelState::Start));
        CHECK(data[3] == static_cast<uint8_t>(PixelState::Laser));
    }
    
    TEST_CASE("parse_laser_image - single pixel") {
        std::string_view input = "S";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 1);
        REQUIRE(data.size() == 1);
        CHECK(data[0] == static_cast<uint8_t>(PixelState::Start));
    }
    
    TEST_CASE("parse_laser_image - with different symbols") {
        std::string_view input = R"(
        
.^S
.|.
...)";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 3);
        REQUIRE(data.size() == 9);
        
        CHECK(data[0] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[1] == static_cast<uint8_t>(PixelState::Splitter));
        CHECK(data[2] == static_cast<uint8_t>(PixelState::Start));
    }
    
    TEST_CASE("parse_laser_image - with trailing whitespace") {
        std::string_view input = R"(.^S
.|.
...
        
)";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 3);
        REQUIRE(data.size() == 9);
        
        CHECK(data[6] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[7] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[8] == static_cast<uint8_t>(PixelState::Space));
    }
    
    TEST_CASE("parse_laser_image - Windows line endings (CRLF)") {
        std::string_view input = ".^S\r\n.|.\r\n...\r\n";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 3);
        REQUIRE(data.size() == 9);
        
        CHECK(data[0] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[1] == static_cast<uint8_t>(PixelState::Splitter));
        CHECK(data[2] == static_cast<uint8_t>(PixelState::Start));
    }

    TEST_CASE("parse_laser_image - inconsistent line lengths (first line longer)") {
        std::string_view input = R"(.....
...
...)";
        auto [data, pitch] = parse_laser_image(input);
        
        CHECK(pitch == 0);
        CHECK(data.size() == 0);
    }
    
    TEST_CASE("parse_laser_image - inconsistent line lengths (last line different)") {
        std::string_view input = R"(...
...
.....)";
        auto [data, pitch] = parse_laser_image(input);
        
        CHECK(pitch == 0);
        CHECK(data.size() == 0);
    }
    
    TEST_CASE("parse_laser_image - empty string") {
        std::string_view input = "";
        auto [data, pitch] = parse_laser_image(input);
        
        CHECK(pitch == 0);
        CHECK(data.size() == 0);
    }
    
    TEST_CASE("parse_laser_image - only whitespace") {
        std::string_view input = "   \n\t\r\n   ";
        auto [data, pitch] = parse_laser_image(input);
        
        CHECK(pitch == 0);
        CHECK(data.size() == 0);
    }
    
    TEST_CASE("parse_laser_image - mixed whitespace between lines") {
        std::string_view input = "...\n\n\n...\n...\n";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 3);
        REQUIRE(data.size() == 9);
        
        for (size_t i = 0; i < data.size(); i++) {
            CHECK(data[i] == static_cast<uint8_t>(PixelState::Space));
        }
    }
    
    TEST_CASE("parse_laser_image - tabs and spaces before content") {
        std::string_view input = "\t  \n  \t.^S\n  \t.|.\n  \t...";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 3);
        REQUIRE(data.size() == 9);
        
        CHECK(data[0] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[1] == static_cast<uint8_t>(PixelState::Splitter));
        CHECK(data[2] == static_cast<uint8_t>(PixelState::Start));
    }
    
    TEST_CASE("parse_laser_image - multiple trailing newlines") {
        std::string_view input = ".^S\n.|.\n...\n\n\n";
        auto [data, pitch] = parse_laser_image(input);
        
        REQUIRE(pitch == 3);
        REQUIRE(data.size() == 9);
        
        CHECK(data[0] == static_cast<uint8_t>(PixelState::Space));
        CHECK(data[8] == static_cast<uint8_t>(PixelState::Space));
    }
}
