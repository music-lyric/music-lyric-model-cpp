#include "info.h"

#include <algorithm>

#include "agent/agent.h"
#include "doctest.h"
#include "line/content.h"
#include "version.h"
#include "word/content.h"

using namespace music_lyric_model;
using namespace lyric;

namespace {
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

TEST_CASE("makeInfo always stamps the schema version") {
	CHECK(makeInfo().version() == SCHEMA_VERSION);
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
