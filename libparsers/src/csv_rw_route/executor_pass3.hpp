#pragma once

#include "core/math.hpp"
#include "csv_rw_route.hpp"
#include "parsers/find_relative_file.hpp"
#include <boost/functional/hash.hpp>
#include <functional>

namespace parsers {
namespace csv_rw_route {
	struct rail_state {
		float x_offset = 0;
		float y_offset = 0;
		std::size_t rail_structure_type = 0;
		float postion_last_updated = 0;
		bool active = false;
	};

	struct pass3_executor {
	  private:
		errors::multi_error& _errors;
		const std::vector<std::string>& _filenames;
		parsed_route_data& _route_data;
		const find_relative_file_func& _get_relative_file;

		// state variables
		std::vector<float> units_of_length = {1, 1};
		float unit_of_speed = 1;
		decltype(instructions::options::SectionBehavior::mode) section_behavior;

		// rall state
		std::unordered_map<std::size_t, rail_state> current_rail_state = {{0, rail_state{0, 0, 0, 0, true}}};

		// structures and poles
		std::unordered_map<std::size_t, filename_set_iterator> object_ground_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_rail_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_wallL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_wallR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_dikeL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_dikeR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_formL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_formR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_formCL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_formCR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_roofL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_roofR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_roofCL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_roofCR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_crackL_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_crackR_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_freeobj_mapping;
		std::unordered_map<std::size_t, filename_set_iterator> object_beacon_mapping;
		// Poles are unique based on the number of rails as well as the pole structure index
		std::unordered_map<std::pair<std::size_t, std::size_t>, filename_set_iterator,
		                   boost::hash<std::pair<std::size_t, std::size_t>>>
		    object_pole_mapping;

		// Background indices
		std::unordered_map<std::size_t, xml::dynamic_backgrounds::parsed_dynamic_background> background_mapping;

		// Signal indices
		std::unordered_map<std::size_t, signal_info> signal_mapping;

		// error checking values
		bool used_dynamic_light = false;

		struct file_index_line_pair {
			std::size_t file_index;
			std::size_t line;
		};

		// blame indices to get the line number that committed an error
		// these are verified in the verificiation pass
		std::unordered_map<std::size_t, file_index_line_pair> rail_runsound_blame;
		std::unordered_map<std::size_t, file_index_line_pair> rail_flangesound_blame;

		// helper functions
		const std::string& get_filename(std::size_t index) {
			return _filenames[index];
		}

		filename_set_iterator add_object_filename(const std::string& val) {
			return _route_data.object_filenames.insert(val).first;
		}

		openbve2::math::evaulate_curve_t find_track_position_at(float position) {
			decltype(_route_data.blocks)::const_iterator starting_it;

			if (position < _route_data.blocks.front().position) {
				starting_it = _route_data.blocks.cbegin();
			}
			else {
				starting_it = std::upper_bound(_route_data.blocks.begin(), _route_data.blocks.end(), position,
				                               [](float a, const rail_block_info& b) { return a < b.position; });
				starting_it -= 1;
			}

			return openbve2::math::evaluate_curve(starting_it->cache.location, starting_it->cache.direction,
			                                      position - starting_it->position, starting_it->radius);
		}

		glm::vec3 get_position_relative_to_rail(std::size_t rail_num, float position, float x_offset, float y_offset) {
			auto track_postion = find_track_position_at(position);

			auto track_state_iter = current_rail_state.find(rail_num);

			return openbve2::math::postion_from_offsets(track_postion.position, track_postion.tangent,
			                                            track_state_iter->second.x_offset + x_offset,
			                                            track_state_iter->second.y_offset + y_offset);
		}

	  public:
		pass3_executor(parsed_route_data& rd, errors::multi_error& e, const std::vector<std::string>& fn,
		               const find_relative_file_func& grf)
		    : _errors(e), _filenames(fn), _route_data(rd), _get_relative_file(grf){};

		// unused instructions
		template <class T>
		void operator()(const T&) {}

		// defined in executor_pass3/options.cpp
		void operator()(const instructions::options::UnitOfLength&);
		void operator()(const instructions::options::UnitOfSpeed&);
		void operator()(const instructions::options::SectionBehavior&);
		/*void operator()(const instructions::options::FogBehavior&);*/
		void operator()(const instructions::options::CompatibleTransparencyMode&);
		void operator()(const instructions::options::EnableBveTsHacks&);

