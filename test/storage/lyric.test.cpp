#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model;
using namespace music_lyric_model::common;
using namespace music_lyric_model::storage;

namespace {
	/**
	 * Build a Lyric with two lines referencing agents by id.
	 */
	Lyric buildLyric() {
		Lyric     init;
		AgentItem a1;
		a1.id    = "a1";
		a1.names = {"Alice"};
		AgentItem a2;
		a2.id       = "a2";
		a2.names    = {"Bob"};
		init.agents = {a1, a2};

		Line first;
		first.time   = Time{1000, 2000};
		first.agents = {"a1", "a2"};
		WordNormal hello;
		hello.content  = "hello";
		hello.language = "en";
		first.words    = {makeWordNormal(hello)};
		init.lines.push_back(first);

		Line second;
		second.time   = Time{2000, 3000};
		second.agents = {"a1"};
		WordNormal world;
		world.content = "world";
		second.words  = {makeWordNormal(world)};
		init.lines.push_back(second);

		return makeStorageLyric(init);
	}
} // namespace

TEST_CASE("makeStorageLyric stamps the schema version") {
	CHECK(makeStorageLyric().version == SCHEMA_VERSION);
	Lyric init;
	init.version = "0.0.1";
	CHECK(makeStorageLyric(init).version == SCHEMA_VERSION);
}

TEST_CASE("line time, plain text and multi-agent resolution") {
	const Lyric lyric = buildLyric();
	const Line& first = lyric.lines[0];
	CHECK(getStorageLineTime(first)->start == 1000);
	CHECK(getStorageLineDuration(first) == 1000);
	CHECK(getStorageLineText(first) == "hello");
	CHECK(getStorageLineLanguages(first) == std::vector<std::string>{"en"});
	const auto agents = resolveLineAgents(lyric.agents, first);
	REQUIRE(agents.size() == 2);
	CHECK(agents[0]->id == "a1");
	CHECK(agents[1]->id == "a2");
}

TEST_CASE("agent line counts and primary agent") {
	const Lyric lyric  = buildLyric();
	const auto  counts = getAgentLineCounts(lyric.lines);
	CHECK(counts.at("a1") == 2);
	CHECK(counts.at("a2") == 1);
	CHECK(getPrimaryAgent(lyric.agents, lyric.lines)->id == "a1");
}

TEST_CASE("binary round-trip preserves content") {
	const Lyric lyric      = buildLyric();
	const Lyric fromBinary = decodeStorageLyric(encodeStorageLyric(lyric));
	CHECK(getStorageLineText(fromBinary.lines[0]) == "hello");
	CHECK(getStorageLineText(fromBinary.lines[1]) == "world");
}

#if MUSIC_LYRIC_MODEL_ENABLE_JSON
TEST_CASE("json round-trip preserves content") {
	const Lyric lyric    = buildLyric();
	const Lyric fromJson = storageLyricFromJson(storageLyricToJson(lyric));
	CHECK(getStorageLineText(fromJson.lines[0]) == "hello");
	CHECK(getStorageLineText(fromJson.lines[1]) == "world");
}
#endif

TEST_CASE("sortStorageLinesByTime orders ascending including backgrounds") {
	Lyric lyric = buildLyric();

	Line late;
	late.time = Time{4000, 5000};
	LineBackground bgLate;
	bgLate.time = Time{4500, 4600};
	LineBackground bgEarly;
	bgEarly.time = Time{4100, 4200};
	late.backgrounds.push_back(bgLate);
	late.backgrounds.push_back(bgEarly);
	lyric.lines.push_back(late);

	std::reverse(lyric.lines.begin(), lyric.lines.end());
	sortStorageLinesByTime(lyric);
	CHECK(getStorageLineTime(lyric.lines[0])->start == 1000);
	CHECK(getStorageLineTime(lyric.lines[2])->start == 4000);
	REQUIRE(lyric.lines[2].backgrounds.size() == 2);
	CHECK(lyric.lines[2].backgrounds[0].time->start == 4100);
	CHECK(lyric.lines[2].backgrounds[1].time->start == 4500);
}

TEST_CASE("getStorageActiveLine picks the timed line covering the moment") {
	const Lyric lyric = buildLyric();
	CHECK(getStorageActiveLineIndex(lyric.lines, 1500) == 0);
	CHECK(getStorageActiveLine(lyric.lines, 2500) == &lyric.lines[1]);
	CHECK(getStorageActiveLineIndex(lyric.lines, 50) == -1);
	CHECK(getStorageActiveLine(lyric.lines, 50) == nullptr);
}

TEST_CASE("getStorageLineTranslation and getStorageLineRoman read line annotations") {
	LineAnnotation            annotation;
	LineAnnotationTranslation translation;
	translation.language = "en";
	translation.content  = "hello";
	annotation.translations.push_back(translation);
	LineAnnotationRoman roman;
	roman.language = "romaji";
	roman.content  = "konnichiwa";
	annotation.romans.push_back(roman);

	Line line;
	line.annotation = annotation;
	CHECK(*getStorageLineTranslation(line, std::string("en")) == "hello");
	CHECK(*getStorageLineRoman(line, std::string("romaji")) == "konnichiwa");
}

TEST_CASE("decodeStorageLyric throws on invalid binary") {
	const std::vector<uint8_t> badBinary = {0x01, 0x02, 0x03};
	CHECK_THROWS_AS(decodeStorageLyric(badBinary), std::runtime_error);
}

#if MUSIC_LYRIC_MODEL_ENABLE_JSON
TEST_CASE("storageLyricFromJson throws on invalid json") {
	CHECK_THROWS_AS(storageLyricFromJson("{not-json"), std::runtime_error);
}
#endif
