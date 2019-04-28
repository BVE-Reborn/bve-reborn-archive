#pragma once

#include "parsers/csv_rw_route.hpp"
#include "parsers/find_relative_file.hpp"
#include "util/math.hpp"
#include "util/pair_hash.hpp"
#include "util/parsing.hpp"
#include <functional>
#include <iosfwd>
#include <iostream>
#include <vector>

namespace bve::parsers::csv_rw_route {
	using CycleType = std::vector<std::size_t>;

	// defined in executor_pass3/cycle.cpp
	absl::optional<FilenameSetIterator> get_cycle_filename_index(
	    const std::unordered_map<std::size_t, std::vector<std::size_t>>& cycle_mapping,
	    const std::unordered_map<std::size_t, FilenameSetIterator>& object_mapping,
	    std::size_t index,
	    std::size_t position);
	void print_cycle_type(std::ostream& o, const CycleType& c);

	struct RailState {
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

	struct Pass3Executor {
	  private:
		errors::MultiError& errors_;
		const std::vector<std::string>& filenames_;
		ParsedRoute& route_data_;
		const RelativeFileFunc& get_relative_file_;

		// state variables
		std::vector<float> units_of_length_ = {1, 1};
		float unit_of_speed_ = 1;
		instructions::options::SectionBehavior::Mode section_behavior_ = instructions::options::SectionBehavior::Mode::normal;

		// rail state
		std::unordered_map<std::size_t, RailState> current_rail_state_ = {
		    //
		    {std::size_t(0), RailState{0, 0, 0, 0, true}},
		    //
		};

		// structures and poles
		std::unordered_map<std::size_t, FilenameSetIterator> object_ground_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_rail_mapping_;
		std::unordered_map<std::size_t, std::vector<std::size_t>> cycle_ground_mapping_;
		std::unordered_map<std::size_t, std::vector<std::size_t>> cycle_rail_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_wall_l_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_wall_r_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_dike_l_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_dike_r_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_form_l_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_form_r_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_form_cl_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_form_cr_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_roof_l_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_roof_r_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_roof_cl_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_roof_cr_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_crack_l_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_crack_r_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_freeobj_mapping_;
		std::unordered_map<std::size_t, FilenameSetIterator> object_beacon_mapping_;
		// Poles are unique based on the number of rails as well as the pole
		// structure index
		std::unordered_map<std::pair<std::size_t, std::size_t>, FilenameSetIterator, util::hash::PairHash> object_pole_mapping_;

		// Background indices
		std::unordered_map<std::size_t, xml::dynamic_background::ParsedDynamicBackground> background_mapping_;

		// Signal indices
		std::unordered_map<std::size_t, SignalInfo> signal_mapping_;

		// error checking values
		bool used_dynamic_light_ = false;

		struct FileIndexLinePair {
			std::size_t file_index;
			std::size_t line;
		};

		// blame indices to get the line number that committed an error
		// these are verified in the verification pass
		// TODO(cwfitzgerald): wtf are these blame pairs?
		std::unordered_map<std::size_t, FileIndexLinePair> rail_runsound_blame_;
		std::unordered_map<std::size_t, FileIndexLinePair> rail_flangesound_blame_;

		// helper functions
		const std::string& getFilename(std::size_t const index) const {
			return filenames_[index];
		}

		FilenameSetIterator addObjectFilename(std::string const& val) const;

		FilenameSetIterator addTextureFilename(std::string const& val) const {
			return route_data_.texture_filenames.insert(util::parsers::lower_copy(val)).first;
		}

		FilenameSetIterator addSoundFilename(std::string const& val) const {
			return route_data_.sound_filenames.insert(util::parsers::lower_copy(val)).first;
		}

		// defined in executor_pass3/util.cpp
		RailState& getRailState(std::size_t index);
		float groundHeightAt(float position) const;
		util::math::EvaluateCurveState trackPositionAt(float position) const;
		glm::vec3 positionRelativeToRail(std::size_t rail_num, float position, float x_offset, float y_offset);

	  public:
		Pass3Executor(ParsedRoute& rd, errors::MultiError& e, const std::vector<std::string>& fn, const RelativeFileFunc& grf) :
		    errors_(e), filenames_(fn), route_data_(rd), get_relative_file_(grf) {}

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
		void backgroundLoadXML(const instructions::texture::BackgroundLoad& /*inst*/);
		void backgroundLoadImage(const instructions::texture::BackgroundLoad& /*inst*/);

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
		void addRailObjectsToPosition(RailState& state, float position) const;

	  public:
		// defined in executor_pass3/rails.cpp
		void operator()(const instructions::track::RailStart& /*inst*/);
		void operator()(const instructions::track::Rail& /*inst*/);
		void operator()(const instructions::track::RailType& /*inst*/);
		void operator()(const instructions::track::RailEnd& /*inst*/);
		void operator()(const instructions::track::Adhesion& /*inst*/) const;

	  private:
		void addWallObjectsToPosition(RailState& state, float position, uint8_t type);
		void addPollObjectsToPosition(std::size_t rail_number, RailState& state, float position);
		void addGroundObjectsToPosition(RailState& state, float position) const;

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
		void operator()(const instructions::track::MarkerXML& /*unused*/) const;
		void operator()(const instructions::track::TextMarker& /*unused*/) const;
		void operator()(const instructions::track::PointOfInterest& /*unused*/);
		void operator()(const instructions::track::PreTrain& /*unused*/) const;
		void operator()(const instructions::track::Announce& /*unused*/) const;
		void operator()(const instructions::track::Doppler& /*unused*/);
		void operator()(const instructions::track::Buffer& /*unused*/) const;
	};
} // namespace bve::parsers::csv_rw_route
