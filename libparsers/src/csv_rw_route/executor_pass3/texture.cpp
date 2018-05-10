#include "executor_pass3.hpp"
#include "utils.hpp"
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	using texture_vector = std::vector<xml::dynamic_background::texture_background_info>;
	using xml::dynamic_background::object_background_info;
	using xml::dynamic_background::parsed_dynamic_background;
	using xml::dynamic_background::texture_background_info;

	//////////////////////////////
	// Background.Load Subcalls //
	//////////////////////////////

	void pass3_executor::background_load_xml(const instructions::texture::background_load& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		parsed_dynamic_background bround;

		auto const filename = get_relative_file_(issuer_filename, inst.filename);

		try {
			auto file_contents = util::load_from_file_utf8_bom(filename);
			bround = xml::dynamic_background::parse(filename, std::move(file_contents), errors_,
			                                        get_relative_file_);
		}
		catch (const std::exception& e) {
			add_error(errors_, issuer_filename, inst.line, e.what());
		}

		auto insert_pair = std::make_pair(inst.background_texture_index, std::move(bround));

		auto const insert_return = background_mapping_.insert(insert_pair);

		auto iterator = insert_return.first;
		auto const inserted = insert_return.second;

		// Something defined this first
		if (!inserted) {
			// actually preform the insertion, not using the bround variable as
			// it has been moved
			iterator->second = std::move(insert_pair.second);
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) is overwriting all prior calls the Texture Background functions"s);
		}
	}

	void pass3_executor::background_load_image(const instructions::texture::background_load& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto found_iter = background_mapping_.find(inst.background_texture_index);

		if (found_iter == background_mapping_.end()) {
			texture_background_info tbi;
			tbi.from_xml = false;
			tbi.filename = inst.filename;
			background_mapping_.insert(
			    {inst.background_texture_index, texture_vector{std::move(tbi)}});
			return;
		}

		auto& bround = found_iter->second;

		auto const created_by_xml = bround.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const texture_vector& v) -> bool { return v[0].from_xml; },
		    [](const object_background_info&) -> bool { return true; });

		if (created_by_xml) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) has already been used, ignoring Texture.Background(Image)"s);
			return;
		}

		bround.get_unchecked<texture_vector>()[0].filename = inst.filename;
	}

	/////////////////////////
	// Direct Instructions //
	/////////////////////////

	void pass3_executor::operator()(const instructions::texture::background_load& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		// Hacky way to find the file extant of the file in order to determine
		// if XML. Works due to OpenBVE1's strictness on filenames
		auto const period_index = inst.filename.find_last_of('.');

		// If there is no period, assuming non xml file
		auto is_xml = false;
		if (period_index != std::string::npos) {
			auto extant = inst.filename.substr(period_index + 1);
			util::lower(extant);

			is_xml = extant == "xml"s;
		}

		if (is_xml) {
			background_load_xml(inst);
		}
		else {
			background_load_image(inst);
		}
	}

	void pass3_executor::operator()(const instructions::texture::background_x& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto found_iter = background_mapping_.find(inst.background_texture_index);

		if (found_iter == background_mapping_.end()) {
			texture_background_info tbi;
			tbi.from_xml = false;
			tbi.repetitions = inst.repetition_count;
			background_mapping_.insert(
			    {inst.background_texture_index, texture_vector{std::move(tbi)}});
			return;
		}

		auto& bround = found_iter->second;

		auto const created_by_xml = bround.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const texture_vector& v) -> bool { return v[0].from_xml; },
		    [](const object_background_info&) -> bool { return true; });

		if (created_by_xml) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) has already been used, ignoring Texture.Background.X"s);
			return;
		}

		bround.get_unchecked<texture_vector>()[0].repetitions = inst.repetition_count;
	}

	void pass3_executor::operator()(const instructions::texture::background_aspect& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		auto found_iter = background_mapping_.find(inst.background_texture_index);

		if (found_iter == background_mapping_.end()) {
			texture_background_info tbi;
			tbi.from_xml = false;
			tbi.preserve_aspect = inst.mode == instructions::texture::background_aspect::aspect;
			background_mapping_.insert(
			    {inst.background_texture_index, texture_vector{std::move(tbi)}});
			return;
		}

		auto& bround = found_iter->second;

		auto const created_by_xml = bround.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const texture_vector& v) -> bool { return v[0].from_xml; },
		    [](const object_background_info&) -> bool { return true; });

		if (created_by_xml) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) has already been used, ignoring Texture.Background.Aspect"s);
			return;
		}

		bround.get_unchecked<texture_vector>()[0].preserve_aspect =
		    inst.mode == instructions::texture::background_aspect::aspect;
	}
} // namespace csv_rw_route
} // namespace parsers
