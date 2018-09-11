#pragma once

#include <string>
#include <vector>
#include "parsers/errors.hpp"

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

        /**
         * \brief
         */
        struct parsed_extensions_config {
            /// \brief
            std::vector<car> cars;
            /// \brief
            std::vector<coupler_t> couplers;
            /// \brief
            exterior_t exterior;
        };

        /**
         * \brief
         * \param file
         * \param filename
         * \param errors
         * \return
         */
        parsed_extensions_config parse(const std::string& file, std::string const& filename, errors::multi_error_t& errors);
    } // namespace extensions_cfg
} // namespace config
} // namespace parsers
