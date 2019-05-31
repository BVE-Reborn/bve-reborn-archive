#include "parsers/csv_rw_route.hpp"
#include "parsers/errors.hpp"
#include "util/math.hpp"

namespace bve::parsers::csv_rw_route {
	namespace {
		struct Pass2Executor {
		  public:
			ParsedRoute rd = {};
			float first_factor_uod = 1;
			float block_size = 25;

		  private:
			errors::MultiError& errors_;
			const std::vector<std::string>& filenames_;

			instructions::options::CantBehavior::Mode cant_behavior_ = instructions::options::CantBehavior::Mode::unsigned_cant;

			RailBlockInfo& makeNewBlock(float position) {
				position = bve::util::math::max<float>(0, position);
				if (rd.blocks.empty()) {
					if (position != 0) {
						rd.blocks.emplace_back();
						rd.blocks.back().cache.direction = glm::vec3(0, 0, 1);
					}
					else {
						rd.blocks.emplace_back();
						auto& rbi = rd.blocks.back();
						rbi.position = position;
						rbi.cache.direction = glm::vec3(0, 0, 1);
						return rbi;
					}
				}
				// Make sure we always have the same block if positions are
				// identical
				if (rd.blocks.back().position == position) {
					return rd.blocks.back();
				}

				// set the last block's length based on this block's
				// position
				{
					auto& last_block = rd.blocks.back();
					last_block.length = position - last_block.position;

					// make a duplicate of the last block
					rd.blocks.emplace_back(last_block);
					// emplacement may invalidate reference
				}
				auto& rbi = rd.blocks.back();
				rbi.position = position;
				return rbi;
			}

			static void calculateCacheImpl(RailBlockInfo& last_block, RailBlockInfo& current_block) {
				// Only update the cache if it hasn't been updated before
				// any changes are made to the current block, the last block
				// will not change
				if (!last_block.cache.valid) {
					// this implementation will maintain the amplitude of the
					// direction vector between input and output as well as
					// maintaining the amplitude of the vertical component of the
					// vector

					// all vectors are initialized a the beginning of the route
					// with a vector of (0, 0, 1) this way we can directly set
					// the y component in order to get the proper angle
					last_block.cache.direction.y = last_block.pitch;
					auto const curve_results = util::math::evaluate_curve(last_block.cache.location, last_block.cache.direction,
					                                                      last_block.length, last_block.radius);

					current_block.cache.location = curve_results.position;
					current_block.cache.direction = curve_results.tangent;
					last_block.cache.valid = true;
				}
			}

			void calculateCache() {
				// All positions and directions are for the beginning of the
				// block. We need to get the information from the block before
				// us
				if (rd.blocks.size() >= 2) {
					auto& current_block = rd.blocks.back();
					auto& last_block = rd.blocks[rd.blocks.size() - 2];

					calculateCacheImpl(last_block, current_block);
				}
				// There is no block before us, we are the start of the line.
				else {
					auto& current_block = rd.blocks.back();
					RailBlockInfo last_block{};

					last_block.length = current_block.position;
					last_block.cache.direction = glm::vec3(0, 0, 1);
					last_block.cache.location = glm::vec3(0);
					// secant line set by the block in front

					calculateCacheImpl(last_block, current_block);
				}
			}

		  public:
			Pass2Executor(errors::MultiError& e, const std::vector<std::string>& f) : errors_(e), filenames_(f) {}

			void operator()(const instructions::options::UnitOfLength& inst) {
				// verification of argument count has already happened
				// first element is the factor used by include offsets (always
				// one) the first user defined factor is the second element
				first_factor_uod = inst.factors_in_meters[1];
			}

			void operator()(const instructions::options::BlockLength& inst) {
				block_size = first_factor_uod * inst.length;
			}

			void operator()(const instructions::options::CantBehavior& inst) {
				cant_behavior_ = inst.mode;
			}

			void operator()(const instructions::track::Pitch& inst) {
				auto& block = makeNewBlock(inst.absolute_position);
				block.pitch = inst.rate / 1000;
				calculateCache();
			}

			void operator()(const instructions::track::Curve& inst) {
				auto& block = makeNewBlock(inst.absolute_position);
				block.radius = inst.radius;
				if (cant_behavior_ == instructions::options::CantBehavior::Mode::unsigned_cant) {
					block.cant = block.radius != 0 ? std::abs(inst.cant) : 0;
				}
				else {
					block.cant = inst.cant;
				}
				calculateCache();
			}

			void operator()(const instructions::track::Turn& inst) {
				auto& block = makeNewBlock(inst.absolute_position);

				if (inst.ratio != 0) {
					auto const x = inst.ratio * block_size;
					auto const z = x / inst.ratio;

					block.radius = util::math::radius_from_distances(z, x);
				}
				else {
					block.radius = 0;
				}

				calculateCache();
			}

			void operator()(const instructions::track::Height& inst) {
				rd.ground_height.emplace_back(GroundHeight{inst.absolute_position, inst.y});
			}

			template <class T>
			void operator()(const T& /*unused*/) {
				// Might need error handling later.
				(void) errors_;
				(void) filenames_;
			}
		};
	} // namespace

	ParsedRoute execute_instructions_pass2(InstructionList& list, errors::MultiError& errors) {
		Pass2Executor p2_e(errors, list.filenames);

		for (auto& i : list.instructions) {
			apply_visitor(p2_e, i);
		}

		return p2_e.rd;
	}
} // namespace bve::parsers::csv_rw_route
