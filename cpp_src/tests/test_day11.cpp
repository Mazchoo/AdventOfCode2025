#include "doctest.h"
#include "day11.hpp"

using namespace day11;

TEST_SUITE("Day11 Tests") {
    TEST_CASE("get_number_of_paths_chain - sample from specification") {
        std::string_view input = R"(svr: aaa bbb
aaa: fft
fft: ccc
bbb: tty
tty: ccc
ccc: ddd eee
ddd: hub
hub: fff
eee: dac
dac: fff
fff: ggg hhh
ggg: out
hhh: out)";

        auto result = get_number_of_paths_in_chain(input);
        CHECK(result == 2);
    }

    TEST_CASE("get_number_of_paths - sample from specification") {
        std::string_view input = R"(aaa: you hhh
you: bbb ccc
bbb: ddd eee
ccc: ddd eee fff
ddd: ggg
eee: out
fff: out
ggg: out
hhh: ccc fff iii
iii: out)";

        auto result = get_number_of_paths_start_to_end(input);
        CHECK(result == 5);
    }

    TEST_CASE("get_number_of_paths - impossible path returns 0") {
        std::string_view input = R"(aaa: you hhh
you: bbb ccc
bbb: ddd eee
ccc: ddd eee fff
ddd: ggg
eee: out
fff: out
ggg: out
hhh: ccc fff iii
iii: out)";

        auto graph = parse_graph(input);
        auto result = get_number_of_paths(graph, "out", "you");

        CHECK(result == 0);
    }

    TEST_CASE("parse_graph - simple graph") {
        std::string_view input = R"(aaa: bbb
bbb: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 3);
        CHECK(graph.nodes[0] == "aaa");
        CHECK(graph.nodes[1] == "bbb");
        CHECK(graph.nodes[2] == "out");

        REQUIRE(graph.edges.size() == 2);
        CHECK(graph.edges[0].from_index == 0);  // aaa -> bbb
        CHECK(graph.edges[0].to_index == 1);
        CHECK(graph.edges[1].from_index == 1);  // bbb -> out
        CHECK(graph.edges[1].to_index == 2);
    }

    TEST_CASE("parse_graph - sample from specification") {
        std::string_view input = R"(aaa: you hhh
you: bbb ccc
bbb: ddd eee
ccc: ddd eee fff
ddd: ggg
eee: out
fff: out
ggg: out
hhh: ccc fff iii
iii: out)";

        auto graph = parse_graph(input);

        // Should have 11 unique nodes
        REQUIRE(graph.nodes.size() == 11);

        // Check that expected nodes exist
        bool found_you = false;
        bool found_out = false;
        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (graph.nodes[i] == "you") found_you = true;
            if (graph.nodes[i] == "out") found_out = true;
        }
        CHECK(found_you);
        CHECK(found_out);

        // Count edges - should be 17 total (sum of all outgoing edges)
        // aaa: 2, you: 2, bbb: 2, ccc: 3, ddd: 1, eee: 1, fff: 1, ggg: 1, hhh: 3, iii: 1
        CHECK(graph.edges.size() == 17);
    }

    TEST_CASE("parse_graph - node with single neighbor") {
        std::string_view input = "aaa: out";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 2);
        REQUIRE(graph.edges.size() == 1);
        CHECK(graph.edges[0].from_index == 0);
        CHECK(graph.edges[0].to_index == 1);
    }

    TEST_CASE("parse_graph - node with multiple neighbors") {
        std::string_view input = "aaa: bbb ccc ddd eee";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 5);
        REQUIRE(graph.edges.size() == 4);

        // All edges should originate from aaa (index 0)
        for (const auto& edge : graph.edges) {
            CHECK(edge.from_index == 0);
        }
    }

    TEST_CASE("parse_graph - empty input") {
        std::string_view input = "";

        auto graph = parse_graph(input);

        CHECK(graph.nodes.empty());
        CHECK(graph.edges.empty());
    }

    TEST_CASE("parse_graph - with empty lines") {
        std::string_view input = R"(aaa: bbb

bbb: ccc

ccc: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 4);
        REQUIRE(graph.edges.size() == 3);
    }

    TEST_CASE("parse_graph - with whitespace") {
        std::string_view input = "  aaa:   bbb   ccc  \n  bbb:  out  ";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 4);
        REQUIRE(graph.edges.size() == 3);
        CHECK(graph.nodes[0] == "aaa");
        CHECK(graph.nodes[1] == "bbb");
    }

    TEST_CASE("parse_graph - node references itself") {
        std::string_view input = "aaa: aaa out";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 2);
        REQUIRE(graph.edges.size() == 2);

        // First edge: aaa -> aaa (self-loop)
        CHECK(graph.edges[0].from_index == 0);
        CHECK(graph.edges[0].to_index == 0);

        // Second edge: aaa -> out
        CHECK(graph.edges[1].from_index == 0);
        CHECK(graph.edges[1].to_index == 1);
    }

    TEST_CASE("parse_graph - multiple nodes point to same target") {
        std::string_view input = R"(aaa: out
bbb: out
ccc: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 4);
        REQUIRE(graph.edges.size() == 3);

        // Find "out" index
        size_t out_index = 0;
        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (graph.nodes[i] == "out") out_index = i;
        }

        // All edges should point to "out"
        for (const auto& edge : graph.edges) {
            CHECK(edge.to_index == out_index);
        }
    }

    TEST_CASE("parse_graph - nodes appear in any order") {
        std::string_view input = R"(bbb: ccc
aaa: bbb
ccc: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 4);
        REQUIRE(graph.edges.size() == 3);

        // Find indices
        size_t aaa_idx = 0, bbb_idx = 0, ccc_idx = 0, out_idx = 0;
        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (graph.nodes[i] == "aaa") aaa_idx = i;
            if (graph.nodes[i] == "bbb") bbb_idx = i;
            if (graph.nodes[i] == "ccc") ccc_idx = i;
            if (graph.nodes[i] == "out") out_idx = i;
        }

        // Verify edges exist with correct indices
        bool found_aaa_bbb = false, found_bbb_ccc = false, found_ccc_out = false;
        for (const auto& edge : graph.edges) {
            if (edge.from_index == aaa_idx && edge.to_index == bbb_idx) found_aaa_bbb = true;
            if (edge.from_index == bbb_idx && edge.to_index == ccc_idx) found_bbb_ccc = true;
            if (edge.from_index == ccc_idx && edge.to_index == out_idx) found_ccc_out = true;
        }

        CHECK(found_aaa_bbb);
        CHECK(found_bbb_ccc);
        CHECK(found_ccc_out);
    }

    TEST_CASE("parse_graph - lines without colon are skipped") {
        std::string_view input = R"(aaa: bbb
invalid line without colon
bbb: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 3);
        REQUIRE(graph.edges.size() == 2);
    }

    TEST_CASE("parse_graph - node with no neighbors after colon") {
        std::string_view input = "aaa:";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 1);
        CHECK(graph.nodes[0] == "aaa");
        CHECK(graph.edges.empty());
    }

    TEST_CASE("parse_graph - complex graph with cycles") {
        std::string_view input = R"(aaa: bbb ccc
bbb: ccc aaa
ccc: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 4);
        REQUIRE(graph.edges.size() == 5);

        // Verify it correctly handles the cycle (aaa -> bbb -> aaa)
        size_t aaa_idx = 0, bbb_idx = 0;
        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (graph.nodes[i] == "aaa") aaa_idx = i;
            if (graph.nodes[i] == "bbb") bbb_idx = i;
        }

        bool found_aaa_to_bbb = false, found_bbb_to_aaa = false;
        for (const auto& edge : graph.edges) {
            if (edge.from_index == aaa_idx && edge.to_index == bbb_idx) found_aaa_to_bbb = true;
            if (edge.from_index == bbb_idx && edge.to_index == aaa_idx) found_bbb_to_aaa = true;
        }

        CHECK(found_aaa_to_bbb);
        CHECK(found_bbb_to_aaa);
    }

    TEST_CASE("parse_graph - node names with numbers") {
        std::string_view input = R"(node1: node2 node3
node2: node4
node3: node4
node4: out)";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 5);
        CHECK(graph.edges.size() == 5);
    }

    TEST_CASE("parse_graph - directed edges are not duplicated") {
        std::string_view input = R"(aaa: bbb bbb
bbb: out)";

        auto graph = parse_graph(input);

        // Should create two edges from aaa to bbb (not deduplicated)
        REQUIRE(graph.edges.size() == 3);

        size_t aaa_idx = 0, bbb_idx = 0;
        for (size_t i = 0; i < graph.nodes.size(); i++) {
            if (graph.nodes[i] == "aaa") aaa_idx = i;
            if (graph.nodes[i] == "bbb") bbb_idx = i;
        }

        size_t count_aaa_to_bbb = 0;
        for (const auto& edge : graph.edges) {
            if (edge.from_index == aaa_idx && edge.to_index == bbb_idx) {
                count_aaa_to_bbb++;
            }
        }

        CHECK(count_aaa_to_bbb == 2);
    }

    TEST_CASE("parse_graph - trailing whitespace and newlines") {
        std::string_view input = "aaa: bbb\n\n\n";

        auto graph = parse_graph(input);

        REQUIRE(graph.nodes.size() == 2);
        REQUIRE(graph.edges.size() == 1);
    }
}
