#pragma once

#include "parsers/csv_rw_route.hpp"
#include "util/macro_helpers.hpp"
#include <absl/types/optional.h>
#include <gsl/gsl_util>
#include <iomanip>
#include <ostream>

template <class T>
std::ostream& operator<<(std::ostream& os, absl::optional<T> const& val) {
	if (val.has_value()) {
		os << val.value();
	}
	else {
		os << '-';
	}
	return os;
}

#define PRINT_VALUE(name) os_ << ", " #name " = " << inst.name;
#define PRINT_BYTE(name) os_ << ", " #name " = " << gsl::narrow<int>(inst.name);
#define PRINT_VECTOR(name)   \
	os_ << ", " #name " = "; \
	printVector(inst.name);
#define PRINT_TIME(name)                                                                                                           \
	os_ << ", " #name " = " << std::setw(2) << std::setfill('0') << (inst.name / 3600) << ':' << std::setw(2) << std::setfill('0') \
	    << ((inst.name / 60) % 60) << ':' << std::setw(2) << std::setfill('0') << (inst.name % 60);
#define PRINT_COLOR(name)                                                                        \
	os_ << ", " #name " = "                                                                      \
	    << "(r = " << static_cast<int>(inst.name.r) << ", g = " << static_cast<int>(inst.name.g) \
	    << ", b = " << static_cast<int>(inst.name.b) << ')';

#define PRINT_ENUM1(name, nsp, value1)          \
	if (inst.name == instructions::nsp::value1) \
		os_ << ", " #name " = " #value1;
#define PRINT_ENUM2(name, nsp, value2, ...)     \
	if (inst.name == instructions::nsp::value2) \
		os_ << ", " #name " = " #value2;        \
	EXPAND(PRINT_ENUM1(name, nsp, __VA_ARGS__))
#define PRINT_ENUM3(name, nsp, value3, ...)     \
	if (inst.name == instructions::nsp::value3) \
		os_ << ", " #name " = " #value3;        \
	EXPAND(PRINT_ENUM2(name, nsp, __VA_ARGS__))
#define PRINT_ENUM4(name, nsp, value4, ...)     \
	if (inst.name == instructions::nsp::value4) \
		os_ << ", " #name " = " #value4;        \
	EXPAND(PRINT_ENUM3(name, nsp, __VA_ARGS__))
#define PRINT_ENUM5(name, nsp, value5, ...)     \
	if (inst.name == instructions::nsp::value5) \
		os_ << ", " #name " = " #value5;        \
	EXPAND(PRINT_ENUM4(name, nsp, __VA_ARGS__))
#define PRINT_ENUM6(name, nsp, value6, ...)     \
	if (inst.name == instructions::nsp::value6) \
		os_ << ", " #name " = " #value6;        \
	EXPAND(PRINT_ENUM5(name, nsp, __VA_ARGS__))
#define PRINT_ENUM7(name, nsp, value7, ...)     \
	if (inst.name == instructions::nsp::value7) \
		os_ << ", " #name " = " #value7;        \
	EXPAND(PRINT_ENUM6(name, nsp, __VA_ARGS__))
#define PRINT_ENUM8(name, nsp, value8, ...)     \
	if (inst.name == instructions::nsp::value8) \
		os_ << ", " #name " = " #value8;        \
	EXPAND(PRINT_ENUM7(name, nsp, __VA_ARGS__))
#define PRINT_ENUM9(name, nsp, value9, ...)     \
	if (inst.name == instructions::nsp::value9) \
		os_ << ", " #name " = " #value9;        \
	EXPAND(PRINT_ENUM8(name, nsp, __VA_ARGS__))
#define PRINT_ENUM10(name, nsp, value10, ...)    \
	if (inst.name == instructions::nsp::value10) \
		os_ << ", " #name " = " #value10;        \
	EXPAND(PRINT_ENUM9(name, nsp, __VA_ARGS__))
#define PRINT_ENUM11(name, nsp, value11, ...)    \
	if (inst.name == instructions::nsp::value11) \
		os_ << ", " #name " = " #value11;        \
	EXPAND(PRINT_ENUM10(name, nsp, __VA_ARGS__))
