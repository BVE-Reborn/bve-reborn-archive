#include "core/openbve/logger.hpp"
#include "util/macro_helpers.hpp"

// A large selection of helper macros to allow easy definitions of per-namespace loggers
#define BVEREBORN_LOGGER_DEFINE_NAMESPACE_1(elem) elem
#define BVEREBORN_LOGGER_DEFINE_NAMESPACE_2(elem, ...) elem ::EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_1(__VA_ARGS__))
#define BVEREBORN_LOGGER_DEFINE_NAMESPACE_3(elem, ...) elem ::EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_2(__VA_ARGS__))
#define BVEREBORN_LOGGER_DEFINE_NAMESPACE_4(elem, ...) elem ::EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_3(__VA_ARGS__))

#define BVEREBORN_LOGGER_DEFINE_STR_1(elem) elem
#define BVEREBORN_LOGGER_DEFINE_STR_2(elem, ...) STR(elem) "/" EXPAND(STR(BVEREBORN_LOGGER_DEFINE_STR_1(__VA_ARGS__)))
#define BVEREBORN_LOGGER_DEFINE_STR_3(elem, ...) STR(elem) "/" EXPAND(STR(BVEREBORN_LOGGER_DEFINE_STR_2(__VA_ARGS__)))
#define BVEREBORN_LOGGER_DEFINE_STR_4(elem, ...) STR(elem) "/" EXPAND(STR(BVEREBORN_LOGGER_DEFINE_STR_3(__VA_ARGS__)))

#define BVEREBORN_LOGGER_DEFINE_1(...)                                                   \
	namespace EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_1(__VA_ARGS__)) {                 \
		FOUNDATIONAL_LOGGER(logger, EXPAND(BVEREBORN_LOGGER_DEFINE_STR_1(__VA_ARGS__))); \
	}
#define BVEREBORN_LOGGER_DEFINE_2(...)                                                   \
	namespace EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_2(__VA_ARGS__)) {                 \
		FOUNDATIONAL_LOGGER(logger, EXPAND(BVEREBORN_LOGGER_DEFINE_STR_2(__VA_ARGS__))); \
	}
#define BVEREBORN_LOGGER_DEFINE_3(...)                                                   \
	namespace EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_3(__VA_ARGS__)) {                 \
		FOUNDATIONAL_LOGGER(logger, EXPAND(BVEREBORN_LOGGER_DEFINE_STR_3(__VA_ARGS__))); \
	}
#define BVEREBORN_LOGGER_DEFINE_4(...)                                                   \
	namespace EXPAND(BVEREBORN_LOGGER_DEFINE_NAMESPACE_4(__VA_ARGS__)) {                 \
		FOUNDATIONAL_LOGGER(logger, EXPAND(BVEREBORN_LOGGER_DEFINE_STR_4(__VA_ARGS__))); \
	}

// Logger list
BVEREBORN_LOGGER_DEFINE_2(bve, openbve)
