#pragma once

#include <rapidxml_ns.hpp>

namespace parsers {
namespace xml {
	namespace {
		inline std::string get_node_name(rapidxml_ns::xml_node<char> const* const node) {
			return std::string(node->name(), node->name_size());
		}

		inline std::string get_node_value(rapidxml_ns::xml_node<char> const* const node) {
			return std::string(node->value(), node->value_size());
		}
	} // namespace
} // namespace xml
} // namespace parsers
