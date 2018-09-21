#include "executor_pass3.hpp"
#include "utils.hpp"
#include <string>

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route {
	using TextureVector = std::vector<xml::dynamic_background::TextureBackgroundInfo>;
	using xml::dynamic_background::ObjectBackgroundInfo;
	using xml::dynamic_background::ParsedDynamicBackground;
	using xml::dynamic_background::TextureBackgroundInfo;

	//////////////////////////////
	// Background.Load Subcalls //
	//////////////////////////////

	void Pass3Executor::backgroundLoadXML(const instructions::texture::BackgroundLoad& inst) {
		auto const issuer_filename = getFilename(inst.file_index);

		ParsedDynamicBackground background;

		auto const filename = get_relative_file_(issuer_filename, inst.filename);

		try {
			auto file_contents = util::load_from_file_utf8_bom(filename);
			background = xml::dynamic_background::parse(filename, std::move(file_contents), errors_,
			                                            get_relative_file_);
		}
		catch (const std::exception& e) {
			add_error(errors_, issuer_filename, inst.line, e.what());
		}

		auto insert_pair = std::make_pair(inst.background_texture_index, std::move(background));

		auto const insert_return = background_mapping_.insert(insert_pair);

		auto iterator = insert_return.first;
		auto const inserted = insert_return.second;

		// Something defined this first
		if (!inserted) {
			// actually preform the insertion, not using the background variable as
			// it has been moved
			iterator->second = std::move(insert_pair.second);
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) is overwriting all prior calls the Texture Background functions"s);
		}
	}

	void Pass3Executor::backgroundLoadImage(const instructions::texture::BackgroundLoad& inst) {
		auto const issuer_filename = getFilename(inst.file_index);

		auto found_iter = background_mapping_.find(inst.background_texture_index);

		if (found_iter == background_mapping_.end()) {
			TextureBackgroundInfo tbi;
			tbi.from_xml = false;
			tbi.filename = inst.filename;
			background_mapping_.insert(
			    {inst.background_texture_index, TextureVector{std::move(tbi)}});
			return;
		}

		auto& background = found_iter->second;

		auto const created_by_xml = background.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const TextureVector& v) -> bool { return v[0].from_xml; },
		    [](const ObjectBackgroundInfo&) -> bool { return true; });

		if (created_by_xml) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) has already been used, ignoring Texture.Background(Image)"s);
			return;
		}

		background.get_unchecked<TextureVector>()[0].filename = inst.filename;
	}

	/////////////////////////
	// Direct Instructions //
	/////////////////////////

	void Pass3Executor::operator()(const instructions::texture::BackgroundLoad& inst) {
		auto issuer_filename = getFilename(inst.file_index);

		// Hacky way to find the file extant of the file in order to determine
		// if XML. Works due to OpenBVE's strictness on filenames
		auto const period_index = inst.filename.find_last_of('.');

		// If there is no period, assuming non xml file
		auto is_xml = false;
		if (period_index != std::string::npos) {
			auto extant = inst.filename.substr(period_index + 1);
			util::lower(extant);

			is_xml = extant == "xml"s;
		}

		if (is_xml) {
			backgroundLoadXML(inst);
		}
		else {
			backgroundLoadImage(inst);
		}
	}

	void Pass3Executor::operator()(const instructions::texture::BackgroundX& inst) {
		auto const issuer_filename = getFilename(inst.file_index);

		auto found_iter = background_mapping_.find(inst.background_texture_index);

		if (found_iter == background_mapping_.end()) {
			TextureBackgroundInfo tbi;
			tbi.from_xml = false;
			tbi.repetitions = inst.repetition_count;
			background_mapping_.insert(
			    {inst.background_texture_index, TextureVector{std::move(tbi)}});
			return;
		}

		auto& background = found_iter->second;

		auto const created_by_xml = background.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const TextureVector& v) -> bool { return v[0].from_xml; },
		    [](const ObjectBackgroundInfo&) -> bool { return true; });

		if (created_by_xml) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) has already been used, ignoring Texture.Background.X"s);
			return;
		}

		background.get_unchecked<TextureVector>()[0].repetitions = inst.repetition_count;
	}

	void Pass3Executor::operator()(const instructions::texture::BackgroundAspect& inst) {
		auto const issuer_filename = getFilename(inst.file_index);

		auto found_iter = background_mapping_.find(inst.background_texture_index);

		if (found_iter == background_mapping_.end()) {
			TextureBackgroundInfo tbi;
			tbi.from_xml = false;
			tbi.preserve_aspect =
			    inst.mode == instructions::texture::BackgroundAspect::Mode::aspect;
			background_mapping_.insert(
			    {inst.background_texture_index, TextureVector{std::move(tbi)}});
			return;
		}

		auto& background = found_iter->second;

		auto const created_by_xml = background.match(
		    // vector will always have at least one element
		    // it may have more, but it will definitely be from XML
		    [](const TextureVector& v) -> bool { return v[0].from_xml; },
		    [](const ObjectBackgroundInfo&) -> bool { return true; });

		if (created_by_xml) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Texture.Background(XML) has already been used, ignoring Texture.Background.Aspect"s);
			return;
		}

		background.get_unchecked<TextureVector>()[0].preserve_aspect =
		    inst.mode == instructions::texture::BackgroundAspect::Mode::aspect;
	}
} // namespace bve::parsers::csv_rw_route
