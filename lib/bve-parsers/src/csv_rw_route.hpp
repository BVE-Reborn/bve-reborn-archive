#pragma once

#include "core/datatypes.hpp"
#include "csv_rw_route/instructions.hpp"
#include "csv_rw_route/route_structure.hpp"
#include "parsers/errors.hpp"
#include "parsers/find_relative_file.hpp"
#include <functional>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace bve::parsers::csv_rw_route {
	enum class file_type { csv, rw };

	struct preprocessed_line {
		std::string contents;
		std::size_t filename_index;
		std::size_t line;
		float offset;
	};

	struct preprocessed_lines {
		std::vector<preprocessed_line> lines;
		std::vector<std::string> filenames;
	};

	preprocessed_lines process_include_directives(const std::string& filename,
	                                              bve::core::datatypes::rng& rng,
	                                              errors::MultiError& errors,
	                                              file_type ft,
	                                              const find_relative_file_func& get_abs_path);

	void preprocess_file(preprocessed_lines& lines,
	                     bve::core::datatypes::rng& rng,
	                     errors::MultiError& errors,
	                     file_type ft);

	using instruction = mapbox::util::variant<instructions::naked::None,
	                                          instructions::naked::Position,
	                                          // options commands
	                                          instructions::options::UnitOfLength,
	                                          instructions::options::UnitOfSpeed,
	                                          instructions::options::BlockLength,
	                                          instructions::options::ObjectVisibility,
	                                          instructions::options::SectionBehavior,
	                                          instructions::options::CantBehavior,
	                                          instructions::options::FogBehavior,
	                                          instructions::options::CompatibleTransparencyMode,
	                                          instructions::options::EnableBveTsHacks,
	                                          // route commands
	                                          instructions::route::Comment,
	                                          instructions::route::Image,
	                                          instructions::route::Timetable,
	                                          instructions::route::Change,
	                                          instructions::route::Gauge,
	                                          instructions::route::Signal,
	                                          instructions::route::RunInterval,
	                                          instructions::route::AccelerationDueToGravity,
	                                          instructions::route::Elevation,
	                                          instructions::route::Temperature,
	                                          instructions::route::Pressure,
	                                          instructions::route::DisplaySpeed,
	                                          instructions::route::LoadingScreen,
	                                          instructions::route::StartTime,
	                                          instructions::route::DynamicLight,
	                                          instructions::route::AmbientLight,
	                                          instructions::route::DirectionalLight,
	                                          instructions::route::LightDirection,
	                                          // train commands
	                                          instructions::train::Folder,
	                                          instructions::train::Rail,
	                                          instructions::train::Flange,
	                                          instructions::train::Timetable,
	                                          instructions::train::Velocity,
	                                          // structure commands
	                                          instructions::structure::Command,
	                                          instructions::structure::Pole,
	                                          // texture commands
	                                          instructions::texture::BackgroundLoad,
	                                          instructions::texture::BackgroundX,
	                                          instructions::texture::BackgroundAspect,
	                                          // cycle commands
	                                          instructions::cycle::Ground,
	                                          instructions::cycle::Rail,
	                                          // signal commands
	                                          instructions::naked::SignalAnimated,
	                                          instructions::naked::Signal,
	                                          // track,
	                                          instructions::track::RailStart,
	                                          instructions::track::Rail,
	                                          instructions::track::RailType,
	                                          instructions::track::RailEnd,
	                                          instructions::track::Adhesion,
	                                          instructions::track::Pitch,
	                                          instructions::track::Curve,
	                                          instructions::track::Turn,
	                                          instructions::track::Height,
	                                          instructions::track::FreeObj,
	                                          instructions::track::Wall,
	                                          instructions::track::WallEnd,
	                                          instructions::track::Dike,
	                                          instructions::track::DikeEnd,
	                                          instructions::track::Pole,
	                                          instructions::track::PoleEnd,
	                                          instructions::track::Crack,
	                                          instructions::track::Ground,
	                                          instructions::track::Sta,
	                                          instructions::track::StationXml,
	                                          instructions::track::Stop,
	                                          instructions::track::Form,
	                                          instructions::track::Limit,
	                                          instructions::track::Section,
	                                          instructions::track::SigF,
	                                          instructions::track::Signal,
	                                          instructions::track::Relay,
	                                          instructions::track::Beacon,
	                                          instructions::track::Transponder,
	                                          instructions::track::Pattern,
	                                          instructions::track::Back,
	                                          instructions::track::Fog,
	                                          instructions::track::Brightness,
	                                          instructions::track::Marker,
	                                          instructions::track::marker_xml,
	                                          instructions::track::TextMarker,
	                                          instructions::track::PointOfInterest,
	                                          instructions::track::PreTrain,
	                                          instructions::track::Announce,
	                                          instructions::track::Doppler,
	                                          instructions::track::Buffer>;

	struct instruction_list {
		std::vector<instruction> instructions;
		std::vector<std::string> filenames;
	};

	namespace line_splitting {
		struct instruction_info {
			std::string name;
			std::vector<std::string> indices;
			std::vector<std::string> args;
			std::string suffix;
			float offset;
			bool track_position = false;
		};

		instruction_info csv(const preprocessed_line& l);
		instruction_info rw(const preprocessed_line& l);
	} // namespace line_splitting

	instruction_list generate_instructions(const preprocessed_lines& lines,
	                                       errors::MultiError& errors,
	                                       file_type ft);

	void execute_instructions_pass1(instruction_list& list, errors::MultiError& errors);
	parsed_route_data execute_instructions_pass2(instruction_list& list,
	                                             errors::MultiError& errors);
	void execute_instructions_pass3(parsed_route_data& rd,
	                                instruction_list& list,
	                                errors::MultiError& errors,
	                                const find_relative_file_func& get_abs_path);

} // namespace bve::parsers::csv_rw_route

std::ostream& operator<<(std::ostream& os, const bve::parsers::csv_rw_route::instruction& i);
std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::csv_rw_route::instruction_list& list);
