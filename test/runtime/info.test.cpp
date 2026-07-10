#include <algorithm>

#include "doctest.h"
#include "music_lyric_model.h"

using namespace music_lyric_model::runtime;
using namespace lyric::common;
using namespace lyric::runtime;

namespace {
	/**
	 * Builds a timed line performed by the sample agent.
	 */
	Line makeAgentLine(uint32_t start, uint32_t end, const std::string& content) {
		WordNormal w;
		w.set_content(content);
		LineNormal body;
		body.mutable_content()->mutable_agent()->set_id("a1");
		*body.mutable_content()->add_words() = makeWordNormal(w);
		Time time;
		time.set_start(start);
		time.set_end(end);
		return makeLineNormal(body, &time);
	}

	/**
	 * Builds sample lyric info for serialization and sorting tests.
	 */
	Info buildInfo() {
		Info info = makeInfo();
		info.add_agents()->set_id("a1");
		info.add_agents()->set_id("a2");
		*info.add_lines() = makeAgentLine(1000, 2000, "hello");
		*info.add_lines() = makeAgentLine(2000, 3000, "world");
		Time interlude;
		interlude.set_start(3000);
		interlude.set_end(4000);
		*info.add_lines() = makeLineInterlude(&interlude);
		return info;
	}
} // namespace

TEST_CASE("runtime proto namespace exports generated types") {
	music_lyric_model::runtime::proto::Info info;
	info.set_type(INFO_TYPE_VALID);
	CHECK(info.type() == INFO_TYPE_VALID);
}

TEST_CASE("makeInfo preserves initial values and stamps the schema version") {
	Info init;
	init.set_version("0.0.1");
	init.set_type(INFO_TYPE_INVALID);
	const Info info = makeInfo(init);
	CHECK(info.version() == music_lyric_model::SCHEMA_VERSION);
	CHECK(info.type() == INFO_TYPE_INVALID);
}

TEST_CASE("sortLinesByTime orders ascending") {
	Info info = buildInfo();
	std::reverse(info.mutable_lines()->begin(), info.mutable_lines()->end());
	sortLinesByTime(info);
	REQUIRE(getLineTime(info.lines(0)) != nullptr);
	CHECK(getLineTime(info.lines(0))->start() == 1000);
	REQUIRE(getLineTime(info.lines(2)) != nullptr);
	CHECK(getLineTime(info.lines(2))->start() == 3000);
}

TEST_CASE("binary round-trip preserves content") {
	const Info info = buildInfo();
	const Info back = decodeInfo(encodeInfo(info));
	CHECK(getLineText(back.lines(0)) == "hello");
	CHECK(back.version() == info.version());
}

TEST_CASE("json round-trip preserves content") {
	const Info info = buildInfo();
	const Info back = infoFromJson(infoToJson(info));
	CHECK(getLineText(back.lines(1)) == "world");
}
