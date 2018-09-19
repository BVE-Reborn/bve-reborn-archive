#include "instruction_generator.hpp"

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route::instruction_generation {
	const std::map<std::string, instruction (*)(const line_splitting::instruction_info& inst)>
	    function_mapping = {
	        ////////////////
	        // CSV ROUTES //
	        ////////////////

	        // Options namespace
	        {"options.unitoflength"s, &create_instruction_options_unitoflength},
	        {"options.unitofspeed"s, &create_instruction_options_unitofspeed},
	        {"options.blocklength"s, &create_instruction_options_blocklength},
	        {"options.objectvisibility"s, &create_instruction_options_objectvisibility},
	        {"options.sectionbehavior"s, &create_instruction_options_sectionbehavior},
	        {"options.cantbehavior"s, &create_instruction_options_cantbehavior},
	        {"options.fogbehavior"s, &create_instruction_options_fogbehavior},
	        {"options.compatibletransparencymode"s,
	         &create_instruction_options_compatibletransparencymode},
	        {"options.enablebvetshacks"s, &create_instruction_options_enablebvetshacks},

	        // Route namespace
	        {"route.comment"s, &create_instruction_route_comment},
	        {"route.image"s, &create_instruction_route_image},
	        {"route.timetable"s, &create_instruction_route_timetable},
	        {"route.change"s, &create_instruction_route_change},
	        {"route.gauge"s, &create_instruction_route_gauge},
	        {"route.signal"s, &create_instruction_route_signal},
	        {"route.runinterval"s, &create_instruction_route_runinterval},
	        {"route.accelerationduetogravity"s, &create_instruction_route_accelerationduetogravity},
	        {"route.elevation"s, &create_instruction_route_elevation},
	        {"route.temperature"s, &create_instruction_route_temperature},
	        {"route.pressure"s, &create_instruction_route_pressure},
	        {"route.displayspeed"s, &create_instruction_route_displayspeed},
	        {"route.loadingscreen"s, &create_instruction_route_loadingscreen},
	        {"route.starttime "s, &create_instruction_route_starttime},
	        {"route.dynamiclight"s, &create_instruction_route_dynamiclight},
	        {"route.ambientlight"s, &create_instruction_route_ambientlight},
	        {"route.directionallight"s, &create_instruction_route_directionallight},
	        {"route.lightdirection"s, &create_instruction_route_lightdirection},

	        // Train namespace
	        {"train.folder"s, &create_instruction_train_folder},
	        {"train.file"s, &create_instruction_train_folder},
	        {"train.run"s, &create_instruction_train_run},
	        {"train.rail"s, &create_instruction_train_run},
	        {"train.flange"s, &create_instruction_train_flange},
	        {"train.timetable"s, &create_instruction_train_timetable},
	        {"train.gauge"s, &create_instruction_route_gauge},
	        {"train.interval"s, &create_instruction_route_runinterval},
	        {"train.velocity"s, &create_instruction_train_velocity},

	        // Structure namespace
	        {"structure.rail"s, &create_instruction_structure_command},
	        {"structure.ground"s, &create_instruction_structure_command},
	        {"structure.walll"s, &create_instruction_structure_command},
	        {"structure.wallr"s, &create_instruction_structure_command},
	        {"structure.dikel"s, &create_instruction_structure_command},
	        {"structure.diker"s, &create_instruction_structure_command},
	        {"structure.forml"s, &create_instruction_structure_command},
	        {"structure.formr"s, &create_instruction_structure_command},
	        {"structure.formcl"s, &create_instruction_structure_command},
	        {"structure.formcr"s, &create_instruction_structure_command},
	        {"structure.roofl"s, &create_instruction_structure_command},
	        {"structure.roofr"s, &create_instruction_structure_command},
	        {"structure.roofcl"s, &create_instruction_structure_command},
	        {"structure.roofcr"s, &create_instruction_structure_command},
	        {"structure.crackl"s, &create_instruction_structure_command},
	        {"structure.crackr"s, &create_instruction_structure_command},
	        {"structure.freeobj"s, &create_instruction_structure_command},
	        {"structure.beacon"s, &create_instruction_structure_command},
	        {"structure.pole"s, &create_instruction_structure_pole},

	        // Texture namespace
	        {"texture.background"s, &create_instruction_texture_background},

	        // Cycle namespace
	        {"cycle.ground"s, &create_instruction_cycle_ground},
	        {"cycle.rail"s, &create_instruction_cycle_rail},

	        // Signal namespace
	        {"signal"s, &create_instruction_signal},

	        // Track namespace

	        // Rails
	        {"track.railstart"s, &create_instruction_track_railstart},
	        {"track.rail"s, &create_instruction_track_rail},
	        {"track.railtype"s, &create_instruction_track_railtype},
	        {"track.railend"s, &create_instruction_track_railend},
	        {"track.accuracy"s, &create_instruction_track_accuracy},
	        {"track.adhesion"s, &create_instruction_track_adhesion},

	        // Geometry
	        {"track.pitch"s, &create_instruction_track_pitch},
	        {"track.curve"s, &create_instruction_track_curve},
	        {"track.turn"s, &create_instruction_track_turn},
	        {"track.height"s, &create_instruction_track_height},

	        // Objects
	        {"track.freeobj"s, &create_instruction_track_freeobj},
	        {"track.wall"s, &create_instruction_track_wall},
	        {"track.wallend"s, &create_instruction_track_wallend},
	        {"track.dike"s, &create_instruction_track_dike},
	        {"track.dikeend"s, &create_instruction_track_dikeend},
	        {"track.pole"s, &create_instruction_track_pole},
	        {"track.poleend"s, &create_instruction_track_poleend},
	        {"track.crack"s, &create_instruction_track_crack},
	        {"track.ground"s, &create_instruction_track_ground},

	        // Stations
	        {"track.sta"s, &create_instruction_track_sta},
	        {"track.stationxml"s, &create_instruction_track_station_xml},
	        {"track.station"s, &create_instruction_track_station},
	        {"track.stop"s, &create_instruction_track_stop},
	        {"track.form"s, &create_instruction_track_form},

	        // Signalling and speed limits
	        {"track.limit"s, &create_instruction_track_limit},
	        {"track.section"s, &create_instruction_track_section},
	        {"track.sigf"s, &create_instruction_track_sigf},
	        {"track.signal"s, &create_instruction_track_signal},
	        {"track.sig"s, &create_instruction_track_signal},
	        {"track.relay"s, &create_instruction_track_relay},

	        // Safety systems
	        {"track.beacon"s, &create_instruction_track_beacon},
	        {"track.transponder"s, &create_instruction_track_transponder},
	        {"track.atssn"s, &create_instruction_track_atssn},
	        {"track.atsp"s, &create_instruction_track_atsp},
	        {"track.pattern"s, &create_instruction_track_pattern},
	        {"track.plimit"s, &create_instruction_track_plimit},

	        // Miscellaneous
	        {"track.back"s, &create_instruction_track_back},
	        {"track.fog"s, &create_instruction_track_fog},
	        {"track.brightness"s, &create_instruction_track_brightness},
	        {"track.marker"s, &create_instruction_track_marker},
	        {"track.textmarker"s, &create_instruction_track_text_marker},
	        {"track.pointofinterest"s, &create_instruction_track_pointofinterest},
	        {"track.pretrain"s, &create_instruction_track_pretrain},
	        {"track.announce"s, &create_instruction_track_announce},
	        {"track.doppler"s, &create_instruction_track_doppler},
	        {"track.buffer"s, &create_instruction_track_buffer},

	        ///////////////
	        // RW Routes //
	        ///////////////

	        // [Options]
	        {"@@options@@unitoflength"s, &create_instruction_options_unitoflength},
	        {"@@options@@unitofspeed"s, &create_instruction_options_unitofspeed},
	        {"@@options@@blocklength"s, &create_instruction_options_blocklength},
	        {"@@options@@objectvisibility"s, &create_instruction_options_objectvisibility},
	        {"@@options@@sectionbehavior"s, &create_instruction_options_sectionbehavior},
	        {"@@options@@cantbehavior"s, &create_instruction_options_cantbehavior},
	        {"@@options@@fogbehavior"s, &create_instruction_options_fogbehavior},

	        // [Route]
	        {"@@route@@comment"s, &create_instruction_route_comment},
	        {"@@route@@image"s, &create_instruction_route_image},
	        {"@@route@@timetable"s, &create_instruction_route_timetable},
	        {"@@route@@change"s, &create_instruction_route_change},
	        {"@@route@@gauge"s, &create_instruction_route_gauge},
	        {"@@route@@signal"s, &create_instruction_route_signal},
	        {"@@route@@runinterval"s, &create_instruction_route_runinterval},
	        {"@@route@@accelerationduetogravity"s,
	         &create_instruction_route_accelerationduetogravity},
	        {"@@route@@elevation"s, &create_instruction_route_elevation},
	        {"@@route@@temperature"s, &create_instruction_route_temperature},
	        {"@@route@@pressure"s, &create_instruction_route_pressure},
	        {"@@route@@ambientlight"s, &create_instruction_route_ambientlight},
	        {"@@route@@directionallight"s, &create_instruction_route_directionallight},
	        {"@@route@@lightdirection"s, &create_instruction_route_lightdirection},

	        // [Train]
	        {"@@train@@folder"s, &create_instruction_train_folder},
	        {"@@train@@file"s, &create_instruction_train_folder},
	        {"@@train@@run"s, &create_instruction_train_run},
	        {"@@train@@rail"s, &create_instruction_train_run},
	        {"@@train@@flange"s, &create_instruction_train_flange},
	        {"@@train@@timetable"s, &create_instruction_train_timetable},
	        {"@@train@@gauge"s, &create_instruction_route_gauge},
	        {"@@train@@interval"s, &create_instruction_route_runinterval},
	        {"@@train@@velocity"s, &create_instruction_train_velocity},

	        // [Object]
	        {"@@object@@rail"s, &create_instruction_structure_command},
	        {"@@object@@beacon"s, &create_instruction_structure_command},
	        {"@@object@@ground"s, &create_instruction_structure_command},
	        {"@@object@@walll"s, &create_instruction_structure_command},
	        {"@@object@@wallr"s, &create_instruction_structure_command},
	        {"@@object@@dikel"s, &create_instruction_structure_command},
	        {"@@object@@diker"s, &create_instruction_structure_command},
	        {"@@object@@forml"s, &create_instruction_structure_command},
	        {"@@object@@formr"s, &create_instruction_structure_command},
	        {"@@object@@formcl"s, &create_instruction_structure_command},
	        {"@@object@@formcr"s, &create_instruction_structure_command},
	        {"@@object@@roofl"s, &create_instruction_structure_command},
	        {"@@object@@roofr"s, &create_instruction_structure_command},
	        {"@@object@@roofcl"s, &create_instruction_structure_command},
	        {"@@object@@roofcr"s, &create_instruction_structure_command},
	        {"@@object@@crackl"s, &create_instruction_structure_command},
	        {"@@object@@crackr"s, &create_instruction_structure_command},
	        {"@@object@@freeobj"s, &create_instruction_structure_command},
	        {"@@object@@pole"s, &create_instruction_structure_pole},
	        {"@@object@@back"s, &create_instruction_texture_background},

	        // [Cycle]
	        {"@@cycle@@groundstructureindex"s, &create_instruction_cycle_ground},

	        // [Signal]
	        {"@@signal@@signalindex"s, &create_instruction_signal},

	        // [Railway]

	        // Rails
	        {"@@railway@@railstart"s, &create_instruction_track_railstart},
	        {"@@railway@@rail"s, &create_instruction_track_rail},
	        {"@@railway@@railtype"s, &create_instruction_track_railtype},
	        {"@@railway@@railend"s, &create_instruction_track_railend},
	        {"@@railway@@accuracy"s, &create_instruction_track_accuracy},
	        {"@@railway@@adhesion"s, &create_instruction_track_adhesion},

	        // Geometry
	        {"@@railway@@pitch"s, &create_instruction_track_pitch},
	        {"@@railway@@curve"s, &create_instruction_track_curve},
	        {"@@railway@@turn"s, &create_instruction_track_turn},
	        {"@@railway@@height"s, &create_instruction_track_height},

	        // Objects
	        {"@@railway@@freeobj"s, &create_instruction_track_freeobj},
	        {"@@railway@@wall"s, &create_instruction_track_wall},
	        {"@@railway@@wallend"s, &create_instruction_track_wallend},
	        {"@@railway@@dike"s, &create_instruction_track_dike},
	        {"@@railway@@dikeend"s, &create_instruction_track_dikeend},
	        {"@@railway@@pole"s, &create_instruction_track_pole},
	        {"@@railway@@poleend"s, &create_instruction_track_poleend},
	        {"@@railway@@crack"s, &create_instruction_track_crack},
	        {"@@railway@@ground"s, &create_instruction_track_ground},

	        // Stations
	        {"@@railway@@sta"s, &create_instruction_track_sta},
	        {"@@railway@@station"s, &create_instruction_track_station},
	        {"@@railway@@stop"s, &create_instruction_track_stop},
	        {"@@railway@@form"s, &create_instruction_track_form},

	        // Signalling and speed limits
	        {"@@railway@@limit"s, &create_instruction_track_limit},
	        {"@@railway@@section"s, &create_instruction_track_section},
	        {"@@railway@@sigf"s, &create_instruction_track_sigf},
	        {"@@railway@@signal"s, &create_instruction_track_signal},
	        {"@@railway@@relay"s, &create_instruction_track_relay},

	        // Safety systems
	        {"@@railway@@beacon"s, &create_instruction_track_beacon},
	        {"@@railway@@transponder"s, &create_instruction_track_transponder},
	        {"@@railway@@atssn"s, &create_instruction_track_atssn},
	        {"@@railway@@atsp"s, &create_instruction_track_atsp},
	        {"@@railway@@pattern"s, &create_instruction_track_pattern},
	        {"@@railway@@plimit"s, &create_instruction_track_plimit},

	        // Miscellaneous
	        {"@@railway@@back"s, &create_instruction_track_back},
	        {"@@railway@@fog"s, &create_instruction_track_fog},
	        {"@@railway@@brightness"s, &create_instruction_track_brightness},
	        {"@@railway@@marker"s, &create_instruction_track_marker},
	        {"@@railway@@pointofinterest"s, &create_instruction_track_pointofinterest},
	        {"@@railway@@pretrain"s, &create_instruction_track_pretrain},
	        {"@@railway@@announce"s, &create_instruction_track_announce},
	        {"@@railway@@doppler"s, &create_instruction_track_doppler},
	        {"@@railway@@buffer"s, &create_instruction_track_buffer},
	};

	const std::map<std::string, instructions::structure::Command::Type> command_type_mapping{
	    {"structure.ground"s, instructions::structure::Command::Type::ground},
	    {"structure.rail"s, instructions::structure::Command::Type::rail},
	    {"structure.walll"s, instructions::structure::Command::Type::wall_l},
	    {"structure.wallr"s, instructions::structure::Command::Type::wall_r},
	    {"structure.dikel"s, instructions::structure::Command::Type::dike_l},
	    {"structure.diker"s, instructions::structure::Command::Type::dike_r},
	    {"structure.forml"s, instructions::structure::Command::Type::form_l},
	    {"structure.formr"s, instructions::structure::Command::Type::form_r},
	    {"structure.formcl"s, instructions::structure::Command::Type::form_cl},
	    {"structure.formcr"s, instructions::structure::Command::Type::form_cr},
	    {"structure.roofl"s, instructions::structure::Command::Type::roof_l},
	    {"structure.roofr"s, instructions::structure::Command::Type::roof_r},
	    {"structure.roofcl"s, instructions::structure::Command::Type::roof_cl},
	    {"structure.roofcr"s, instructions::structure::Command::Type::roof_cr},
	    {"structure.crackl"s, instructions::structure::Command::Type::crack_l},
	    {"structure.crackr"s, instructions::structure::Command::Type::crack_r},
	    {"structure.freeobj"s, instructions::structure::Command::Type::free_obj},
	    {"structure.beacon"s, instructions::structure::Command::Type::beacon},
	    {"@@object@@ground"s, instructions::structure::Command::Type::ground},
	    {"@@object@@rail"s, instructions::structure::Command::Type::rail},
	    {"@@object@@walll"s, instructions::structure::Command::Type::wall_l},
	    {"@@object@@wallr"s, instructions::structure::Command::Type::wall_r},
	    {"@@object@@dikel"s, instructions::structure::Command::Type::dike_l},
	    {"@@object@@diker"s, instructions::structure::Command::Type::dike_r},
	    {"@@object@@forml"s, instructions::structure::Command::Type::form_l},
	    {"@@object@@formr"s, instructions::structure::Command::Type::form_r},
	    {"@@object@@formcl"s, instructions::structure::Command::Type::form_cl},
	    {"@@object@@formcr"s, instructions::structure::Command::Type::form_cr},
	    {"@@object@@roofl"s, instructions::structure::Command::Type::roof_l},
	    {"@@object@@roofr"s, instructions::structure::Command::Type::roof_r},
	    {"@@object@@roofcl"s, instructions::structure::Command::Type::roof_cl},
	    {"@@object@@roofcr"s, instructions::structure::Command::Type::roof_cr},
	    {"@@object@@crackl"s, instructions::structure::Command::Type::crack_l},
	    {"@@object@@crackr"s, instructions::structure::Command::Type::crack_r},
	    {"@@object@@freeobj"s, instructions::structure::Command::Type::free_obj},
	    {"@@object@@beacon"s, instructions::structure::Command::Type::beacon},
	};
} // namespace bve::parsers::csv_rw_route::instruction_generation
