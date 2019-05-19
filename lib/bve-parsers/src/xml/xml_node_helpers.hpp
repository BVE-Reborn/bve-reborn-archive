#pragma once

#include <rapidxml-ns/rapidxml_ns.hpp>
#include <util/assert.hpp>

namespace bve::parsers::xml {
	namespace {
		inline std::string get_node_name(rapidxml_ns::xml_node<char> const* const node) {
			BVE_ASSERT(node, "get_node_name must not be null");
			return std::string(node->name(), node->name_size());
		}

		inline std::string get_node_value(rapidxml_ns::xml_node<char> const* const node) {
			BVE_ASSERT(node, "get_node_name must not be null");
			return std::string(node->value(), node->value_size());
		}
	} // namespace
} // namespace bve::parsers::xml
