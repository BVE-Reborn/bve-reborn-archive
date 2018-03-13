#include "csv_rw_route.hpp"
#include "parsers/errors.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace {
		class pass1_executor {
		  private:
			errors::multi_error& _errors;
			const std::vector<std::string>& _filenames;
			float current_position = -1;
			std::vector<float> current_unitoflength = {1, 1};

		  public:
			pass1_executor(errors::multi_error& e, const std::vector<std::string>& f) :
			    _errors(e),
			    _filenames(f){};

			void operator()(instructions::naked::position& p) {
				if (p.distances.size() > current_unitoflength.size()) {
					add_error(_errors, _filenames[p.file_index], p.line,
					          "Position has more arguments than UnitOfLength, "
					          "assuming 0 factors for missing Units.");
				}
				// Dot product
				current_position = 0;
				for (std::size_t i = 0;
				     i < std::min(current_unitoflength.size(), p.distances.size()); ++i) {
					current_position += current_unitoflength[i] * p.distances[i];
				}
				p.absolute_position = current_position;
			}

			void operator()(instructions::options::UnitOfLength& u) {
				current_unitoflength = u.factors_in_meters;
				u.absolute_position = current_position;
			}

			template <class T>
			void operator()(T& value) {
				value.absolute_position = current_position;
			}
		};
	} // namespace

	void execute_instructions_pass1(instruction_list& list, errors::multi_error& errors) {
		pass1_executor e(errors, list.filenames);

		for (auto& i : list.instructions) {
			mapbox::util::apply_visitor(e, i);
		}

		std::stable_sort(list.instructions.begin(), list.instructions.end(),
		                 [](const instruction& a, const instruction& b) {
			                 auto position = [](auto& val) -> float {
				                 return val.absolute_position;
			                 };
			                 return mapbox::util::apply_visitor(position, a)
			                        < mapbox::util::apply_visitor(position, b);
		                 });
	}
} // namespace csv_rw_route
} // namespace parsers
