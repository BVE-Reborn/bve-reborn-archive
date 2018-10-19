#pragma once

#include <string>
#include <vector>
#include "parsers/errors.hpp"

namespace parsers {
namespace config {
    namespace extensions_cfg {

		/**
		 * \brief define properties for individual axle positions
		 * \param rear A floating-point number indicating the z-position of the rear axle measured from the center of the car. Usually a negative value.
		 * \param front A floating-point number indicating the z-position of the front axle measured from the center of the car. Usually a positive value.
		 */
        struct axles_t {
            float rear;
            float front;
        };

        /**
         * \brief define properties specific to a certain car.
         * \param object The relative file name of the exterior object to use for this car.
         * \param length A positive floating-point number representing the length of the car.
         * \param axles defines the positions of the axles.
         * \param reversed Either True or False to indicate whether to reverse the car.
         */
        struct car {
            std::string object;
            float length;
            axles_t axles;
            bool reversed;
        };

        /**
         * \brief Defines the lowest and highest allowed distances between the cars, resembling a buffer and chain coupler.
         * \param minimum: A floating-point number indicating the minimum allowed distance between the cars.
         * \param maximum: A floating-point number indicating the maximum allowed distance between the cars.
         */
        struct distances_t {
            float minimum; 
            float maximum;
        };

        /**
         * \brief Define properties specific to a certain coupler.
         * \param distances Defines the lowest and highest allowed distances between the cars, resembling a buffer and chain coupler.
         */
        struct coupler_t {
            distances_t distances;
        };

        /**
         * \brief Provides an easy way of adding exterior objects to the particular train.
         * \param car_filenames The relative file name of the exterior object to use for car i, relative to the train folder.
         */
        struct exterior_t {
            std::vector<std::string> car_filenames;
        };

        /**
         * \brief holds parsed properties for individual cars, couplers, and exterior objects.
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
         * \brief Parses extensions.cfg file to define properties for individual cars, like length, axle positions and exterior objects.
         * \param file plain text file expected to be named extensions.cfg
         * \param filename
         * \param errors
         * \return parsed properties
         */
        parsed_extensions_config parse(const std::string& file, std::string const& filename, errors::multi_error_t& errors);
    } // namespace extensions_cfg
} // namespace config
} // namespace parsers
