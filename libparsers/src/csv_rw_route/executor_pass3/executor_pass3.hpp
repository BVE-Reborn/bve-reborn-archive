#pragma once

#include "core/math.hpp"
#include "csv_rw_route.hpp"
#include "parsers/find_relative_file.hpp"
#include "utils.hpp"
#include <boost/functional/hash.hpp>
#include <functional>
#include <iosfwd>

namespace parsers {
namespace csv_rw_route {
	using cycle_type = std::vector<std::size_t>;

	// defined in executor_pass3/cycle.cpp
	boost::optional<filename_set_iterator> get_cycle_filename_index(
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
		errors::multi_error_t& errors_;
		const std::vector<std::string>& filenames_;
		parsed_route_data& route_data_;
		const find_relative_file_func& get_relative_file_;

		// state variables
		std::vector<float> units_of_length_ = {1, 1};
		float unit_of_speed_ = 1;
		decltype(instructions::options::section_behavior::mode) section_behavior_ =
		    instructions::options::section_behavior::normal;

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
		                   boost::hash<std::pair<std::size_t, std::size_t>>>
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
		rail_state& get_rail_state(std::size_t index);
		float ground_height_at(float position) const;
		openbve2::math::evaulate_curve_t track_position_at(float position) const;
		glm::vec3 position_relative_to_rail(std::size_t rail_num,
		                                    float position,
		                                    float x_offset,
		                                    float y_offset);

	  public:
		pass3_executor(parsed_route_data& rd,
		               errors::multi_error_t& e,
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
		void operator()(const instructions::options::unit_of_length& /*inst*/);
		void operator()(const instructions::options::unit_of_speed& /*inst*/);
		void operator()(const instructions::options::section_behavior& /*inst*/);
		/*void operator()(const instructions::options::FogBehavior&);*/
		void operator()(const instructions::options::compatible_transparency_mode& /*inst*/) const;
		void operator()(const instructions::options::enable_bve_ts_hacks& /*inst*/) const;

		// defined in executor_pass3/route.cpp
		void operator()(const instructions::route::comment& /*inst*/) const;
		void operator()(const instructions::route::image& /*inst*/) const;
		void operator()(const instructions::route::timetable& /*inst*/) const;
		void operator()(const instructions::route::change& /*inst*/) const;
		void operator()(const instructions::route::guage& /*inst*/) const;
		void operator()(const instructions::route::signal& /*inst*/) const;
		void operator()(const instructions::route::run_interval& /*inst*/) const;
		void operator()(const instructions::route::acceleration_due_to_gravity& /*inst*/) const;
		void operator()(const instructions::route::elevation& /*inst*/);
		void operator()(const instructions::route::temperature& /*inst*/) const;
		void operator()(const instructions::route::pressure& /*inst*/) const;
		void operator()(const instructions::route::display_speed& /*inst*/) const;
		void operator()(const instructions::route::loading_screen& /*inst*/) const;
		void operator()(const instructions::route::start_time& /*inst*/) const;
		void operator()(const instructions::route::dynamic_light& /*inst*/);
		void operator()(const instructions::route::ambiant_light& /*inst*/) const;
		void operator()(const instructions::route::directional_light& /*inst*/) const;
		void operator()(const instructions::route::light_direction& /*inst*/) const;

		// defined in executor_pass3/train.cpp
		void operator()(const instructions::train::folder& /*inst*/) const;
		void operator()(const instructions::train::rail& /*inst*/);
		void operator()(const instructions::train::flange& /*inst*/);
		void operator()(const instructions::train::timetable& /*inst*/) const;
		void operator()(const instructions::train::velocity& /*inst*/) const;

		// defined in executor_pass3/structure.cpp
		void operator()(const instructions::structure::command& /*inst*/);
		void operator()(const instructions::structure::pole& /*inst*/);

		// defined in executor_pass3/texture.cpp
		// helper functions for background_load
	  private:
		void background_load_xml(const instructions::texture::background_load& /*inst*/);
		void background_load_image(const instructions::texture::background_load& /*inst*/);

	  public:
		void operator()(const instructions::texture::background_load& /*inst*/);
		void operator()(const instructions::texture::background_x& /*inst*/);
		void operator()(const instructions::texture::background_aspect& /*inst*/);

		// defined in executor_pass3/cycle.cpp
		void operator()(const instructions::cycle::ground& /*inst*/);
		void operator()(const instructions::cycle::rail& /*inst*/);

		// defined in executor_pass3/signal.cpp
		void operator()(const instructions::naked::signal_animated& /*inst*/);
		void operator()(const instructions::naked::signal& /*inst*/);

	  private:
		void add_rail_objects_up_to_position(rail_state& state, float position) const;

	  public:
		// defined in executor_pass3/rails.cpp
		void operator()(const instructions::track::rail_start& /*inst*/);
		void operator()(const instructions::track::rail& /*inst*/);
		void operator()(const instructions::track::rail_type& /*inst*/);
		void operator()(const instructions::track::rail_end& /*inst*/);
		void operator()(const instructions::track::adhesion& /*inst*/) const;

	  private:
		void add_wall_objects_up_to_position(rail_state& state, float position, uint8_t type);
		void add_poll_objects_up_to_position(std::size_t rail_number,
		                                     rail_state& state,
		                                     float position);
		void add_ground_objects_up_to_position(rail_state& state, float position) const;

	  public:
		// defined in executor_pass3/objects.cpp
		void operator()(const instructions::track::free_obj& /*inst*/);
		void operator()(const instructions::track::wall& /*inst*/);
		void operator()(const instructions::track::wall_end& /*inst*/);
		void operator()(const instructions::track::dike& /*inst*/);
		void operator()(const instructions::track::dike_end& /*inst*/);
		void operator()(const instructions::track::pole& /*inst*/);
		void operator()(const instructions::track::pole_end& /*inst*/);
		void operator()(const instructions::track::crack& /*inst*/) const;
		void operator()(const instructions::track::ground& /*inst*/);

		// defined in executor_pass3/stations.cpp
		void operator()(const instructions::track::sta& /*inst*/) const;
		void operator()(const instructions::track::station_xml& /*inst*/) const;
		void operator()(const instructions::track::stop& /*inst*/) const;
		void operator()(const instructions::track::form& /*inst*/) const;

		// defined in executor_pass3/signalling.cpp
		void operator()(const instructions::track::limit& /*inst*/) const;
		void operator()(const instructions::track::section& /*inst*/) const;
		void operator()(const instructions::track::sig_f& /*inst*/);
		void operator()(const instructions::track::signal& /*inst*/);
		void operator()(const instructions::track::relay& /*inst*/);

		// defined in executor_pass3/safety.cpp
		void operator()(const instructions::track::beacon& /*inst*/);
		void operator()(const instructions::track::transponder& /*unused*/);
		void operator()(const instructions::track::pattern& /*unused*/) const;

		// defined in executor_pass3/misc.cpp
		void operator()(const instructions::track::back& /*unused*/);
		void operator()(const instructions::track::fog& /*unused*/);
		void operator()(const instructions::track::brightness& /*unused*/) const;
		void operator()(const instructions::track::marker& /*unused*/) const;
		void operator()(const instructions::track::marker_xml& /*unused*/) const;
		void operator()(const instructions::track::text_marker& /*unused*/) const;
		void operator()(const instructions::track::point_of_interest& /*unused*/);
		void operator()(const instructions::track::pre_train& /*unused*/) const;
		void operator()(const instructions::track::announce& /*unused*/) const;
		void operator()(const instructions::track::doppler& /*unused*/);
		void operator()(const instructions::track::buffer& /*unused*/) const;
	};
} // namespace csv_rw_route
} // namespace parsers
