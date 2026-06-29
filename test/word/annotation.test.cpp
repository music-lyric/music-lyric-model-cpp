#include "word/annotation.h"

#include "doctest.h"

using namespace music_lyric_model;
using namespace lyric;

TEST_CASE("getWordAnnotationText joins ruby tokens") {
	WordAnnotationRuby ruby;
	ruby.add_words()->set_content("か");
	ruby.add_words()->set_content("ん");
	CHECK(getWordAnnotationText(ruby) == "かん");
}

TEST_CASE("getWordAnnotationText joins roman tokens") {
	WordAnnotationRoman roman;
	roman.add_words()->set_content("ni");
	roman.add_words()->set_content("hao");
	CHECK(getWordAnnotationText(roman) == "nihao");
}
