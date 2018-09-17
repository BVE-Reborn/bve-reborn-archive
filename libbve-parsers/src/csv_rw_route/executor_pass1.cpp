#include "csv_rw_route.hpp"
#include "parsers/errors.hpp"

namespace bve::parsers::csv_rw_route {
	namespace {
		class pass1_executor {
		  private:
			errors::multi_error_t& errors_;
			const std::vector<std::string>& filenames_;
			float current_position_ = -1;
			std::vector<float> current_unitoflength_ = {1, 1};

		  public:
			pass1_executor(errors::multi_error_t& e, const std::vector<std::string>& f) :
			    errors_(e),
			    filenames_(f) {}

			void operator()(instructions::naked::position& p) {
				if (p.distances.size() > current_unitoflength_.size()) {
					add_error(errors_, filenames_[p.file_index], p.line,
					          "Position has more arguments than UnitOfLength, "
					          "assuming 0 factors for missing Units.");
				}
				// Dot product
				current_position_ = 0;
				for (std::size_t i = 0;
				     i < std::min(current_unitoflength_.size(), p.distances.size()); ++i) {
					current_position_ += current_unitoflength_[i] * p.distances[i];
				}
				p.absolute_position = current_position_;
			}

			void operator()(instructions::options::unit_of_length& u) {
				current_unitoflength_ = u.factors_in_meters;
				u.absolute_position = current_position_;
			}

			template <class T>
			void operator()(T& value) {
				value.absolute_position = current_position_;
			}
		};
	} // namespace

	void execute_instructions_pass1(instruction_list& list, errors::multi_error_t& errors) {
		pass1_executor e(errors, list.filenames);

		for (auto& i : list.instructions) {
			apply_visitor(e, i);
		}

		std::stable_sort(list.instructions.begin(), list.instructions.end(),
		                 [](const instruction& a, const instruction& b) {
			                 auto position = [](auto& val) -> float {
				                 return val.absolute_position;
			                 };
			                 return apply_visitor(position, a) < apply_visitor(position, b);
		                 });
	}
} // namespace bve::parsers::csv_rw_route
