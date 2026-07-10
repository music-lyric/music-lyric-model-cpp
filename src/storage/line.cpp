#include "storage/line.h"

#include <unordered_set>

#include "common/time.h"
#include "storage/word.h"

namespace music_lyric_model::storage {
	lyric::storage::LineContent makeLineContent(const lyric::storage::LineContent& content) {
		return content;
	}

	lyric::storage::Line makeLine(const lyric::storage::Line& line) {
		return line;
	}

	lyric::storage::LineAnnotationUnknown makeLineAnnotationUnknown(const lyric::storage::LineAnnotationUnknown& unknown) {
		return unknown;
	}

	lyric::storage::LineAnnotationRoman makeLineAnnotationRoman(const lyric::storage::LineAnnotationRoman& roman) {
		return roman;
	}

	lyric::storage::LineAnnotationTranslate makeLineAnnotationTranslate(const lyric::storage::LineAnnotationTranslate& translate) {
		return translate;
	}

	lyric::storage::LineAnnotation makeLineAnnotation(const lyric::storage::LineAnnotation& annotation) {
		return annotation;
	}

	const lyric::storage::LineContent* getLineContent(const lyric::storage::Line& line) {
		return line.has_content() ? &line.content() : nullptr;
	}

	const lyric::common::Time* getLineTime(const lyric::storage::Line& line) {
		const lyric::storage::LineContent* content = getLineContent(line);
		return content && content->has_time() ? &content->time() : nullptr;
	}

	int64_t getLineDuration(const lyric::storage::Line& line) {
		return common::getTimeDuration(getLineTime(line));
	}

	const google::protobuf::RepeatedPtrField<lyric::storage::Word>& getLineWords(const lyric::storage::Line& line) {
		static const google::protobuf::RepeatedPtrField<lyric::storage::Word> empty;
		const lyric::storage::LineContent*                                    content = getLineContent(line);
		return content ? content->words() : empty;
	}

	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words) {
		std::string result;
		for (const auto& word : words) {
			result += getWordText(word);
		}
		return result;
	}

	std::string getLineText(const lyric::storage::Line& line) {
		return getWordsText(getLineWords(line));
	}

	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::storage::Word>& words) {
		std::vector<std::string>        result;
		std::unordered_set<std::string> seen;
		for (const auto& word : words) {
			if (word.body_case() != lyric::storage::Word::kNormal) {
				continue;
			}
			const lyric::storage::WordNormal& normal = word.normal();
			if (!normal.has_language() || normal.language().empty()) {
				continue;
			}
			if (seen.insert(normal.language()).second) {
				result.push_back(normal.language());
			}
		}
		return result;
	}

	std::vector<std::string> getLineLanguages(const lyric::storage::Line& line) {
		return getWordsLanguages(getLineWords(line));
	}

	const lyric::storage::LineAnnotation* getLineAnnotation(const lyric::storage::Line& line) {
		const lyric::storage::LineContent* content = getLineContent(line);
		return content && content->has_annotation() ? &content->annotation() : nullptr;
	}

	int getActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms) {
		for (int i = 0, len = lines.size(); i < len; i++) {
			if (common::isTimeActive(getLineTime(lines.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::storage::Line* getActiveLine(const google::protobuf::RepeatedPtrField<lyric::storage::Line>& lines, int64_t ms) {
		const int index = getActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines.Get(index);
	}

	std::optional<std::string> getLineTranslate(const lyric::storage::Line& line, const std::optional<std::string>& language) {
		const lyric::storage::LineAnnotation* annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::storage::LineAnnotationTranslate* item = getFirstAnnotation(annotation->translates(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}

	std::optional<std::string> getLineRoman(const lyric::storage::Line& line, const std::optional<std::string>& language) {
		const lyric::storage::LineAnnotation* annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::storage::LineAnnotationRoman* item = getFirstAnnotation(annotation->romans(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}
} // namespace music_lyric_model::storage
