#pragma once

#include <string>
#include <vector>

namespace parsers {
namespace config {
    namespace extensions_cfg {
    
        struct axles_t {
            float rear;
            float front;
        };

        struct car {
            std::string object;
            float length;
            axles_t axles;
            bool reversed;
        };
    
        struct distances_t {
            float minimum; 
            float maximum;
        };

        struct coupler_t {
            distances_t distances;
        };

        struct exterior_t {
            std::vector<std::string> car_filenames;
        };

        struct parsed_extensions_config {
            std::vector<car> cars;
            coupler_t couplers;
            exterior_t exterior;
        };

        parsed_extensions_config parse(const std::string& file);
    } // namespace extensions_cfg
} // namespace config
} // namespace parsers
