#include "../executor_pass3.hpp"
#include "utils.hpp"
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	using texture_vector = std::vector<xml::dynamic_backgrounds::texture_background_info>;
	using xml::dynamic_backgrounds::object_background_info;
	using xml::dynamic_backgrounds::parsed_dynamic_background;
	using xml::dynamic_backgrounds::texture_background_info;

	//////////////////////////////
	// Background.Load Subcalls //
	//////////////////////////////

	void pass3_executor::background_load_xml(const instructions::texture::Background_Load& instr) {
		auto issuer_filename = get_filename(instr.file_index);

		parsed_dynamic_background bround;

		auto filename = _get_relative_file(issuer_filename, instr.filename);

		try {
			bround = xml::dynamic_backgrounds::parse(filename, _errors, _get_relative_file);
		}
		catch (const std::exception& e) {
			_errors[issuer_filename].emplace_back<errors::error_t>({instr.line, e.what()});
		}

		auto insert_pair = std::make_pair(instr.background_texture_index, std::move(bround));

		auto insert_return = background_mapping.insert(insert_pair);

		auto iterator = insert_return.first;
		auto inserted = insert_return.second;

		// Something defined this first
		if (!inserted) {
			// actually preform the insertion, not using the bround variable as it has been moved
			iterator->second = std::move(insert_pair.second);
			_errors[issuer_filename].emplace_back<errors::error_t>(
			    {instr.line,
			     "Texture.Background(XML) is overwriting all prior calls the Texture Background functions"s});
		}
	}

	void pass3_executor::background_load_image(const instructions::texture::Background_Load& instr) {
		auto issuer_filename = get_filename(instr.file_index);

		auto found_iter = background_mapping.find(instr.background_texture_index);

		if (found_iter == background_mapping.end()) {
			texture_background_info tbi;
			tbi.fromXML = false;
			tbi.filename = instr.filename;
			background_mapping.insert({instr.background_texture_index, texture_vector{std::move(tbi)}});
			return;
		}

		parsed_dynamic_background& bround = found_iter->second;

		bool created_by_xml = bround.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const texture_vector& v) -> bool { return v[0].fromXML; },
		    [](const xml::dynamic_backgrounds::object_background_info&) -> bool { return true; });

		if (created_by_xml) {
			_errors[issuer_filename].emplace_back<errors::error_t>(
			    {instr.line, "Texture.Background(XML) has already been used, ignoring Texture.Background(Image)"s});
			return;
		}

		bround.get_unchecked<texture_vector>()[0].filename = instr.filename;
	}

	/////////////////////////
	// Direct Instructions //
	/////////////////////////

	void pass3_executor::operator()(const instructions::texture::Background_Load& instr) {
		auto issuer_filename = get_filename(instr.file_index);

		// Hacky way to find the file extant of the file in order to determine if XML. Works due to OpenBVE1's
		// strictness on filenames
		auto period_index = instr.filename.find_last_of('.');

		// If there is no period, assuming non xml file
		bool is_XML = false;
		if (period_index != std::string::npos) {
			auto extant = instr.filename.substr(period_index + 1);
			util::lower(extant);

			is_XML = extant == "xml"s;
		}

		if (is_XML) {
			background_load_xml(instr);
		}
		else {
			background_load_image(instr);
		}
	}

	void pass3_executor::operator()(const instructions::texture::Background_X& instr) {
		auto issuer_filename = get_filename(instr.file_index);

		auto found_iter = background_mapping.find(instr.background_texture_index);

		if (found_iter == background_mapping.end()) {
			texture_background_info tbi;
			tbi.fromXML = false;
			tbi.repetitions = instr.repetition_count;
			background_mapping.insert({instr.background_texture_index, texture_vector{std::move(tbi)}});
			return;
		}

		parsed_dynamic_background& bround = found_iter->second;

		bool created_by_xml = bround.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const texture_vector& v) -> bool { return v[0].fromXML; },
		    [](const xml::dynamic_backgrounds::object_background_info&) -> bool { return true; });

		if (created_by_xml) {
			_errors[issuer_filename].emplace_back<errors::error_t>(
			    {instr.line, "Texture.Background(XML) has already been used, ignoring Texture.Background.X"s});
			return;
		}

		bround.get_unchecked<texture_vector>()[0].repetitions = instr.repetition_count;
	}

	void pass3_executor::operator()(const instructions::texture::Background_Aspect& instr) {
		auto issuer_filename = get_filename(instr.file_index);

		auto found_iter = background_mapping.find(instr.background_texture_index);

		if (found_iter == background_mapping.end()) {
			texture_background_info tbi;
			tbi.fromXML = false;
			tbi.preserve_aspect = instr.mode == instructions::texture::Background_Aspect::Aspect;
			background_mapping.insert({instr.background_texture_index, texture_vector{std::move(tbi)}});
			return;
		}

		parsed_dynamic_background& bround = found_iter->second;

		bool created_by_xml = bround.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const texture_vector& v) -> bool { return v[0].fromXML; },
		    [](const xml::dynamic_backgrounds::object_background_info&) -> bool { return true; });

		if (created_by_xml) {
			_errors[issuer_filename].emplace_back<errors::error_t>(
			    {instr.line, "Texture.Background(XML) has already been used, ignoring Texture.Background.Aspect"s});
			return;
		}

		bround.get_unchecked<texture_vector>()[0].preserve_aspect =
		    instr.mode == instructions::texture::Background_Aspect::Aspect;
	}
} // namespace csv_rw_route
} // namespace parsers
