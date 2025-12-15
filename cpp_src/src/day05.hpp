// Day05
#include <vector>
#include <string_view>
#include <optional>

namespace day05
{
    // Type alias for a range (start, end)
    using Range = std::pair<uint64_t, uint64_t>;
    
    // Type alias for the return type of parse_ranges_and_ids
    using RangesAndIds = std::pair<std::vector<Range>, std::vector<uint64_t>>;

    // Parses input containing ranges and IDs separated by a blank line
    // Format:
    //   3-5
    //   10-14
    //   16-20
    //   
    //   1
    //   5
    //   8
    // Returns: pair of (vector of range pairs, vector of IDs)
    RangesAndIds parse_ranges_and_ids(std::string_view payload) {
        std::vector<Range> ranges;
        std::vector<uint64_t> ids;
        
        uint64_t current_number = 0;
        uint64_t range_start = 0;
        bool has_number = false;
        bool has_range_start = false;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (c == ' ' || c == '\t' || c == '\r')
                continue;

            if (c >= '0' && c <= '9') {
                current_number = current_number * 10 + (c - '0');
                has_number = true;
            } else if (c == '-') {
                // Dash separates range start from end
                if (has_number) {
                    range_start = current_number;
                    has_range_start = true;
                    current_number = 0;
                    has_number = false;
                }
            } else if (c == '\n') {
                // End of line - complete the range
                if (has_number && has_range_start) {
                    ranges.push_back({range_start, current_number});
                    has_range_start = false;
                    current_number = 0;
                    has_number = false;
                }
                else if (has_number) {
                    ids.push_back(current_number);
                    current_number = 0;
                    has_number = false;
                }
            }
        }
        
        // Handle the last number if we ended while parsing
        if (has_number && has_range_start) {
            ranges.push_back({range_start, current_number});
        } else if (has_number) {
            ids.push_back(current_number);
        }
        
        return {ranges, ids};
    }

    // Fuses (merges) two ranges if they overlap or are adjacent
    // Returns the fused range if successful, or std::nullopt if ranges cannot be fused
    std::optional<Range> fuse_two_ranges(
        const Range& range1,
        const Range& range2) {
        
        // Check if ranges overlap or are adjacent
        if (range1.first <= range2.second && range2.first <= range1.second) {
            // Ranges can be fused
            uint64_t fused_start = std::min(range1.first, range2.first);
            uint64_t fused_end = std::max(range1.second, range2.second);
            return std::make_pair(fused_start, fused_end);
        }
        
        // Ranges cannot be fused
        return std::nullopt;
    }

    // Count the number of ids in the range
    uint64_t count_range(const Range& range) {
        // Ranges cannot be fused
        return range.second - range.first + 1;
    }

    // If falls into any range return true
    bool id_in_any_range(std::vector<Range>& ranges, uint64_t id) {
        for (auto& range : ranges) {
            if (id >= range.first && id <= range.second)
                return true;
        }
        return false;
    }

    uint32_t count_ids_in_range(std::string_view payload) {
        auto range_and_ids = parse_ranges_and_ids(payload);
        uint32_t result = 0;
        for (auto id: range_and_ids.second) {
            if (id_in_any_range(range_and_ids.first, id))
                result += 1;
        }
        return result;
    }

    uint64_t count_all_valid_ids(std::string_view payload) {
        auto ranges = parse_ranges_and_ids(payload).first;
        std::vector<Range> valid_ranges = {};
        std::vector<Range> new_ranges = {};

        for (auto range : ranges) {
            new_ranges.clear();
            for (auto valid_range : valid_ranges) {
                auto new_range = fuse_two_ranges(range, valid_range);
                if (new_range != std::nullopt) {
                    range = new_range.value();
                } else {
                    new_ranges.push_back(valid_range);
                }
            }
            new_ranges.push_back(range);
            valid_ranges = new_ranges;
        }

        uint64_t result = 0;
        for (auto& range: valid_ranges)
            result += count_range(range);

        return result;
    }
}