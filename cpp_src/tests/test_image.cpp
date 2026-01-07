#include "doctest.h"
#include "../src/Image.hpp"

// LLM finds a bug
TEST_CASE("Image with incomplete data is padded with zeros") {
    // Create an image with pitch=3 but only 7 elements (not divisible by 3)
    // This represents corrupted or incomplete image data
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7};
    Image img(data, 3);

    size_t reported_height = img.get_height();
    size_t expected_size = reported_height * img.get_pitch();
    
    CHECK(expected_size == img.get_size());
    CHECK(img.get_data()[img.get_size() - 1] == 0);
}
