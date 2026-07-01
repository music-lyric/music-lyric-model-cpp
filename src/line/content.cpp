#include "line/content.h"

#include <unordered_set>

#include "common/time.h"
#include "word/content.h"

namespace music_lyric_model {
	lyric::LineContent makeLineContent(const lyric::LineContent& content) {
		return content;
	}

	lyric::Line makeLineNormal(const lyric::LineNormal& normal, const lyric::Time* time) {
		lyric::Line line;
		if (time) {
			*line.mutable_time() = *time;
		}
		*line.mutable_normal() = normal;
		return line;
	}

	lyric::LineBackground makeLineBackground(const lyric::LineBackground& background) {
		return background;
	}

	lyric::Line makeLineInterlude(const lyric::Time* time) {
		lyric::Line line;
		if (time) {
			*line.mutable_time() = *time;
		}
		line.mutable_interlude();
		return line;
	}

	bool isLineNormal(const lyric::Line& line) {
		return line.body_case() == lyric::Line::kNormal;
	}

	bool isLineInterlude(const lyric::Line& line) {
		return line.body_case() == lyric::Line::kInterlude;
	}

	const lyric::Time* getLineTime(const lyric::Line& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	int64_t getLineDuration(const lyric::Line& line) {
		return getTimeDuration(getLineTime(line));
	}

	const lyric::LineContent* getLineContent(const lyric::Line& line) {
		if (line.body_case() != lyric::Line::kNormal) {
			return nullptr;
		}
		const lyric::LineNormal& normal = line.normal();
		return normal.has_content() ? &normal.content() : nullptr;
	}

	const google::protobuf::RepeatedPtrField<lyric::Word>& getLineWords(const lyric::Line& line) {
		static const google::protobuf::RepeatedPtrField<lyric::Word> empty;
		const lyric::LineContent*                                    content = getLineContent(line);
		return content ? content->words() : empty;
	}

	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::Word>& words) {
		std::string result;
		for (const auto& word : words) {
			result += getWordText(word);
		}
		return result;
	}

	std::string getLineText(const lyric::Line& line) {
		return getWordsText(getLineWords(line));
	}

	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::Word>& words) {
		std::vector<std::string>        result;
		std::unordered_set<std::string> seen;
		for (const auto& word : words) {
			if (word.body_case() != lyric::Word::kNormal) {
				continue;
			}
			const lyric::WordNormal& normal = word.normal();
			if (!normal.has_language() || normal.language().empty()) {
				continue;
			}
			if (seen.insert(normal.language()).second) {
				result.push_back(normal.language());
			}
		}
		return result;
	}

	std::vector<std::string> getLineLanguages(const lyric::Line& line) {
		const lyric::LineContent* content = getLineContent(line);
		if (!content) {
			return {};
		}
		if (!content->languages().empty()) {
			return {content->languages().begin(), content->languages().end()};
		}
		return getWordsLanguages(content->words());
	}

	const lyric::LineAnnotation* getLineAnnotation(const lyric::Line& line) {
		const lyric::LineContent* content = getLineContent(line);
		return content && content->has_annotation() ? &content->annotation() : nullptr;
	}

	int getActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::Line>& lines, int64_t ms) {
		for (int i = 0, len = lines.size(); i < len; i++) {
			if (isTimeActive(getLineTime(lines.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::Line* getActiveLine(const google::protobuf::RepeatedPtrField<lyric::Line>& lines, int64_t ms) {
		const int index = getActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines.Get(index);
	}
} // namespace music_lyric_model
