#include "core/math.hpp"
#include "csv_rw_route.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace {
		struct pass2_executor {
		  public:
			parsed_route_data rd;
			float first_factor_uod = 1;
			float block_size = 25;

		  private:
			errors::multi_error& _errors;
			const std::vector<std::string>& _filenames;

			enum { Unsigned, Signed } cant_behavior;

			rail_block_info& make_new_block(float position) {
				if (!rd.blocks.empty()) {
					rd.blocks.emplace_back(rd.blocks.back());
					rail_block_info& rbi = rd.blocks.back();
					rbi.length = position - rbi.position;
					rbi.position = position;
					rd.blocks.emplace_back(std::move(rbi));
					return rbi;
				}
				else {
					rd.blocks.emplace_back();
					rail_block_info& rbi = rd.blocks.back();
					rbi.position = position;
					return rbi;
				}
			}

			void calculate_cache() {
				if (rd.blocks.size() >= 2) {
					auto& current_block = rd.blocks.back();
					auto& last_block = rd.blocks[rd.blocks.size() - 2];

					openbve2::math::evaluate_curve(last_block.cache.location, last_block.cache.direction,
					                               last_block.length, last_block.radius);
				}
			}

		  public:
			pass2_executor(errors::multi_error& e, const std::vector<std::string>& f) : _errors(e), _filenames(f){};

			void operator()(const instructions::options::UnitOfLength& instr) {
				first_factor_uod = instr.factors_in_meters[1];
			}

			void operator()(const instructions::options::BlockLength& instr) {
				block_size = first_factor_uod * instr.length;
			}

			void operator()(const instructions::track::Pitch& instr) {
				auto& block = make_new_block(instr.absolute_offset);
				block.pitch = instr.rate / 1000;
				calculate_cache();
			}

			void operator()(const instructions::track::Curve& instr) {
				auto& block = make_new_block(instr.absolute_offset);
				block.radius = instr.radius;
				block.cant = instr.cant;
				calculate_cache();
			}

			void operator()(const instructions::track::Turn& instr) {
				auto& block = make_new_block(instr.absolute_offset);

				if (instr.ratio != 0) {
					auto x = instr.ratio * block_size;
					auto z = x / instr.ratio;

					block.radius = openbve2::math::radius_from_distances(z, x);
				}
				else {
					block.radius = 0;
				}

				calculate_cache();
			}

			void operator()(const instructions::track::Height& instr) {
				auto& block = make_new_block(instr.absolute_offset);

				block.height = instr.y;

				calculate_cache();
			}
		};
	} // namespace
} // namespace csv_rw_route
} // namespace parsers
