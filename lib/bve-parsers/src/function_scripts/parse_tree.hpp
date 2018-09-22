#pragma once

#include "lexer.hpp"
#include "parsers/errors.hpp"
#include "parsers/function_scripts.hpp"
#include <iosfwd>
#include <mapbox/recursive_wrapper.hpp>
#include <mapbox/variant.hpp>
#include <string>
#include <absl/types/optional.h>
#include <vector>

namespace bve::parsers::function_scripts {
	namespace tree_types {
		// forward declare all structures that have nodes in them
		// recursive_wrapper transparently allocates
		struct BinaryAnd;
		struct BinaryXor;
		struct BinaryOr;
		struct BinaryEq;
		struct BinaryNotEq;
		struct BinaryLess;
		struct BinaryGreater;
		struct BinaryLessEq;
		struct BinaryGreaterEq;
		struct BinaryAdd;
		struct BinarySubtract;
		struct BinaryMultiply;
		struct BinaryDivide;
		struct UnaryNot;
		struct UnaryMinus;
		struct FunctionCall;
		struct Integer {
			std::intmax_t num;
		};
		struct Floating {
			float num;
		};
		struct Identifier {
			std::string val;
		};

		struct None {};
	} // namespace tree_types

	using TreeNode =
	    mapbox::util::variant<mapbox::util::recursive_wrapper<tree_types::BinaryAnd>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryXor>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryOr>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryNotEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryLess>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryGreater>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryLessEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryGreaterEq>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryAdd>,
	                          mapbox::util::recursive_wrapper<tree_types::BinarySubtract>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryMultiply>,
	                          mapbox::util::recursive_wrapper<tree_types::BinaryDivide>,
	                          mapbox::util::recursive_wrapper<tree_types::UnaryNot>,
	                          mapbox::util::recursive_wrapper<tree_types::UnaryMinus>,
	                          mapbox::util::recursive_wrapper<tree_types::FunctionCall>,
	                          tree_types::Integer,
	                          tree_types::Floating,
	                          tree_types::Identifier,
	                          tree_types::None>;

	namespace tree_types {
		struct BinaryAnd {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryXor {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryOr {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryNotEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryLess {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryGreater {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryLessEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryGreaterEq {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryAdd {
			TreeNode left;
			TreeNode right;
		};

		struct BinarySubtract {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryMultiply {
			TreeNode left;
			TreeNode right;
		};

		struct BinaryDivide {
			TreeNode left;
			TreeNode right;
		};

		struct UnaryNot {
			TreeNode child;
		};
		struct UnaryMinus {
			TreeNode child;
		};

		struct FunctionCall {
			Identifier name;
			std::vector<TreeNode> args;
		};
	} // namespace tree_types

	TreeNode create_tree(const LexerTokenList& list, errors::Errors& errors);
	InstructionList build_instructions(const TreeNode& head_node,
	                                   const errors::Errors& errors = {});
} // namespace bve::parsers::function_scripts

std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::function_scripts::LexerTokenList& list);
std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::TreeNode& node);