		// defined in executor_pass3/route.cpp
		void operator()(const instructions::route::Comment&);
		void operator()(const instructions::route::Image&);
		void operator()(const instructions::route::Timetable&);
		void operator()(const instructions::route::Change&);
		void operator()(const instructions::route::Guage&);
		void operator()(const instructions::route::Signal&);
		void operator()(const instructions::route::RunInterval&);
		void operator()(const instructions::route::AccelerationDueToGravity&);
		void operator()(const instructions::route::Elevation&);
		void operator()(const instructions::route::Temperature&);
		void operator()(const instructions::route::Pressure&);
		void operator()(const instructions::route::DisplaySpeed&);
		void operator()(const instructions::route::LoadingScreen&);
		void operator()(const instructions::route::StartTime&);
		void operator()(const instructions::route::DynamicLight&);
		void operator()(const instructions::route::AmbiantLight&);
		void operator()(const instructions::route::DirectionalLight&);
		void operator()(const instructions::route::LightDirection&);

		// defined in executor_pass3/train.cpp
		void operator()(const instructions::train::Folder&);
		void operator()(const instructions::train::Rail&);
		void operator()(const instructions::train::Flange&);
		void operator()(const instructions::train::Timetable&);
		void operator()(const instructions::train::Velocity&);

		// defined in executor_pass3/structure.cpp
		void operator()(const instructions::structure::Command&);
		void operator()(const instructions::structure::Pole&);

		// defined in executor_pass3/texture.cpp
		// helper functions for background_load
	  private:
		void background_load_xml(const instructions::texture::Background_Load&);
		void background_load_image(const instructions::texture::Background_Load&);

	  public:
		void operator()(const instructions::texture::Background_Load&);
		void operator()(const instructions::texture::Background_X&);
		void operator()(const instructions::texture::Background_Aspect&);

		// defined in executor_pass3/cycle.cpp
		void operator()(const instructions::cycle::Ground&);
		void operator()(const instructions::cycle::Rail&);

		// defined in executor_pass3/signal.cpp
		void operator()(const instructions::naked::SignalAnimated&);
		void operator()(const instructions::naked::Signal&);

	  private:
		void add_rail_objects_up_to_postion(rail_state& state, float position);

	  public:
		// defined in executor_pass3/rails.cpp
		void operator()(const instructions::track::RailStart&);
		void operator()(const instructions::track::Rail&);
		void operator()(const instructions::track::RailType&);
		void operator()(const instructions::track::RailEnd&);
		void operator()(const instructions::track::Adhesion&);

		// defined in executor_pass3/objects.cpp
		void operator()(const instructions::track::FreeObj&);
		void operator()(const instructions::track::Wall&);
		void operator()(const instructions::track::WallEnd&);
		void operator()(const instructions::track::Dike&);
		void operator()(const instructions::track::DikeEnd&);
		void operator()(const instructions::track::Pole&);
		void operator()(const instructions::track::PoleEnd&);
		void operator()(const instructions::track::Crack&);
		void operator()(const instructions::track::Ground&);

		// defined in executor_pass3/stations.cpp
		void operator()(const instructions::track::Sta&);
		void operator()(const instructions::track::Stop&);
		void operator()(const instructions::track::Form&);

		// defined in executor_pass3/signalling.cpp
		void operator()(const instructions::track::Limit&);
		void operator()(const instructions::track::Section&);
		void operator()(const instructions::track::SigF&);
		void operator()(const instructions::track::Signal&);
		void operator()(const instructions::track::Relay&);

		// defined in executor_pass3/safety.cpp
		void operator()(const instructions::track::Beacon&);
		void operator()(const instructions::track::Transponder&);
		void operator()(const instructions::track::Pattern&);

		// defined in executor_pass3/misc.cpp
		void operator()(const instructions::track::Back&);
		void operator()(const instructions::track::Fog&);
		void operator()(const instructions::track::Brightness&);
		void operator()(const instructions::track::Marker&);
		void operator()(const instructions::track::MarkerXML&);
		void operator()(const instructions::track::TextMarker&);
		void operator()(const instructions::track::PointOfInterest&);
		void operator()(const instructions::track::PreTrain&);
		void operator()(const instructions::track::Announce&);
		void operator()(const instructions::track::Doppler&);
		void operator()(const instructions::track::Buffer&);
	};
} // namespace csv_rw_route
} // namespace parsers
