#include "storage/word.h"

#include "doctest.h"

namespace Storage = music_lyric_model::storage;

using namespace lyric::storage;
using google::protobuf::RepeatedPtrField;

TEST_CASE("storage word makers preserve initial values") {
	WordAnnotationContent content;
	content.set_content("ni");
	CHECK(Storage::makeWordAnnotationContent(content).content() == "ni");
	WordAnnotationRoman roman;
	*roman.add_words() = content;
	CHECK(Storage::makeWordAnnotationRoman(roman).words(0).content() == "ni");
	WordAnnotationTranslate translate;
	translate.set_content("hello");
	CHECK(Storage::makeWordAnnotationTranslate(translate).content() == "hello");
	CHECK(Storage::makeWordAnnotationRuby().words().empty());
	CHECK(Storage::makeWordAnnotationUnknown().key().empty());
	CHECK(Storage::makeWordAnnotation().unknowns().empty());
}

TEST_CASE("storage word variants unwrap safely") {
	WordNormal normalInit;
	normalInit.set_content("hi");
	const Word normal = Storage::makeWordNormal(normalInit);
	WordSpace  spaceInit;
	spaceInit.set_count(2);
	const Word space = Storage::makeWordSpace(spaceInit);
	REQUIRE(Storage::asWordNormal(normal) != nullptr);
	CHECK(Storage::asWordNormal(normal)->content() == "hi");
	CHECK(Storage::asWordNormal(space) == nullptr);
	REQUIRE(Storage::asWordSpace(space) != nullptr);
	CHECK(Storage::asWordSpace(space)->count() == 2);
	CHECK(Storage::asWordSpace(normal) == nullptr);
	CHECK(Storage::isWordNormal(normal) == true);
	CHECK(Storage::isWordSpace(space) == true);
}

TEST_CASE("storage word text renders content and spaces") {
	WordNormal normal;
	normal.set_content("hi");
	WordSpace space;
	space.set_count(3);
	CHECK(Storage::getWordText(Storage::makeWordNormal(normal)) == "hi");
	CHECK(Storage::getWordText(Storage::makeWordSpace(space)) == "   ");
}

TEST_CASE("storage word time and duration accept a wrapper or bare normal word") {
	WordNormal init;
	init.set_content("hi");
	init.mutable_time()->set_start(1000);
	init.mutable_time()->set_end(1500);
	const Word        word   = Storage::makeWordNormal(init);
	const WordNormal* normal = Storage::asWordNormal(word);
	REQUIRE(normal != nullptr);
	REQUIRE(Storage::getWordTime(word) != nullptr);
	CHECK(Storage::getWordTime(word)->start() == 1000);
	REQUIRE(Storage::getWordTime(*normal) != nullptr);
	CHECK(Storage::getWordTime(*normal)->start() == 1000);
	CHECK(Storage::getWordDuration(word) == 500);
	CHECK(Storage::getWordDuration(*normal) == 500);
	WordSpace space;
	space.set_count(1);
	CHECK(Storage::getWordTime(Storage::makeWordSpace(space)) == nullptr);
	CHECK(Storage::getWordDuration(Storage::makeWordSpace(space)) == 0);
}

TEST_CASE("storage word ruby and annotation text are readable") {
	WordAnnotationRuby ruby;
	ruby.add_words()->set_content("か");
	ruby.add_words()->set_content("ん");
	WordNormal normal;
	*normal.mutable_annotation()->mutable_ruby() = ruby;
	const Word word                              = Storage::makeWordNormal(normal);
	REQUIRE(Storage::getWordRuby(word) != nullptr);
	CHECK(Storage::getWordAnnotationText(*Storage::getWordRuby(word)) == "かん");
}

TEST_CASE("storage active word lookup uses half-open time ranges") {
	WordNormal first;
	first.mutable_time()->set_start(1000);
	first.mutable_time()->set_end(1500);
	WordNormal second;
	second.mutable_time()->set_start(1500);
	second.mutable_time()->set_end(2000);
	RepeatedPtrField<Word> words;
	*words.Add() = Storage::makeWordNormal(first);
	*words.Add() = Storage::makeWordNormal(second);
	CHECK(Storage::getActiveWordIndex(words, 1499) == 0);
	CHECK(Storage::getActiveWordIndex(words, 1500) == 1);
	CHECK(Storage::getActiveWordIndex(words, 2000) == -1);
	REQUIRE(Storage::getActiveWord(words, 1500) != nullptr);
	CHECK(Storage::getActiveWord(words, 1500) == &words.Get(1));
}
