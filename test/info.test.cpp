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
		body.mutable_time()->set_start(start);
		body.mutable_time()->set_end(end);
		body.mutable_agent()->set_id("a1");
		*body.add_words() = makeWordNormal(w);
		return makeLineNormal(body);
	}

	Info buildInfo() {
		Info info = makeInfo();
		info.add_agents()->set_id("a1");
		info.add_agents()->set_id("a2");
		*info.add_lines() = makeAgentLine(1000, 2000, "hello");
		*info.add_lines() = makeAgentLine(2000, 3000, "world");
		LineInterlude interlude;
		interlude.mutable_time()->set_start(3000);
		interlude.mutable_time()->set_end(4000);
		*info.add_lines() = makeLineInterlude(interlude);
		return info;
	}
} // namespace

TEST_CASE("makeInfo always stamps the schema version") {
	CHECK(makeInfo().version() == SCHEMA_VERSION);
}

TEST_CASE("calcAgentIndex fills index and count snapshots") {
	Info info = buildInfo();
	calcAgentIndex(info);
	REQUIRE(isLineNormal(info.lines(0)));
	REQUIRE(isLineNormal(info.lines(1)));
	CHECK(info.lines(0).normal().agent().global_index() == 0);
	CHECK(info.lines(1).normal().agent().global_index() == 1);
	CHECK(info.lines(1).normal().agent().block_index() == 1);
	REQUIRE(getAgentById(info.agents(), "a1") != nullptr);
	CHECK(getAgentById(info.agents(), "a1")->count() == 2);
	REQUIRE(getAgentById(info.agents(), "a2") != nullptr);
	CHECK(getAgentById(info.agents(), "a2")->count() == 0);
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
