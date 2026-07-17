#include <algorithm>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model;
using namespace music_lyric_model::common;
using namespace music_lyric_model::parsed;

namespace {
	/**
	 * Build an Info with two timed lines and a trailing interlude.
	 */
	lyric::parsed::Info buildInfo() {
		lyric::parsed::Info init;

		lyric::common::AgentItem a1;
		a1.set_id("a1");
		*init.add_agents() = makeAgentItem(a1);

		lyric::common::AgentItem a2;
		a2.set_id("a2");
		*init.add_agents() = makeAgentItem(a2);

		lyric::parsed::LineNormal first;
		first.mutable_time()->set_start(1000);
		first.mutable_time()->set_end(2000);
		first.add_agents("a1");
		lyric::common::WordNormal hello;
		hello.set_content("hello");
		*first.add_words() = makeWordNormal(hello);
		*init.add_lines() = makeParsedLineNormal(first);

		lyric::parsed::LineNormal second;
		second.mutable_time()->set_start(2000);
		second.mutable_time()->set_end(3000);
		second.add_agents("a1");
		lyric::common::WordNormal world;
		world.set_content("world");
		*second.add_words() = makeWordNormal(world);
		*init.add_lines() = makeParsedLineNormal(second);

		lyric::parsed::LineInterlude interlude;
		interlude.mutable_time()->set_start(3000);
		interlude.mutable_time()->set_end(4000);
		*init.add_lines() = makeParsedLineInterlude(interlude);

		return makeParsedInfo(init);
	}
} // namespace

TEST_CASE("makeParsedInfo stamps the schema version") {
	CHECK(std::string(makeParsedInfo().version()) == SCHEMA_VERSION);
	lyric::parsed::Info init;
	init.set_version("0.0.1");
	CHECK(std::string(makeParsedInfo(init).version()) == SCHEMA_VERSION);
}

TEST_CASE("isParsedInfoValid and isParsedInfoInvalid read InfoType") {
	lyric::parsed::Info valid;
	valid.set_type(lyric::parsed::INFO_TYPE_VALID);
	CHECK(isParsedInfoValid(makeParsedInfo(valid)) == true);
	CHECK(isParsedInfoInvalid(makeParsedInfo(valid)) == false);

	lyric::parsed::Info invalid;
	invalid.set_type(lyric::parsed::INFO_TYPE_INVALID);
	CHECK(isParsedInfoInvalid(makeParsedInfo(invalid)) == true);
	CHECK(isParsedInfoValid(makeParsedInfo(invalid)) == false);

	CHECK(isParsedInfoValid(makeParsedInfo()) == false);
	CHECK(isParsedInfoInvalid(makeParsedInfo()) == false);
}

TEST_CASE("line guards and plain text read the body") {
	const lyric::parsed::Info info = buildInfo();
	CHECK(isParsedLineNormal(info.lines(0)) == true);
	CHECK(isParsedLineInterlude(info.lines(2)) == true);
	CHECK(getParsedLineText(info.lines(0)) == "hello");
}

TEST_CASE("agent line counts and primary agent over normal bodies") {
	const lyric::parsed::Info info = buildInfo();
	std::vector<lyric::parsed::LineNormal> normals;
	for (const auto& line : info.lines()) {
		if (const lyric::parsed::LineNormal* normal = asParsedLineNormal(line)) {
			normals.push_back(*normal);
		}
	}
	const auto counts = getAgentLineCounts(normals);
	CHECK(counts.at("a1") == 2);
	CHECK(getPrimaryAgent(info.agents(), normals)->id() == "a1");
}

TEST_CASE("sortParsedLinesByTime orders ascending") {
	lyric::parsed::Info info = buildInfo();
	std::reverse(info.mutable_lines()->begin(), info.mutable_lines()->end());
	sortParsedLinesByTime(info);
	CHECK(getParsedLineTime(info.lines(0))->start() == 1000);
}

TEST_CASE("binary and json round-trip preserve content") {
	const lyric::parsed::Info info      = buildInfo();
	const lyric::parsed::Info fromBinary = decodeParsedInfo(encodeParsedInfo(info));
	CHECK(getParsedLineText(fromBinary.lines(0)) == "hello");
	const lyric::parsed::Info fromJson = parsedInfoFromJson(parsedInfoToJson(info));
	CHECK(getParsedLineText(fromJson.lines(1)) == "world");
}
