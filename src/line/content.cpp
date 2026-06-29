#include "line/content.h"

#include <unordered_set>

#include "common/time.h"
#include "word/content.h"

namespace music_lyric_model {
	lyric::Line makeLineNormal(const lyric::LineNormal &normal) {
		lyric::Line line;
		*line.mutable_normal() = normal;
		return line;
	}

	lyric::LineBackground makeLineBackground() {
		return {};
	}

	lyric::Line makeLineInterlude(const lyric::LineInterlude &interlude) {
		lyric::Line line;
		*line.mutable_interlude() = interlude;
		return line;
	}

	bool isLineNormal(const lyric::Line &line) {
		return line.body_case() == lyric::Line::kNormal;
	}

	bool isLineInterlude(const lyric::Line &line) {
		return line.body_case() == lyric::Line::kInterlude;
	}

	const lyric::Time *getLineTime(const lyric::Line &line) {
		if (line.body_case() == lyric::Line::kNormal) {
			const lyric::LineNormal &normal = line.normal();
			return normal.has_time() ? &normal.time() : nullptr;
		}
		if (line.body_case() == lyric::Line::kInterlude) {
			const lyric::LineInterlude &interlude = line.interlude();
			return interlude.has_time() ? &interlude.time() : nullptr;
		}
		return nullptr;
	}

	int64_t getLineDuration(const lyric::Line &line) {
		return getTimeDuration(getLineTime(line));
	}

	const google::protobuf::RepeatedPtrField<lyric::Word> &getLineWords(const lyric::Line &line) {
		static const google::protobuf::RepeatedPtrField<lyric::Word> empty;
		return line.body_case() == lyric::Line::kNormal ? line.normal().words() : empty;
	}

	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::Word> &words) {
		std::string result;
		for (const auto &word : words) {
			result += getWordText(word);
		}
		return result;
	}

	std::string getLineText(const lyric::Line &line) {
		return getWordsText(getLineWords(line));
	}

	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::Word> &words) {
		std::vector<std::string> result;
		std::unordered_set<std::string> seen;
		for (const auto &word : words) {
			if (word.body_case() != lyric::Word::kNormal) {
				continue;
			}
			const lyric::WordNormal &normal = word.normal();
			if (!normal.has_language() || normal.language().empty()) {
				continue;
			}
			if (seen.insert(normal.language()).second) {
				result.push_back(normal.language());
			}
		}
		return result;
	}

	std::vector<std::string> getLineLanguages(const lyric::Line &line) {
		if (line.body_case() != lyric::Line::kNormal) {
			return {};
		}
		const lyric::LineNormal &normal = line.normal();
		if (!normal.languages().empty()) {
			return {normal.languages().begin(), normal.languages().end()};
		}
		return getWordsLanguages(normal.words());
	}

	const lyric::LineAnnotation *getLineAnnotation(const lyric::Line &line) {
		if (line.body_case() != lyric::Line::kNormal) {
			return nullptr;
		}
		const lyric::LineNormal &normal = line.normal();
		return normal.has_annotation() ? &normal.annotation() : nullptr;
	}
}
