#include "parsers/csv_rw_route.hpp"
#include "parsers/errors.hpp"

namespace bve::parsers::csv_rw_route {
	namespace {
		class Pass1Executor {
		  private:
			errors::MultiError& errors_;
			const std::vector<std::string>& filenames_;
			float current_position_ = -1;
			std::vector<float> current_unitoflength_ = {1, 1};

		  public:
			Pass1Executor(errors::MultiError& e, const std::vector<std::string>& f) :
			    errors_(e),
			    filenames_(f) {}

			void operator()(instructions::naked::Position& p) {
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

			void operator()(instructions::options::UnitOfLength& u) {
				current_unitoflength_ = u.factors_in_meters;
				u.absolute_position = current_position_;
			}

			template <class T>
			void operator()(T& value) {
				value.absolute_position = current_position_;
			}
		};
	} // namespace

	void execute_instructions_pass1(InstructionList& list, errors::MultiError& errors) {
		Pass1Executor e(errors, list.filenames);

		for (auto& i : list.instructions) {
			apply_visitor(e, i);
		}

		std::stable_sort(list.instructions.begin(), list.instructions.end(),
		                 [](const Instruction& a, const Instruction& b) {
			                 auto position = [](auto& val) -> float {
				                 return val.absolute_position;
			                 };
			                 return apply_visitor(position, a) < apply_visitor(position, b);
		                 });
	}
} // namespace bve::parsers::csv_rw_route
