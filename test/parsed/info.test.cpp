#include <algorithm>
#include <stdexcept>
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
	Info buildInfo() {
		Info      init;
		AgentItem a1;
		a1.id = "a1";
		AgentItem a2;
		a2.id       = "a2";
		init.agents = {a1, a2};

		LineNormal first;
		first.time   = Time{1000, 2000};
		first.agents = {"a1"};
		WordNormal hello;
		hello.content = "hello";
		first.words   = {makeWordNormal(hello)};
		init.lines.push_back(makeParsedLineNormal(first));

		LineNormal second;
		second.time   = Time{2000, 3000};
		second.agents = {"a1"};
		WordNormal world;
		world.content = "world";
		second.words  = {makeWordNormal(world)};
		init.lines.push_back(makeParsedLineNormal(second));

		LineInterlude interlude;
		interlude.time = Time{3000, 4000};
		init.lines.push_back(makeParsedLineInterlude(interlude));

		return makeParsedInfo(init);
	}
} // namespace

TEST_CASE("makeParsedInfo stamps the schema version") {
	CHECK(makeParsedInfo().version == SCHEMA_VERSION);
	Info init;
	init.version = "0.0.1";
	CHECK(makeParsedInfo(init).version == SCHEMA_VERSION);
}

TEST_CASE("isParsedInfoValid and isParsedInfoInvalid read InfoType") {
	Info valid;
	valid.type = InfoType::Valid;
	CHECK(isParsedInfoValid(makeParsedInfo(valid)) == true);
	CHECK(isParsedInfoInvalid(makeParsedInfo(valid)) == false);

	Info invalid;
	invalid.type = InfoType::Invalid;
	CHECK(isParsedInfoInvalid(makeParsedInfo(invalid)) == true);
	CHECK(isParsedInfoValid(makeParsedInfo(invalid)) == false);

	CHECK(isParsedInfoValid(makeParsedInfo()) == false);
	CHECK(isParsedInfoInvalid(makeParsedInfo()) == false);
}

TEST_CASE("line guards and plain text read the body") {
	const Info info = buildInfo();
	CHECK(isParsedLineNormal(info.lines[0]) == true);
	CHECK(isParsedLineInterlude(info.lines[2]) == true);
	CHECK(getParsedLineText(info.lines[0]) == "hello");
}

TEST_CASE("agent line counts and primary agent over normal bodies") {
	const Info              info = buildInfo();
	std::vector<LineNormal> normals;
	for (const auto& line : info.lines) {
		if (const LineNormal* normal = asParsedLineNormal(line)) {
			normals.push_back(*normal);
		}
	}
	const auto counts = getAgentLineCounts(normals);
	CHECK(counts.at("a1") == 2);
	CHECK(getPrimaryAgent(info.agents, normals)->id == "a1");
}

TEST_CASE("sortParsedLinesByTime orders ascending including backgrounds") {
	Info info = buildInfo();

	LineNormal body;
	body.time = Time{500, 900};
	LineBackground late;
	late.time = Time{700, 800};
	LineBackground early;
	early.time = Time{600, 650};
	body.backgrounds.push_back(late);
	body.backgrounds.push_back(early);
	info.lines.push_back(makeParsedLineNormal(body));

	std::reverse(info.lines.begin(), info.lines.end());
	sortParsedLinesByTime(info);
	CHECK(getParsedLineTime(info.lines[0])->start == 500);
	const LineNormal* sorted = asParsedLineNormal(info.lines[0]);
	REQUIRE(sorted != nullptr);
	REQUIRE(sorted->backgrounds.size() == 2);
	CHECK(sorted->backgrounds[0].time->start == 600);
	CHECK(sorted->backgrounds[1].time->start == 700);
}

TEST_CASE("binary and json round-trip preserve content") {
	const Info info       = buildInfo();
	const Info fromBinary = decodeParsedInfo(encodeParsedInfo(info));
	CHECK(getParsedLineText(fromBinary.lines[0]) == "hello");
	const Info fromJson = parsedInfoFromJson(parsedInfoToJson(info));
	CHECK(getParsedLineText(fromJson.lines[1]) == "world");
}

TEST_CASE("decodeParsedInfo throws on invalid binary and json") {
	const std::vector<uint8_t> badBinary = {0x01, 0x02, 0x03};
	CHECK_THROWS_AS(decodeParsedInfo(badBinary), std::runtime_error);
	CHECK_THROWS_AS(parsedInfoFromJson("{not-json"), std::runtime_error);
}