#define PRINT_ENUM12(name, nsp, value12, ...)    \
	if (inst.name == instructions::nsp::value12) \
		os_ << ", " #name " = " #value12;        \
	EXPAND(PRINT_ENUM11(name, nsp, __VA_ARGS__))
#define PRINT_ENUM13(name, nsp, value13, ...)    \
	if (inst.name == instructions::nsp::value13) \
		os_ << ", " #name " = " #value13;        \
	EXPAND(PRINT_ENUM12(name, nsp, __VA_ARGS__))
#define PRINT_ENUM14(name, nsp, value14, ...)    \
	if (inst.name == instructions::nsp::value14) \
		os_ << ", " #name " = " #value14;        \
	EXPAND(PRINT_ENUM13(name, nsp, __VA_ARGS__))
#define PRINT_ENUM15(name, nsp, value15, ...)    \
	if (inst.name == instructions::nsp::value15) \
		os_ << ", " #name " = " #value15;        \
	EXPAND(PRINT_ENUM14(name, nsp, __VA_ARGS__))
#define PRINT_ENUM16(name, nsp, value16, ...)    \
	if (inst.name == instructions::nsp::value16) \
		os_ << ", " #name " = " #value16;        \
	EXPAND(PRINT_ENUM15(name, nsp, __VA_ARGS__))
#define PRINT_ENUM17(name, nsp, value17, ...)    \
	if (inst.name == instructions::nsp::value17) \
		os_ << ", " #name " = " #value17;        \
	EXPAND(PRINT_ENUM16(name, nsp, __VA_ARGS__))
#define PRINT_ENUM18(name, nsp, value18, ...)    \
	if (inst.name == instructions::nsp::value18) \
		os_ << ", " #name " = " #value18;        \
	EXPAND(PRINT_ENUM17(name, nsp, __VA_ARGS__))

namespace bve::parsers::csv_rw_route {
	struct CSVRwRouteInstructionIOClass {
	  private:
		std::ostream& os_;
		const std::vector<std::string> filenames_;
		bool has_filenames_;

	  public:
		explicit CSVRwRouteInstructionIOClass(std::ostream& os) : os_(os), has_filenames_(false) {}
		explicit CSVRwRouteInstructionIOClass(std::ostream& os, std::vector<std::string> filenames) :
		    os_(os),                          // clang-format can't decide how to format this, so I will
		    filenames_(std::move(filenames)), //
		    has_filenames_(true)              //
		{}                                    //

		template <class T>
		void printVector(const std::vector<T>& vec) {
			std::size_t i = 0;
			os_ << "[";
			for (auto& item : vec) {
				os_ << item;
				if (++i != vec.size()) {
					os_ << ", ";
				}
			}
			os_ << "]";
		}

		template <class T>
		void start(const T& inst, const char* name) {
			os_ << inst.absolute_position << ", " << name;
			if (has_filenames_) {
				os_ << ", filename = " << filenames_[inst.file_index];
			}
			else {
				os_ << ", file_index = " << inst.file_index;
			}
			os_ << ", line = " << inst.line;
		}

		static void end() {}

