#include "doctest.h"
#include "day08.hpp"
#include "PointCloud.hpp"

using namespace day08;

// Test suite for parse_coordinates function
TEST_SUITE("Day08 Tests") {
    TEST_CASE("test saturated return on sample input") {
        std::string_view input = R"("162,817,812
            57, 618, 57
            906, 360, 560
            592, 479, 940
            352, 342, 300
            466, 668, 158
            542, 29, 236
            431, 825, 988
            739, 650, 466
            52, 470, 668
            216, 146, 977
            819, 987, 18
            117, 168, 530
            805, 96, 715
            346, 949, 466
            970, 615, 88
            941, 993, 340
            862, 61, 35
            984, 92, 344
            425, 690, 689)";
        PointCloud* cloud = parse_coordinates(input);

        REQUIRE(cloud != nullptr);
        auto result = connect_until_saturated(cloud);
        CHECK(result == 25272);

        free_point_cloud(cloud);
    }

    TEST_CASE("test number of components sample input") {
        std::string_view input = R"("162,817,812
            57, 618, 57
            906, 360, 560
            592, 479, 940
            352, 342, 300
            466, 668, 158
            542, 29, 236
            431, 825, 988
            739, 650, 466
            52, 470, 668
            216, 146, 977
            819, 987, 18
            117, 168, 530
            805, 96, 715
            346, 949, 466
            970, 615, 88
            941, 993, 340
            862, 61, 35
            984, 92, 344
            425, 690, 689)";
        PointCloud* cloud = parse_coordinates(input);

        REQUIRE(cloud != nullptr);
        auto result = connect_closest_points(cloud, 10);
        CHECK(result == 40);

        free_point_cloud(cloud);
    }

    TEST_CASE("parse_coordinates - single coordinate") {
        std::string_view input = "162,817,812";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 1);
        
        CHECK(cloud->get_point(0, 0) == 162);  // x
        CHECK(cloud->get_point(0, 1) == 817);  // y
        CHECK(cloud->get_point(0, 2) == 812);  // z
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - multiple coordinates") {
        std::string_view input = "162,817,812\n57,618,57\n906,360,560";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 3);
        
        // First coordinate
        CHECK(cloud->get_point(0, 0) == 162);
        CHECK(cloud->get_point(0, 1) == 817);
        CHECK(cloud->get_point(0, 2) == 812);
        
        // Second coordinate
        CHECK(cloud->get_point(1, 0) == 57);
        CHECK(cloud->get_point(1, 1) == 618);
        CHECK(cloud->get_point(1, 2) == 57);
        
        // Third coordinate
        CHECK(cloud->get_point(2, 0) == 906);
        CHECK(cloud->get_point(2, 1) == 360);
        CHECK(cloud->get_point(2, 2) == 560);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - with leading whitespace") {
        std::string_view input = "  \n\t\n162,817,812\n57,618,57";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 2);
        
        CHECK(cloud->get_point(0, 0) == 162);
        CHECK(cloud->get_point(0, 1) == 817);
        CHECK(cloud->get_point(0, 2) == 812);
        
        CHECK(cloud->get_point(1, 0) == 57);
        CHECK(cloud->get_point(1, 1) == 618);
        CHECK(cloud->get_point(1, 2) == 57);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - with trailing whitespace") {
        std::string_view input = "162,817,812\n57,618,57\n\n  \t";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 2);
        
        CHECK(cloud->get_point(0, 0) == 162);
        CHECK(cloud->get_point(1, 0) == 57);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - Windows line endings (CRLF)") {
        std::string_view input = "162,817,812\r\n57,618,57\r\n906,360,560\r\n";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 3);
        
        CHECK(cloud->get_point(0, 0) == 162);
        CHECK(cloud->get_point(0, 1) == 817);
        CHECK(cloud->get_point(0, 2) == 812);
        
        CHECK(cloud->get_point(1, 0) == 57);
        CHECK(cloud->get_point(2, 0) == 906);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - no trailing newline") {
        std::string_view input = "162,817,812\n57,618,57\n906,360,560";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 3);
        
        // Verify last coordinate is parsed correctly
        CHECK(cloud->get_point(2, 0) == 906);
        CHECK(cloud->get_point(2, 1) == 360);
        CHECK(cloud->get_point(2, 2) == 560);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - zero values") {
        std::string_view input = "0,0,0\n1,2,3";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 2);
        
        CHECK(cloud->get_point(0, 0) == 0);
        CHECK(cloud->get_point(0, 1) == 0);
        CHECK(cloud->get_point(0, 2) == 0);
        
        CHECK(cloud->get_point(1, 0) == 1);
        CHECK(cloud->get_point(1, 1) == 2);
        CHECK(cloud->get_point(1, 2) == 3);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - large numbers") {
        std::string_view input = "999999,888888,777777";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 1);
        
        CHECK(cloud->get_point(0, 0) == 999999);
        CHECK(cloud->get_point(0, 1) == 888888);
        CHECK(cloud->get_point(0, 2) == 777777);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - single digit coordinates") {
        std::string_view input = "1,2,3\n4,5,6";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 2);
        
        CHECK(cloud->get_point(0, 0) == 1);
        CHECK(cloud->get_point(0, 1) == 2);
        CHECK(cloud->get_point(0, 2) == 3);
        
        CHECK(cloud->get_point(1, 0) == 4);
        CHECK(cloud->get_point(1, 1) == 5);
        CHECK(cloud->get_point(1, 2) == 6);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - empty string") {
        std::string_view input = "";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        CHECK(cloud->get_num_points() == 0);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - only whitespace") {
        std::string_view input = "   \n\t\r\n   ";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        CHECK(cloud->get_num_points() == 0);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - mixed whitespace between coordinates") {
        std::string_view input = "1,2,3\n\n\n4,5,6\n\n7,8,9";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 3);
        
        CHECK(cloud->get_point(0, 0) == 1);
        CHECK(cloud->get_point(1, 0) == 4);
        CHECK(cloud->get_point(2, 0) == 7);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - verify data vector directly") {
        std::string_view input = "10,20,30\n40,50,60";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        const auto& data = cloud->get_data();
        
        REQUIRE(data.size() == 6);
        CHECK(data[0] == 10);
        CHECK(data[1] == 20);
        CHECK(data[2] == 30);
        CHECK(data[3] == 40);
        CHECK(data[4] == 50);
        CHECK(data[5] == 60);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - verify element access") {
        std::string_view input = "100,200,300";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        CHECK(cloud->get_element(0) == 100);
        CHECK(cloud->get_element(1) == 200);
        CHECK(cloud->get_element(2) == 300);
        
        free_point_cloud(cloud);
    }
    
    TEST_CASE("parse_coordinates - multiple coordinates with various spacing") {
        std::string_view input = "  \n  100,200,300  \n\n  400,500,600\n\n\n";
        PointCloud* cloud = parse_coordinates(input);
        
        REQUIRE(cloud != nullptr);
        REQUIRE(cloud->get_num_points() == 2);
        
        CHECK(cloud->get_point(0, 0) == 100);
        CHECK(cloud->get_point(0, 1) == 200);
        CHECK(cloud->get_point(0, 2) == 300);
        
        CHECK(cloud->get_point(1, 0) == 400);
        CHECK(cloud->get_point(1, 1) == 500);
        CHECK(cloud->get_point(1, 2) == 600);
        
        free_point_cloud(cloud);
    }
}
