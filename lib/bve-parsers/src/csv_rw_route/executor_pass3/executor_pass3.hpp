#pragma once

#include "core/math.hpp"
#include "core/pair_hash.hpp"
#include "csv_rw_route.hpp"
#include "parsers/find_relative_file.hpp"
#include "utils.hpp"
#include <functional>
#include <iosfwd>
#include <iostream>
#include <vector>

namespace bve::parsers::csv_rw_route {
	using cycle_type = std::vector<std::size_t>;

	// defined in executor_pass3/cycle.cpp
	tl::optional<filename_set_iterator> get_cycle_filename_index(
	    const std::unordered_map<std::size_t, std::vector<std::size_t>>& cycle_mapping,
	    const std::unordered_map<std::size_t, filename_set_iterator>& object_mapping,
	    std::size_t index,
	    std::size_t position);
	void print_cycle_type(std::ostream& o, const cycle_type& c);

	struct rail_state {
		float x_offset = 0;
		float y_offset = 0;
		std::size_t rail_structure_index = 0;
		float position_last_updated = 0;
		bool active = false;

		std::size_t ground_index = 0;
		float position_ground_updated = 0;

		std::size_t wall_l_index = 0;
		std::size_t wall_r_index = 0;
		std::size_t dike_l_index = 0;
		std::size_t dike_r_index = 0;

		std::size_t pole_additional_rails = 0;
		std::intmax_t pole_location = 0;
		std::intmax_t pole_interval = 0;
		std::size_t pole_structure_index = 0;

		float position_wall_l_updated = 0;
		float position_wall_r_updated = 0;
		float position_dike_l_updated = 0;
		float position_dike_r_updated = 0;
		float position_pole_updated = 0;

		bool wall_l_active = false;
		bool wall_r_active = false;
		bool dike_l_active = false;
		bool dike_r_active = false;
		bool pole_active = false;
	};

	struct pass3_executor {
	  private:
		errors::MultiError& errors_;
		const std::vector<std::string>& filenames_;
		parsed_route_data& route_data_;
		const find_relative_file_func& get_relative_file_;

		// state variables
		std::vector<float> units_of_length_ = {1, 1};
		float unit_of_speed_ = 1;
		instructions::options::SectionBehavior::Mode section_behavior_ =
		    instructions::options::SectionBehavior::Mode::normal;

		// rall state
		std::unordered_map<std::size_t, rail_state> current_rail_state_ = {
		    //
		    {std::size_t(0), rail_state{0, 0, 0, 0, true}},
		    //
		};

		// structures and poles
		std::unordered_map<std::size_t, filename_set_iterator> object_ground_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_rail_mapping_;
		std::unordered_map<std::size_t, std::vector<std::size_t>> cycle_ground_mapping_;
		std::unordered_map<std::size_t, std::vector<std::size_t>> cycle_rail_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_wall_l_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_wall_r_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_dike_l_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_dike_r_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_form_l_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_form_r_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_form_cl_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_form_cr_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_roof_l_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_roof_r_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_roof_cl_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_roof_cr_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_crack_l_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_crack_r_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_freeobj_mapping_;
		std::unordered_map<std::size_t, filename_set_iterator> object_beacon_mapping_;
		// Poles are unique based on the number of rails as well as the pole
		// structure index
		std::unordered_map<std::pair<std::size_t, std::size_t>,
		                   filename_set_iterator,
		                   bve::core::hash::pair_hash>
		    object_pole_mapping_;

		// Background indices
		std::unordered_map<std::size_t, xml::dynamic_background::parsed_dynamic_background>
		    background_mapping_;

		// Signal indices
		std::unordered_map<std::size_t, signal_info> signal_mapping_;

		// error checking values
		bool used_dynamic_light_ = false;

		struct file_index_line_pair {
			std::size_t file_index;
			std::size_t line;
		};

		// blame indices to get the line number that committed an error
		// these are verified in the verificiation pass
		// TODO(sirflankalot): wtf are these blame pairs?
		std::unordered_map<std::size_t, file_index_line_pair> rail_runsound_blame_;
		std::unordered_map<std::size_t, file_index_line_pair> rail_flangesound_blame_;