		void operator()(const instructions::naked::Position& inst);
		void operator()(const instructions::naked::None& inst);
		void operator()(const instructions::options::UnitOfLength& inst);
		void operator()(const instructions::options::UnitOfSpeed& inst);
		void operator()(const instructions::options::BlockLength& inst);
		void operator()(const instructions::options::ObjectVisibility& inst);
		void operator()(const instructions::options::SectionBehavior& inst);
		void operator()(const instructions::options::CantBehavior& inst);
		void operator()(const instructions::options::FogBehavior& inst);
		void operator()(const instructions::options::CompatibleTransparencyMode& inst);
		void operator()(const instructions::options::EnableBveTsHacks& inst);
		void operator()(const instructions::route::Comment& inst);
		void operator()(const instructions::route::Image& inst);
		void operator()(const instructions::route::Timetable& inst);
		void operator()(const instructions::route::Change& inst);
		void operator()(const instructions::route::Gauge& inst);
		void operator()(const instructions::route::Signal& inst);
		void operator()(const instructions::route::RunInterval& inst);
		void operator()(const instructions::route::AccelerationDueToGravity& inst);
		void operator()(const instructions::route::Elevation& inst);
		void operator()(const instructions::route::Temperature& inst);
		void operator()(const instructions::route::Pressure& inst);
		void operator()(const instructions::route::DisplaySpeed& inst);
		void operator()(const instructions::route::LoadingScreen& inst);
		void operator()(const instructions::route::StartTime& inst);
		void operator()(const instructions::route::DynamicLight& inst);
		void operator()(const instructions::route::AmbientLight& inst);
		void operator()(const instructions::route::DirectionalLight& inst);
		void operator()(const instructions::route::LightDirection& inst);
		void operator()(const instructions::train::Folder& inst);
		void operator()(const instructions::train::Rail& inst);
		void operator()(const instructions::train::Flange& inst);
		void operator()(const instructions::train::Timetable& inst);
		void operator()(const instructions::train::Velocity& inst);
		void operator()(const instructions::structure::Command& inst);
		void operator()(const instructions::structure::Pole& inst);
		void operator()(const instructions::texture::BackgroundLoad& inst);
		void operator()(const instructions::texture::BackgroundX& inst);
		void operator()(const instructions::texture::BackgroundAspect& inst);
		void operator()(const instructions::cycle::Ground& inst);
		void operator()(const instructions::cycle::Rail& inst);
		void operator()(const instructions::naked::SignalAnimated& inst);
		void operator()(const instructions::naked::Signal& inst);
		void operator()(const instructions::track::RailStart& inst);
		void operator()(const instructions::track::Rail& inst);
		void operator()(const instructions::track::RailType& inst);
		void operator()(const instructions::track::RailEnd& inst);
		void operator()(const instructions::track::Adhesion& inst);
		void operator()(const instructions::track::Pitch& inst);
		void operator()(const instructions::track::Curve& inst);
		void operator()(const instructions::track::Turn& inst);
		void operator()(const instructions::track::Height& inst);
		void operator()(const instructions::track::FreeObj& inst);
		void operator()(const instructions::track::Wall& inst);
		void operator()(const instructions::track::WallEnd& inst);
		void operator()(const instructions::track::Dike& inst);
		void operator()(const instructions::track::DikeEnd& inst);
		void operator()(const instructions::track::Pole& inst);
		void operator()(const instructions::track::PoleEnd& inst);
		void operator()(const instructions::track::Crack& inst);
		void operator()(const instructions::track::Ground& inst);
		void operator()(const instructions::track::Sta& inst);
		void operator()(const instructions::track::StationXml& inst);
		void operator()(const instructions::track::Stop& inst);
		void operator()(const instructions::track::Form& inst);
		void operator()(const instructions::track::Limit& inst);
		void operator()(const instructions::track::Section& inst);
		void operator()(const instructions::track::SigF& inst);
		void operator()(const instructions::track::Signal& inst);
		void operator()(const instructions::track::Relay& inst);
		void operator()(const instructions::track::Beacon& inst);
		void operator()(const instructions::track::Transponder& inst);
		void operator()(const instructions::track::Pattern& inst);
		void operator()(const instructions::track::Back& inst);
		void operator()(const instructions::track::Fog& inst);
		void operator()(const instructions::track::Brightness& inst);
		void operator()(const instructions::track::Marker& inst);
		void operator()(const instructions::track::MarkerXML& inst);
		void operator()(const instructions::track::TextMarker& inst);
		void operator()(const instructions::track::PointOfInterest& inst);
		void operator()(const instructions::track::PreTrain& inst);
		void operator()(const instructions::track::Announce& inst);
		void operator()(const instructions::track::Doppler& inst);
		void operator()(const instructions::track::Buffer& inst);
	};
} // namespace bve::parsers::csv_rw_route
