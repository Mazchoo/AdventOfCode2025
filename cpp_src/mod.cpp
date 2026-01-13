#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define EXPORT_FUNC EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT_FUNC
#endif

#include <string_view>

#include "src/Image.hpp"
#include "src/PointCloud.hpp"
#include "src/day00.hpp"
#include "src/day01.hpp"
#include "src/day02.hpp"
#include "src/day03.hpp"
#include "src/day04.hpp"
#include "src/day05.hpp"
#include "src/day06.hpp"
#include "src/day07.hpp"
#include "src/day08.hpp"
#include "src/day09.hpp"

extern "C" {
    // Day09 pt 2
    EXPORT_FUNC
    uint64_t get_largest_rect_within_contour(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day09::largest_rectangle_within_contour(payload);
    }

    // Day09 pt 1
    EXPORT_FUNC
    uint64_t get_largest_rect(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day09::largest_rectangle_area(payload);
    }

    // Day08 specific - Connects closest points in point cloud
    EXPORT_FUNC
    uint32_t connect_closest_points(void* cloud, uint32_t nr_connections) {
        auto pc = static_cast<PointCloud*>(cloud);
        return day08::connect_closest_points(pc, nr_connections);
    }

    // Day08 specific - Connects points until fully connected
    EXPORT_FUNC
    uint64_t connect_until_saturated(void* cloud) {
        auto pc = static_cast<PointCloud*>(cloud);
        return day08::connect_until_saturated(pc);
    }

    // Day08 specific - Creates point cloud from day08 payload
    EXPORT_FUNC
    void* create_point_cloud_day8(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return static_cast<void*>(day08::parse_coordinates(payload));
    }

    // Day07 part 2
    EXPORT_FUNC
    uint64_t calculate_nr_splitting_paths(void* image) {
        auto cast_image = static_cast<Image*>(image);
        return day07::calculate_nr_splitting_paths(cast_image);
    }
    
    // Day07 part 1
    EXPORT_FUNC
    uint32_t simulate_splitting_lasers(void* image) {
        auto cast_image = static_cast<Image*>(image);
        return day07::simulate_splitting_lasers(cast_image);
    }

    // Day07 specific - Creates image from day07 payload
    EXPORT_FUNC
    void* create_image_day7(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return static_cast<void*>(day07::create_laser_image(payload));
    }

    // Day06 part 2
    EXPORT_FUNC
    uint64_t calculate_homework_total_vertical(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day06::calculate_homework_total_vertical(payload);
    }

    // Day06 part 1
    EXPORT_FUNC
    uint64_t calculate_homework_total(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day06::calculate_homework_total(payload);
    }

    // Day05 part 2
    EXPORT_FUNC
    uint64_t count_all_valid_ids(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day05::count_all_valid_ids(payload);
    }

    // Day05 part 1
    EXPORT_FUNC
    uint32_t count_ids_in_range(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day05::count_ids_in_range(payload);
    }

    // Day04 specific - Removes a generation and returns cells removed & edits image in place
    EXPORT_FUNC
    uint32_t remove_dead_cells(void* image) {
        auto cast_image = static_cast<Image*>(image);
        if (!day04::remove_generation_from_image(cast_image))
            return 0;
        return day04::remove_all_dead_cells(cast_image);
    }

    // Day04 specific - Creates image from day04 payload
    EXPORT_FUNC
    void* create_image_day4(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return static_cast<void*>(day04::create_rolls_image(payload));
    }
    
    // Day04 specific - Applies remove_generation algorithm to an image
    EXPORT_FUNC
    bool remove_generation_from_image(void* image) {
        return day04::remove_generation_from_image(static_cast<Image*>(image));
    }

    // Day03 part 2
    EXPORT_FUNC
    uint64_t get_highest_12_digits(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day03::day03_pt2(payload);
    }

    // Day03 part 1
    EXPORT_FUNC
    uint32_t get_highest_digit_pair(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day03::day03_pt1(payload);
    }

    // Day02 part 2
    EXPORT_FUNC
    uint64_t count_all_repeated_digits(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day02::day02_pt2(payload);
    }

    // Day02 part 1
    EXPORT_FUNC
    uint64_t count_repeated_digits(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day02::day02_pt1(payload);
    }

    // Day01 part 2
    EXPORT_FUNC
    uint32_t count_passing_zeros(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day01::day01_pt2(payload);
    }

    // Day01 part 1
    EXPORT_FUNC
    uint32_t count_total_zeros(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day01::day01_pt1(payload);
    }

    // Simple addition function
    EXPORT_FUNC
    int add(int a, int b) {
        return day00::add(a, b);
    }
    
    // Simple multiplication function
    EXPORT_FUNC
    int multiply(int a, int b) {
        return a * b;
    }
    
    // Factorial function
    EXPORT_FUNC
    int factorial(int n) {
        if (n <= 1) {
            return 1;
        }
        return n * factorial(n - 1);
    }
    
    // Power function
    EXPORT_FUNC
    double power(double base, int exponent) {
        double result = 1.0;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        return result;
    }
    
    // Float vector functions for WASM bindings
    
    // Creates a new float vector of specified size
    EXPORT_FUNC
    void* create_float_vector(int size) {
        return static_cast<void*>(day00::create_float_vector(size));
    }
    
    // Frees memory allocated for a float vector
    EXPORT_FUNC
    void free_float_vector(void* vector) {
        day00::free_float_vector(static_cast<day00::FloatVector*>(vector));
    }
    
    // Sums all elements in a float vector
    EXPORT_FUNC
    float sum_float_vector(void* vector) {
        return day00::sum_float_vector(static_cast<day00::FloatVector*>(vector));
    }
    
    // Sets a value at specified index in float vector
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_vector_element(void* vector, int index, float value) {
        return day00::set_vector_element(static_cast<day00::FloatVector*>(vector), index, value) ? 1 : 0;
    }
    
    // Gets a value at specified index in float vector
    EXPORT_FUNC
    float get_vector_element(void* vector, int index) {
        return day00::get_vector_element(static_cast<day00::FloatVector*>(vector), index);
    }
    
    // Raw float array functions for WASM bindings (performance comparison)
    
    // Creates a new float array of specified size
    EXPORT_FUNC
    float* create_float_array(int size) {
        return day00::create_float_array(size);
    }
    
    // Frees memory allocated for a float array
    EXPORT_FUNC
    void free_float_array(float* ptr) {
        day00::free_float_array(ptr);
    }
    
    // Sums all elements in a float array
    EXPORT_FUNC
    float sum_float_array(float* ptr, int size) {
        return day00::sum_float_array(ptr, size);
    }
    
    // Sets a value at specified index in float array
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_array_element(float* ptr, int index, float value) {
        return day00::set_array_element(ptr, index, value) ? 1 : 0;
    }
    
    // Gets a value at specified index in float array
    EXPORT_FUNC
    float get_array_element(float* ptr, int index) {
        return day00::get_array_element(ptr, index);
    }
    
    // Raw byte array functions for WASM bindings (uint8_t operations)
    
    // Creates a new byte array of specified size
    EXPORT_FUNC
    uint8_t* create_byte_array(int size) {
        return day00::create_byte_array(size);
    }
    
    // Frees memory allocated for a byte array
    EXPORT_FUNC
    void free_byte_array(uint8_t* ptr) {
        day00::free_byte_array(ptr);
    }
    
    // Sums all elements in a byte array (returns int to avoid overflow)
    EXPORT_FUNC
    int sum_byte_array(uint8_t* ptr, int size) {
        return day00::sum_byte_array(ptr, size);
    }

    // SIMD-optimized version of sum_byte_array (WASM only)
    EXPORT_FUNC
    int sum_byte_array_simd(uint8_t* ptr, int size) {
        return day00::sum_byte_array_simd(ptr, size);
    }
    
    // Sets a value at specified index in byte array
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_byte_array_element(uint8_t* ptr, int index, uint8_t value) {
        return day00::set_byte_array_element(ptr, index, value) ? 1 : 0;
    }
    
    // Gets a value at specified index in byte array
    EXPORT_FUNC
    uint8_t get_byte_array_element(uint8_t* ptr, int index) {
        return day00::get_byte_array_element(ptr, index);
    }
    
    // ============================================================================
    // Generic Image Functions (using Image class directly)
    // ============================================================================
    
    // Creates a new blank image with specified dimensions
    EXPORT_FUNC
    void* create_blank_image(int width, int height) {
        std::vector<uint8_t> data(width * height, 0);
        return static_cast<void*>(new Image(std::move(data), width));
    }
    
    // Frees memory allocated for an image
    EXPORT_FUNC
    void free_image(void* image) {
        ::free_image(static_cast<Image*>(image));
    }
    
    // Gets the pitch (width) of an image
    EXPORT_FUNC
    int get_image_pitch(void* image) {
        auto img = static_cast<Image*>(image);
        return static_cast<int>(img->get_pitch());
    }
    
    // Gets the height of an image
    EXPORT_FUNC
    int get_image_height(void* image) {
        auto img = static_cast<Image*>(image);
        return static_cast<int>(img->get_height());
    }
    
    // Gets the total size of an image
    EXPORT_FUNC
    int get_image_size(void* image) {
        auto img = static_cast<Image*>(image);
        return static_cast<int>(img->get_size());
    }
    
    // Gets a value at specified index in image
    EXPORT_FUNC
    uint8_t get_image_element(void* image, int index) {
        auto img = static_cast<Image*>(image);
        return img->get_element(static_cast<size_t>(index));
    }
    
    // Sets a value at specified index in image
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_image_element(void* image, int index, uint8_t value) {
        auto img = static_cast<Image*>(image);
        return img->set_element(static_cast<size_t>(index), value) ? 1 : 0;
    }
    
    // Gets a pixel value at specified row and column in image
    EXPORT_FUNC
    uint8_t get_image_pixel(void* image, int row, int col) {
        auto img = static_cast<Image*>(image);
        return img->get_pixel(static_cast<size_t>(row), static_cast<size_t>(col));
    }
    
    // Sets a pixel value at specified row and column in image
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_image_pixel(void* image, int row, int col, uint8_t value) {
        auto img = static_cast<Image*>(image);
        return img->set_pixel(static_cast<size_t>(row), static_cast<size_t>(col), value) ? 1 : 0;
    }
    
    // Gets raw data pointer from image (for numpy integration)
    EXPORT_FUNC
    uint8_t* get_image_data_ptr(void* image) {
        auto img = static_cast<Image*>(image);
        return img->get_data_ptr();
    }
    
    // ============================================================================
    // Generic PointCloud Functions (using PointCloud class directly)
    // ============================================================================
    
    // Creates a new blank point cloud with specified number of points
    EXPORT_FUNC
    void* create_blank_point_cloud(int num_points) {
        std::vector<int32_t> data(num_points * 3, 0);
        return static_cast<void*>(new PointCloud(std::move(data)));
    }
    
    // Frees memory allocated for a point cloud
    EXPORT_FUNC
    void free_point_cloud(void* cloud) {
        ::free_point_cloud(static_cast<PointCloud*>(cloud));
    }
    
    // Gets the number of points in a point cloud
    EXPORT_FUNC
    int get_point_cloud_num_points(void* cloud) {
        auto pc = static_cast<PointCloud*>(cloud);
        return static_cast<int>(pc->get_num_points());
    }
    
    // Gets a value at specified index in point cloud
    EXPORT_FUNC
    int32_t get_point_cloud_element(void* cloud, int index) {
        auto pc = static_cast<PointCloud*>(cloud);
        return pc->get_element(static_cast<size_t>(index));
    }
    
    // Sets a value at specified index in point cloud
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_point_cloud_element(void* cloud, int index, int32_t value) {
        auto pc = static_cast<PointCloud*>(cloud);
        return pc->set_element(static_cast<size_t>(index), value) ? 1 : 0;
    }
    
    // Gets a coordinate value at specified point index and coordinate (0=x, 1=y, 2=z)
    EXPORT_FUNC
    int32_t get_point_cloud_point(void* cloud, int point_index, int coord) {
        auto pc = static_cast<PointCloud*>(cloud);
        return pc->get_point(static_cast<size_t>(point_index), static_cast<size_t>(coord));
    }
    
    // Sets a coordinate value at specified point index and coordinate (0=x, 1=y, 2=z)
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_point_cloud_point(void* cloud, int point_index, int coord, int32_t value) {
        auto pc = static_cast<PointCloud*>(cloud);
        return pc->set_point(static_cast<size_t>(point_index), static_cast<size_t>(coord), value) ? 1 : 0;
    }
    
    // Gets raw data pointer from point cloud (for numpy integration)
    EXPORT_FUNC
    int32_t* get_point_cloud_data_ptr(void* cloud) {
        auto pc = static_cast<PointCloud*>(cloud);
        return const_cast<int32_t*>(pc->get_data_ptr());
    }
    
    // Gets the number of edges in a point cloud
    EXPORT_FUNC
    int get_point_cloud_num_edges(void* cloud) {
        auto pc = static_cast<PointCloud*>(cloud);
        return static_cast<int>(pc->get_edges().size());
    }
    
    // Gets raw edges data pointer from point cloud (for numpy integration)
    // Returns pointer to flattened array of edge pairs as uint32_t
    // Note: WASM uses 32-bit size_t, so we explicitly return uint32_t* for clarity
    EXPORT_FUNC
    uint32_t* get_point_cloud_edges_ptr(void* cloud) {
        auto pc = static_cast<PointCloud*>(cloud);
        auto& edges = pc->get_edges();
        return reinterpret_cast<uint32_t*>(edges.data());
    }
}
