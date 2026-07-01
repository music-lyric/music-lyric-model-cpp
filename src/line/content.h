#ifndef MUSIC_LYRIC_LINE_CONTENT_H_
#define MUSIC_LYRIC_LINE_CONTENT_H_

#include <cstdint>
#include <string>
#include <vector>

#include "line/content.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates a LineContent, the sung content shared by normal and background lines.
	 */
	lyric::LineContent makeLineContent(const lyric::LineContent& content = {});

	/**
	 * Creates a normal line wrapped in a Line, with an optional time range.
	 */
	lyric::Line makeLineNormal(const lyric::LineNormal& normal = {}, const lyric::Time* time = nullptr);

	/**
	 * Creates a LineBackground, a background line attached to a normal line.
	 */
	lyric::LineBackground makeLineBackground(const lyric::LineBackground& background = {});

	/**
	 * Creates an interlude wrapped in a Line, with an optional time range.
	 */
	lyric::Line makeLineInterlude(const lyric::Time* time = nullptr);

	/**
	 * Whether a Line holds a normal line.
	 */
	bool isLineNormal(const lyric::Line& line);

	/**
	 * Whether a Line holds an interlude.
	 */
	bool isLineInterlude(const lyric::Line& line);

	/**
	 * Time range of a line, null when absent.
	 */
	const lyric::Time* getLineTime(const lyric::Line& line);

	/**
	 * Duration of a line in milliseconds.
	 */
	int64_t getLineDuration(const lyric::Line& line);

	/**
	 * Content of a line, null on an interlude.
	 */
	const lyric::LineContent* getLineContent(const lyric::Line& line);

	/**
	 * Words of a line, empty for an interlude.
	 */
	const google::protobuf::RepeatedPtrField<lyric::Word>& getLineWords(const lyric::Line& line);

	/**
	 * Text joined from a list of words.
	 */
	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::Word>& words);

	/**
	 * Plain text of a line.
	 */
	std::string getLineText(const lyric::Line& line);

	/**
	 * Distinct language tags among a list of words.
	 */
	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::Word>& words);

	/**
	 * Languages of a line: the explicit tags, otherwise those of its words.
	 */
	std::vector<std::string> getLineLanguages(const lyric::Line& line);

	/**
	 * Annotation of a line, null on an interlude.
	 */
	const lyric::LineAnnotation* getLineAnnotation(const lyric::Line& line);

	/**
	 * Index of the line active at the given moment, or -1 when none.
	 */
	int getActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::Line>& lines, int64_t ms);

	/**
	 * Line active at the given moment, null when none.
	 */
	const lyric::Line* getActiveLine(const google::protobuf::RepeatedPtrField<lyric::Line>& lines, int64_t ms);
} // namespace music_lyric_model

#endif
