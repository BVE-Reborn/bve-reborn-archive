#include <cstdlib>
#include <functional>
#include <utility>

namespace bve::core::hash {
	inline std::size_t hash_combine(std::size_t lhs, std::size_t const rhs) noexcept {
		lhs ^= rhs + 0x517cc1b727220a95 + (lhs << 6) + (lhs >> 2);
		return lhs;
	}
	struct pair_hash {
		template <class T, class U>
		std::size_t operator()(std::pair<T, U> const& x) const noexcept {
			return hash_combine(std::hash<T>()(x.first), std::hash<T>()(x.second));
		}
	};
} // namespace bve::core::hash
