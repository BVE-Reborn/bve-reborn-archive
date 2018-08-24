#include "parsers/config/extentions_cfg.hpp"
#include "ini.hpp"
#include <map>
#include "utils.hpp"

namespace parsers {
namespace config {
    namespace extensions_cfg {
        parsed_extensions_config parse(const std::string& file) {
            parsed_extensions_config retval;
            auto parsed_ini = ini::parse(file);
            std::map<std::size_t, car> car_map;
            std::map<std::size_t, coupler_t> coupler_map;
            for(auto& section : parsed_ini) {
                if(util::match_against_lower(section.name, "exterior")) {
                    std::map<std::size_t, std::string> cars;
                    for(auto& kvp : section.key_value_pairs) {
                        cars[util::parse_loose_integer(kvp.key)] = kvp.value;
                    }

                    for(auto& itr : cars) {
                        retval.exterior.car_filenames.emplace_back(itr.second);
                    }
                }
                else if(util::match_against_lower(section.name.substr(0,3), "car")) {
                    auto car_number = util::parse_loose_integer(section.name.substr(3));
                    car c;
                    for(auto& kvp : section.key_value_pairs) {
                        if(util::match_against_lower(kvp.key, "object")) {
                            c.object = kvp.value;
                        }
                        else if(util::match_against_lower(kvp.key, "length")) {
                            c.length = util::parse_loose_float(kvp.value);
                        }
                        else if(util::match_against_lower(kvp.key, "axles")) {
                            auto split = util::split_text(kvp.value);
                            c.axles.rear = util::parse_loose_float(split[0]);
                            c.axles.front = util::parse_loose_float(split[1]);
                        }
                        else if(util::match_against_lower(kvp.key, "reversed")) {
                            auto string = kvp.value;
                            util::strip_text(string);
                            if(util::match_against_lower(string, "true")) {
                                c.reversed = true;
                            }
                            else if(util::match_against_lower(string, "false")) {
                                c.reversed = false;
                            }
                        }
                    }
                    car_map[car_number] = c;
                }
                else if(util::match_against_lower(section.name.substr(0,3), "coupler")) {
                    auto coupler_number = util::parse_loose_integer(section.name.substr(3));
                    coupler_t c;
                    for(auto& kvp : section.key_value_pairs) {
                        if(util::match_against_lower(kvp.key, "distances")) {
                            auto split = util::split_text(kvp.value);
                            c.distances.minimum = util::parse_loose_float(split[0]);
                            c.distances.maximum = util::parse_loose_float(split[1]);
                        }
                    }
                    coupler_map[coupler_number] = c;
                }
            }
            for(auto& itr : car_map) {
                retval.cars.emplace_back(itr.second);
            }
            for(auto& itr : coupler_map) {
                retval.couplers = itr.second;
            }
        }
    }
}
}