		// helper functions
		const std::string& get_filename(std::size_t const index) const {
			return filenames_[index];
		}

		filename_set_iterator add_object_filename(std::string const& val) const {
			return route_data_.object_filenames.insert(util::lower_copy(val)).first;
		}

		filename_set_iterator add_texture_filename(std::string const& val) const {
			return route_data_.texture_filenames.insert(util::lower_copy(val)).first;
		}

		filename_set_iterator add_sound_filename(std::string const& val) const {
			return route_data_.sound_filenames.insert(util::lower_copy(val)).first;
		}

		// defined in executor_pass3/util.cpp
		rail_state& getRailState(std::size_t index);
		float ground_height_at(float position) const;
		bve::core::math::evaulate_curve_t track_position_at(float position) const;
		glm::vec3 position_relative_to_rail(std::size_t rail_num,
		                                    float position,
		                                    float x_offset,
		                                    float y_offset);

	  public:
		pass3_executor(parsed_route_data& rd,
		               errors::MultiError& e,
		               const std::vector<std::string>& fn,
		               const find_relative_file_func& grf) :
		    errors_(e),
		    filenames_(fn),
		    route_data_(rd),
		    get_relative_file_(grf) {}

		// defined in executor_pass3/finalize.cpp
		// ensure all state is dumped to the structure
		void finalize(float max_position);

		// unused instructions
		template <class T>
		void operator()(const T& /*unused*/) {}

		// defined in executor_pass3/options.cpp
		void operator()(const instructions::options::UnitOfLength& /*inst*/);
		void operator()(const instructions::options::UnitOfSpeed& /*inst*/);
		void operator()(const instructions::options::SectionBehavior& /*inst*/);
		/*void operator()(const instructions::options::FogBehavior&);*/
		void operator()(const instructions::options::CompatibleTransparencyMode& /*inst*/) const;
		void operator()(const instructions::options::EnableBveTsHacks& /*inst*/) const;

		// defined in executor_pass3/route.cpp
		void operator()(const instructions::route::Comment& /*inst*/) const;
		void operator()(const instructions::route::Image& /*inst*/) const;
		void operator()(const instructions::route::Timetable& /*inst*/) const;
		void operator()(const instructions::route::Change& /*inst*/) const;
		void operator()(const instructions::route::Gauge& /*inst*/) const;
		void operator()(const instructions::route::Signal& /*inst*/) const;
		void operator()(const instructions::route::RunInterval& /*inst*/) const;
		void operator()(const instructions::route::AccelerationDueToGravity& /*inst*/) const;
		void operator()(const instructions::route::Elevation& /*inst*/);
		void operator()(const instructions::route::Temperature& /*inst*/) const;
		void operator()(const instructions::route::Pressure& /*inst*/) const;
		void operator()(const instructions::route::DisplaySpeed& /*inst*/) const;
		void operator()(const instructions::route::LoadingScreen& /*inst*/) const;
		void operator()(const instructions::route::StartTime& /*inst*/) const;
		void operator()(const instructions::route::DynamicLight& /*inst*/);
		void operator()(const instructions::route::AmbientLight& /*inst*/) const;
		void operator()(const instructions::route::DirectionalLight& /*inst*/) const;
		void operator()(const instructions::route::LightDirection& /*inst*/) const;

		// defined in executor_pass3/train.cpp
		void operator()(const instructions::train::Folder& /*inst*/) const;
		void operator()(const instructions::train::Rail& /*inst*/);
		void operator()(const instructions::train::Flange& /*inst*/);
		void operator()(const instructions::train::Timetable& /*inst*/) const;
		void operator()(const instructions::train::Velocity& /*inst*/) const;

		// defined in executor_pass3/structure.cpp
		void operator()(const instructions::structure::Command& /*inst*/);
		void operator()(const instructions::structure::Pole& /*inst*/);

