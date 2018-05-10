#pragma once

#define COMPARE_VARIANT_NODES(lhs, rhs) CHECK((lhs).is<decltype(rhs)>());
#define COMPARE_VARIANT_NODES_MEMBER(lhs, rhs, member)                                             \
	REQUIRE((lhs).is<decltype(rhs)>());                                                            \
	CHECK_EQ((lhs).get_unchecked<decltype(rhs)>().member, (rhs).member);

#define STRINGIFY(n) #n
