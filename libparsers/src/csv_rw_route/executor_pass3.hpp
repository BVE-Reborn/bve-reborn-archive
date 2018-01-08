#pragma once

#include "csv_rw_route.hpp"
#include "parsers/find_relative_file.hpp"
#include <boost/functional/hash.hpp>
#include <functional>

namespace parsers {
namespace csv_rw_route {
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

		// structures and poles
		std::unordered_map<std::size_t, std::string> object_ground_mapping;
		std::unordered_map<std::size_t, std::string> object_rail_mapping;
		std::unordered_map<std::size_t, std::string> object_wallL_mapping;
		std::unordered_map<std::size_t, std::string> object_wallR_mapping;
		std::unordered_map<std::size_t, std::string> object_dikeL_mapping;
		std::unordered_map<std::size_t, std::string> object_dikeR_mapping;
		std::unordered_map<std::size_t, std::string> object_formL_mapping;
		std::unordered_map<std::size_t, std::string> object_formR_mapping;
		std::unordered_map<std::size_t, std::string> object_formCL_mapping;
		std::unordered_map<std::size_t, std::string> object_formCR_mapping;
		std::unordered_map<std::size_t, std::string> object_roofL_mapping;
		std::unordered_map<std::size_t, std::string> object_roofR_mapping;
		std::unordered_map<std::size_t, std::string> object_roofCL_mapping;
		std::unordered_map<std::size_t, std::string> object_roofCR_mapping;
		std::unordered_map<std::size_t, std::string> object_crackL_mapping;
		std::unordered_map<std::size_t, std::string> object_crackR_mapping;
		std::unordered_map<std::size_t, std::string> object_freeobj_mapping;
		std::unordered_map<std::size_t, std::string> object_beacon_mapping;
		// Poles are unique based on the number of rails as well as the pole structure index
		std::unordered_map<std::pair<std::size_t, std::size_t>, std::string,
		                   boost::hash<std::pair<std::size_t, std::size_t>>>
		    object_pole_mapping;

		// error checking values
		bool used_dynamic_light = false;
		bool used_dynamic_background = false;

		struct file_index_line_pair {
			std::size_t file_index;
			std::size_t line;
		};

		// blame indices to get the line number that committed an error
		// these are verified in the verificiation pass
		std::unordered_map<std::size_t, file_index_line_pair> rail_runsound_blame;
		std::unordered_map<std::size_t, file_index_line_pair> rail_flangesound_blame;

		const std::string& get_filename(std::size_t index) {
			return _filenames[index];
		}

	  public:
		pass3_executor(parsed_route_data& rd, errors::multi_error& e, const std::vector<std::string>& fn,
		               const find_relative_file_func& grf)
		    : _errors(e), _filenames(fn), _route_data(rd), _get_relative_file(grf){};

		// unused instructions
		template <class T>
		void operator()(const T&) {}

		// defined in executor_pass3_options.cpp
		void operator()(const instructions::options::UnitOfLength&);
		void operator()(const instructions::options::UnitOfSpeed&);
		void operator()(const instructions::options::SectionBehavior&);
		/*void operator()(const instructions::options::FogBehavior&);*/
		void operator()(const instructions::options::CompatibleTransparencyMode&);
		void operator()(const instructions::options::EnableBveTsHacks&);

		// defined in executor_pass3_route.cpp
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

		// defined in executor_pass3_train.cpp
		void operator()(const instructions::train::Folder&);
		void operator()(const instructions::train::Rail&);
		void operator()(const instructions::train::Flange&);
		void operator()(const instructions::train::Timetable&);
		void operator()(const instructions::train::Velocity&);

		// defined in executor_pass3_structure.cpp
		void operator()(const instructions::structure::Command&);
		void operator()(const instructions::structure::Pole&);

		// defined in executor_pass3_texture.cpp
		void operator()(const instructions::texture::Background_Load&);
		void operator()(const instructions::texture::Background_X&);
		void operator()(const instructions::texture::Background_Aspect&);

		// defined in executor_pass3_structure.cpp
		void operator()(const instructions::cycle::Ground&);
		void operator()(const instructions::cycle::Rail&);

		// defined in executor_pass3_structure.cpp
		void operator()(const instructions::naked::SignalAnimated&);
		void operator()(const instructions::naked::Signal&);

		// defined in executor_pass3_structure
		void operator()(const instructions::track::RailStart&);
		void operator()(const instructions::track::Rail&);
		void operator()(const instructions::track::RailType&);
		void operator()(const instructions::track::RailEnd&);
		void operator()(const instructions::track::Adhesion&);
		void operator()(const instructions::track::Pitch&);
		void operator()(const instructions::track::Curve&);
		void operator()(const instructions::track::Turn&);
		void operator()(const instructions::track::Height&);
		void operator()(const instructions::track::FreeObj&);
		void operator()(const instructions::track::Wall&);
		void operator()(const instructions::track::WallEnd&);
		void operator()(const instructions::track::Dike&);
		void operator()(const instructions::track::DikeEnd&);
		void operator()(const instructions::track::Pole&);
		void operator()(const instructions::track::PoleEnd&);
		void operator()(const instructions::track::Crack&);
		void operator()(const instructions::track::Ground&);
		void operator()(const instructions::track::Sta&);
		void operator()(const instructions::track::Stop&);
		void operator()(const instructions::track::Form&);
		void operator()(const instructions::track::Limit&);
		void operator()(const instructions::track::Section&);
		void operator()(const instructions::track::SigF&);
		void operator()(const instructions::track::Signal&);
		void operator()(const instructions::track::Relay&);
		void operator()(const instructions::track::Beacon&);
		void operator()(const instructions::track::Transponder&);
		void operator()(const instructions::track::Pattern&);
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