		// defined in executor_pass3/texture.cpp
		// helper functions for background_load
	  private:
		void background_load_xml(const instructions::texture::BackgroundLoad& /*inst*/);
		void background_load_image(const instructions::texture::BackgroundLoad& /*inst*/);

	  public:
		void operator()(const instructions::texture::BackgroundLoad& /*inst*/);
		void operator()(const instructions::texture::BackgroundX& /*inst*/);
		void operator()(const instructions::texture::BackgroundAspect& /*inst*/);

		// defined in executor_pass3/cycle.cpp
		void operator()(const instructions::cycle::Ground& /*inst*/);
		void operator()(const instructions::cycle::Rail& /*inst*/);

		// defined in executor_pass3/signal.cpp
		void operator()(const instructions::naked::SignalAnimated& /*inst*/);
		void operator()(const instructions::naked::Signal& /*inst*/);

	  private:
		void addRailObjectsToPosition(rail_state& state, float position) const;

	  public:
		// defined in executor_pass3/rails.cpp
		void operator()(const instructions::track::RailStart& /*inst*/);
		void operator()(const instructions::track::Rail& /*inst*/);
		void operator()(const instructions::track::RailType& /*inst*/);
		void operator()(const instructions::track::RailEnd& /*inst*/);
		void operator()(const instructions::track::Adhesion& /*inst*/) const;

	  private:
		void add_wall_objects_up_to_position(rail_state& state, float position, uint8_t type);
		void add_poll_objects_up_to_position(std::size_t rail_number,
		                                     rail_state& state,
		                                     float position);
		void add_ground_objects_up_to_position(rail_state& state, float position) const;

	  public:
		// defined in executor_pass3/objects.cpp
		void operator()(const instructions::track::FreeObj& /*inst*/);
		void operator()(const instructions::track::Wall& /*inst*/);
		void operator()(const instructions::track::WallEnd& /*inst*/);
		void operator()(const instructions::track::Dike& /*inst*/);
		void operator()(const instructions::track::DikeEnd& /*inst*/);
		void operator()(const instructions::track::Pole& /*inst*/);
		void operator()(const instructions::track::PoleEnd& /*inst*/);
		void operator()(const instructions::track::Crack& /*inst*/) const;
		void operator()(const instructions::track::Ground& /*inst*/);

		// defined in executor_pass3/stations.cpp
		void operator()(const instructions::track::Sta& /*inst*/) const;
		void operator()(const instructions::track::StationXml& /*inst*/) const;
		void operator()(const instructions::track::Stop& /*inst*/) const;
		void operator()(const instructions::track::Form& /*inst*/) const;

		// defined in executor_pass3/signalling.cpp
		void operator()(const instructions::track::Limit& /*inst*/) const;
		void operator()(const instructions::track::Section& /*inst*/) const;
		void operator()(const instructions::track::SigF& /*inst*/);
		void operator()(const instructions::track::Signal& /*inst*/);
		void operator()(const instructions::track::Relay& /*inst*/);

		// defined in executor_pass3/safety.cpp
		void operator()(const instructions::track::Beacon& /*inst*/);
		void operator()(const instructions::track::Transponder& /*unused*/);
		void operator()(const instructions::track::Pattern& /*unused*/) const;

		// defined in executor_pass3/misc.cpp
		void operator()(const instructions::track::Back& /*unused*/);
		void operator()(const instructions::track::Fog& /*unused*/);
		void operator()(const instructions::track::Brightness& /*unused*/) const;
		void operator()(const instructions::track::Marker& /*unused*/) const;
		void operator()(const instructions::track::marker_xml& /*unused*/) const;
		void operator()(const instructions::track::TextMarker& /*unused*/) const;
		void operator()(const instructions::track::PointOfInterest& /*unused*/);
		void operator()(const instructions::track::PreTrain& /*unused*/) const;
		void operator()(const instructions::track::Announce& /*unused*/) const;
		void operator()(const instructions::track::Doppler& /*unused*/);
		void operator()(const instructions::track::Buffer& /*unused*/) const;
	};
} // namespace bve::parsers::csv_rw_route
