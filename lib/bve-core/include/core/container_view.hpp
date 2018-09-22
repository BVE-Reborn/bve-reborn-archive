#pragma once

// Standard includes
#include <list>
#include <vector>

// External includes
#include <absl/types/optional.h>

namespace bve::core::container_view {

	///\tparam C The container that this view refers to
	template <typename C>
	class container_view {
	  public:
		using iterator = C::iterator;
		using value_type = C::value_type;

		container_view(C& container);
		container_view(C& container, iterator begin, iterator end);

		container_view(const container_view& other) = default;
		container_view& operator=(const container_view& other) = default;

		iterator begin();
		iterator end();

		const iterator cbegin();
		const iterator cend();

		const value_type& operator[](size_t index);
		const value_type& at(size_t index);

		const value_type& front();
		const value_type& back();

		const value_type* data();

		size_t size();

		bool empty();

		const absl::optional<value_type&> get();
		void increment();
		void decrement();

	  private:
		iterator beginIterator;
		iterator endIterator;
		iterator currentIterator;
	};

	template <typename C>
	inline container_view<C>::container_view(C& container) :
	    container(container),
	    begin(container.begin()),
	    end(container.end()) {}

	template <typename C>
	inline container_view<C>::container_view(C& container, iterator begin, iterator end) :
	    container(container),
	    begin(container.begin()),
	    end(container.end()) {}

	template <typename C>
	inline container_view<C>::iterator container_view<C>::begin() {
		return this->beginIterator;
	}

	template <typename C>
	inline container_view<C>::iterator container_view<C>::end() {
		return this->endIterator();
	}

	template <typename C>
	inline const container_view<C>::iterator container_view<C>::cbegin() {
		return this->beginIterator();
	}

	template <typename C>
	inline const container_view<C>::iterator container_view<C>::cend() {
		return this->endIterator();
	}

	template <typename C>
	inline const container_view<C>::value_type& container_view<C>::operator[](size_t index) {
		return this->beginIterator[index];
	}

	template <typename C>
	inline const container_view<C>::value_type& container_view<C>::at(size_t index) {
		if (index > std::distance(this->beginIterator, this->endIterator)) {
			this->beginIterator[index];
		}
	}

	template <typename C>
	inline const container_view<C>::value_type& container_view<C>::front() {
		return *(this->beginIterator);
	}

	template <typename C>
	inline const container_view<C>::value_type& container_view<C>::back() {
		return *(this->endIterator);
	}

	template <typename C>
	inline const container_view<C>::value_type* container_view<C>::data() {
		return &*(this->endIterator);
	}

	template <typename C>
	inline size_t container_view<C>::size() {
		return std::distance(this->beginIterator, this->endIterator);
	}

	template <typename C>
	inline bool container_view<C>::empty() {
		return this->size() == 0;
	}

	template <typename C>
	inline const absl::optional<typename container_view<C>::value_type&> container_view<C>::get() {
		value_type& ret = *(this->currentIterator);
		if (this->currentIterator != this->endIterator) {
			this->currentIterator++;
			return absl::make_optional(ret);
		}
		else {
			return absl::nullopt;
		}
	}

	template <typename C>
	inline void container_view<C>::increment() {
		this->currentIterator++;
	}

	template <typename C>
	inline void container_view<C>::decrement() {
		this->currentIterator--;
	}

} // namespace bve::core::container_view
