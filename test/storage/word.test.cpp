#include "doctest.h"
#include "music_lyric_model.h"

using namespace lyric::storage;
using google::protobuf::RepeatedPtrField;

TEST_CASE("storage word makers preserve initial values") {
	WordAnnotationContent content;
	content.set_content("ni");
	CHECK(music_lyric_model::storage::makeWordAnnotationContent(content).content() == "ni");
	WordAnnotationRoman roman;
	*roman.add_words() = content;
	CHECK(music_lyric_model::storage::makeWordAnnotationRoman(roman).words(0).content() == "ni");
	WordAnnotationTranslate translate;
	translate.set_content("hello");
	CHECK(music_lyric_model::storage::makeWordAnnotationTranslate(translate).content() == "hello");
	CHECK(music_lyric_model::storage::makeWordAnnotationRuby().words().empty());
	CHECK(music_lyric_model::storage::makeWordAnnotationUnknown().key().empty());
	CHECK(music_lyric_model::storage::makeWordAnnotation().unknowns().empty());
}

TEST_CASE("storage word variants unwrap safely") {
	WordNormal normalInit;
	normalInit.set_content("hi");
	const Word normal = music_lyric_model::storage::makeWordNormal(normalInit);
	WordSpace  spaceInit;
	spaceInit.set_count(2);
	const Word space = music_lyric_model::storage::makeWordSpace(spaceInit);
	REQUIRE(music_lyric_model::storage::asWordNormal(normal) != nullptr);
	CHECK(music_lyric_model::storage::asWordNormal(normal)->content() == "hi");
	CHECK(music_lyric_model::storage::asWordNormal(space) == nullptr);
	REQUIRE(music_lyric_model::storage::asWordSpace(space) != nullptr);
	CHECK(music_lyric_model::storage::asWordSpace(space)->count() == 2);
	CHECK(music_lyric_model::storage::asWordSpace(normal) == nullptr);
	CHECK(music_lyric_model::storage::isWordNormal(normal) == true);
	CHECK(music_lyric_model::storage::isWordSpace(space) == true);
}

TEST_CASE("storage word text renders content and spaces") {
	WordNormal normal;
	normal.set_content("hi");
	WordSpace space;
	space.set_count(3);
	CHECK(music_lyric_model::storage::getWordText(music_lyric_model::storage::makeWordNormal(normal)) == "hi");
	CHECK(music_lyric_model::storage::getWordText(music_lyric_model::storage::makeWordSpace(space)) == "   ");
}

TEST_CASE("storage word time and duration accept a wrapper or bare normal word") {
	WordNormal init;
	init.set_content("hi");
	init.mutable_time()->set_start(1000);
	init.mutable_time()->set_end(1500);
	const Word        word   = music_lyric_model::storage::makeWordNormal(init);
	const WordNormal* normal = music_lyric_model::storage::asWordNormal(word);
	REQUIRE(normal != nullptr);
	REQUIRE(music_lyric_model::storage::getWordTime(word) != nullptr);
	CHECK(music_lyric_model::storage::getWordTime(word)->start() == 1000);
	REQUIRE(music_lyric_model::storage::getWordTime(*normal) != nullptr);
	CHECK(music_lyric_model::storage::getWordTime(*normal)->start() == 1000);
	CHECK(music_lyric_model::storage::getWordDuration(word) == 500);
	CHECK(music_lyric_model::storage::getWordDuration(*normal) == 500);
	WordSpace space;
	space.set_count(1);
	CHECK(music_lyric_model::storage::getWordTime(music_lyric_model::storage::makeWordSpace(space)) == nullptr);
	CHECK(music_lyric_model::storage::getWordDuration(music_lyric_model::storage::makeWordSpace(space)) == 0);
}

TEST_CASE("storage word ruby and annotation text are readable") {
	WordAnnotationRuby ruby;
	ruby.add_words()->set_content("か");
	ruby.add_words()->set_content("ん");
	WordNormal normal;
	*normal.mutable_annotation()->mutable_ruby() = ruby;
	const Word word                              = music_lyric_model::storage::makeWordNormal(normal);
	REQUIRE(music_lyric_model::storage::getWordRuby(word) != nullptr);
	CHECK(music_lyric_model::storage::getWordAnnotationText(*music_lyric_model::storage::getWordRuby(word)) == "かん");
}

TEST_CASE("storage active word lookup uses half-open time ranges") {
	WordNormal first;
	first.mutable_time()->set_start(1000);
	first.mutable_time()->set_end(1500);
	WordNormal second;
	second.mutable_time()->set_start(1500);
	second.mutable_time()->set_end(2000);
	RepeatedPtrField<Word> words;
	*words.Add() = music_lyric_model::storage::makeWordNormal(first);
	*words.Add() = music_lyric_model::storage::makeWordNormal(second);
	CHECK(music_lyric_model::storage::getActiveWordIndex(words, 1499) == 0);
	CHECK(music_lyric_model::storage::getActiveWordIndex(words, 1500) == 1);
	CHECK(music_lyric_model::storage::getActiveWordIndex(words, 2000) == -1);
	REQUIRE(music_lyric_model::storage::getActiveWord(words, 1500) != nullptr);
	CHECK(music_lyric_model::storage::getActiveWord(words, 1500) == &words.Get(1));
}
