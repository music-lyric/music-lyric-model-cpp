#include "parsed/line.h"

#include <algorithm>
#include <utility>

#include "common/line.h"
#include "common/time.h"
#include "common/word.h"

namespace music_lyric_model::parsed {
	namespace {
		/**
		 * Empty word list returned when a line has no words.
		 */
		const google::protobuf::RepeatedPtrField<lyric::common::Word>& emptyWords() {
			static const google::protobuf::RepeatedPtrField<lyric::common::Word> empty;
			return empty;
		}

		/**
		 * Explicit languages, or those derived from words when empty.
		 */
		template <typename T>
		std::vector<std::string> languagesOf(const T& item) {
			if (!item.languages().empty()) {
				return {item.languages().begin(), item.languages().end()};
			}
			return common::getWordsLanguages(item.words());
		}

		/**
		 * Translation content from an annotation, preferring a language match.
		 */
		std::optional<std::string> translationOf(const lyric::common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const lyric::common::LineAnnotationTranslation* item = common::getFirstAnnotation(annotation->translations(), language);
			return item ? std::optional<std::string>(item->content()) : std::nullopt;
		}

		/**
		 * Roman content from an annotation, preferring a language match.
		 */
		std::optional<std::string> romanOf(const lyric::common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const lyric::common::LineAnnotationRoman* item = common::getFirstAnnotation(annotation->romans(), language);
			return item ? std::optional<std::string>(item->content()) : std::nullopt;
		}

		/**
		 * Aggregates per-word annotations into line items, one per distinct group.
		 */
		template <typename Item, typename Result, typename Collect, typename GroupOf, typename TextOf, typename LanguageOf, typename Make>
		std::vector<Result> aggregate(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words, Collect collect, GroupOf groupOf, TextOf textOf, LanguageOf languageOf, Make make) {
			std::vector<std::string> groups;
			for (const auto& word : words) {
				if (word.body_case() != lyric::common::Word::kNormal) {
					continue;
				}
				const google::protobuf::RepeatedPtrField<Item>* items = collect(word.normal());
				if (!items) {
					continue;
				}
				for (const auto& item : *items) {
					std::string group = groupOf(item);
					if (std::find(groups.begin(), groups.end(), group) == groups.end()) {
						groups.push_back(std::move(group));
					}
				}
			}

			std::vector<Result> result;
			for (const auto& group : groups) {
				std::string                content;
				uint32_t                   pending = 0;
				std::optional<std::string> language;
				bool                       has = false;
				for (const auto& word : words) {
					if (word.body_case() == lyric::common::Word::kSpace) {
						pending += word.space().count();
						continue;
					}
					if (word.body_case() != lyric::common::Word::kNormal) {
						continue;
					}
					const google::protobuf::RepeatedPtrField<Item>* items = collect(word.normal());
					const Item*                                     found = nullptr;
					if (items) {
						for (const auto& item : *items) {
							if (groupOf(item) == group) {
								found = &item;
								break;
							}
						}
					}
					if (found) {
						if (has) {
							content.append(pending, ' ');
						}
						content += textOf(*found);
						if (!language.has_value()) {
							language = languageOf(*found);
						}
						pending = 0;
						has     = true;
					}
				}
				if (has) {
					result.push_back(make(group, content, language));
				}
			}
			return result;
		}
	} // namespace

	lyric::parsed::Line makeParsedLineNormal(const lyric::parsed::LineNormal& normal) {
		lyric::parsed::Line line;
		*line.mutable_normal() = normal;
		return line;
	}

	lyric::parsed::LineBackground makeParsedLineBackground(const lyric::parsed::LineBackground& background) {
		return background;
	}

	lyric::parsed::Line makeParsedLineInterlude(const lyric::parsed::LineInterlude& interlude) {
		lyric::parsed::Line line;
		*line.mutable_interlude() = interlude;
		return line;
	}

	bool isParsedLineNormal(const lyric::parsed::Line& line) {
		return line.body_case() == lyric::parsed::Line::kNormal;
	}

	bool isParsedLineInterlude(const lyric::parsed::Line& line) {
		return line.body_case() == lyric::parsed::Line::kInterlude;
	}

	const lyric::parsed::LineNormal* asParsedLineNormal(const lyric::parsed::Line& line) {
		return isParsedLineNormal(line) ? &line.normal() : nullptr;
	}

	const lyric::parsed::LineInterlude* asParsedLineInterlude(const lyric::parsed::Line& line) {
		return isParsedLineInterlude(line) ? &line.interlude() : nullptr;
	}

	const lyric::common::Time* getParsedLineTime(const lyric::parsed::Line& line) {
		if (line.body_case() == lyric::parsed::Line::kNormal) {
			return getParsedLineTime(line.normal());
		}
		if (line.body_case() == lyric::parsed::Line::kInterlude) {
			return getParsedLineTime(line.interlude());
		}
		return nullptr;
	}

	const lyric::common::Time* getParsedLineTime(const lyric::parsed::LineNormal& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	const lyric::common::Time* getParsedLineTime(const lyric::parsed::LineBackground& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	const lyric::common::Time* getParsedLineTime(const lyric::parsed::LineInterlude& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	int64_t getParsedLineDuration(const lyric::parsed::Line& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	int64_t getParsedLineDuration(const lyric::parsed::LineNormal& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	int64_t getParsedLineDuration(const lyric::parsed::LineBackground& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	int64_t getParsedLineDuration(const lyric::parsed::LineInterlude& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getParsedLineWords(const lyric::parsed::Line& line) {
		const lyric::parsed::LineNormal* normal = asParsedLineNormal(line);
		return normal ? normal->words() : emptyWords();
	}

	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getParsedLineWords(const lyric::parsed::LineNormal& line) {
		return line.words();
	}

	const google::protobuf::RepeatedPtrField<lyric::common::Word>& getParsedLineWords(const lyric::parsed::LineBackground& line) {
		return line.words();
	}

	std::string getParsedLineText(const lyric::parsed::Line& line) {
		return common::getWordsText(getParsedLineWords(line));
	}

	std::string getParsedLineText(const lyric::parsed::LineNormal& line) {
		return common::getWordsText(getParsedLineWords(line));
	}

	std::string getParsedLineText(const lyric::parsed::LineBackground& line) {
		return common::getWordsText(getParsedLineWords(line));
	}

	std::vector<std::string> getParsedLineLanguages(const lyric::parsed::Line& line) {
		const lyric::parsed::LineNormal* normal = asParsedLineNormal(line);
		return normal ? languagesOf(*normal) : std::vector<std::string>{};
	}

	std::vector<std::string> getParsedLineLanguages(const lyric::parsed::LineNormal& line) {
		return languagesOf(line);
	}

	std::vector<std::string> getParsedLineLanguages(const lyric::parsed::LineBackground& line) {
		return languagesOf(line);
	}

	const lyric::common::LineAnnotation* getParsedLineAnnotation(const lyric::parsed::Line& line) {
		const lyric::parsed::LineNormal* normal = asParsedLineNormal(line);
		return normal ? getParsedLineAnnotation(*normal) : nullptr;
	}

	const lyric::common::LineAnnotation* getParsedLineAnnotation(const lyric::parsed::LineNormal& line) {
		return line.has_annotation() ? &line.annotation() : nullptr;
	}

	const lyric::common::LineAnnotation* getParsedLineAnnotation(const lyric::parsed::LineBackground& line) {
		return line.has_annotation() ? &line.annotation() : nullptr;
	}

	int getParsedActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::parsed::Line>& lines, int64_t ms) {
		for (int i = 0, len = lines.size(); i < len; i++) {
			if (common::isTimeActive(getParsedLineTime(lines.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::parsed::Line* getParsedActiveLine(const google::protobuf::RepeatedPtrField<lyric::parsed::Line>& lines, int64_t ms) {
		const int index = getParsedActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines.Get(index);
	}

	std::optional<std::string> getParsedLineTranslation(const lyric::parsed::Line& line, const std::optional<std::string>& language) {
		return translationOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineTranslation(const lyric::parsed::LineNormal& line, const std::optional<std::string>& language) {
		return translationOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineTranslation(const lyric::parsed::LineBackground& line, const std::optional<std::string>& language) {
		return translationOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineRoman(const lyric::parsed::Line& line, const std::optional<std::string>& language) {
		return romanOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineRoman(const lyric::parsed::LineNormal& line, const std::optional<std::string>& language) {
		return romanOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineRoman(const lyric::parsed::LineBackground& line, const std::optional<std::string>& language) {
		return romanOf(getParsedLineAnnotation(line), language);
	}

	std::vector<lyric::common::LineAnnotationRoman> deriveParsedLineRomans(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words) {
		return aggregate<lyric::common::WordAnnotationRoman, lyric::common::LineAnnotationRoman>(
			words,
			[](const lyric::common::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::common::WordAnnotationRoman>* {
				return word.has_annotation() ? &word.annotation().romans() : nullptr;
			},
			[](const lyric::common::WordAnnotationRoman& item) -> std::string { return item.has_language() ? item.language() : std::string(); },
			[](const lyric::common::WordAnnotationRoman& item) -> std::string { return common::getWordAnnotationText(item); },
			[](const lyric::common::WordAnnotationRoman& item) -> std::optional<std::string> { return item.has_language() ? std::optional<std::string>(item.language()) : std::nullopt; },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				lyric::common::LineAnnotationRoman result;
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	std::vector<lyric::common::LineAnnotationTranslation> deriveParsedLineTranslations(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words) {
		return aggregate<lyric::common::WordAnnotationTranslation, lyric::common::LineAnnotationTranslation>(
			words,
			[](const lyric::common::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::common::WordAnnotationTranslation>* {
				return word.has_annotation() ? &word.annotation().translations() : nullptr;
			},
			[](const lyric::common::WordAnnotationTranslation& item) -> std::string { return item.has_language() ? item.language() : std::string(); },
			[](const lyric::common::WordAnnotationTranslation& item) -> std::string { return item.content(); },
			[](const lyric::common::WordAnnotationTranslation& item) -> std::optional<std::string> { return item.has_language() ? std::optional<std::string>(item.language()) : std::nullopt; },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				lyric::common::LineAnnotationTranslation result;
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	std::vector<lyric::common::Unknown> deriveParsedLineUnknowns(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words) {
		return aggregate<lyric::common::Unknown, lyric::common::Unknown>(
			words,
			[](const lyric::common::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::common::Unknown>* {
				return word.has_annotation() ? &word.annotation().unknowns() : nullptr;
			},
			[](const lyric::common::Unknown& item) -> std::string { return item.key(); },
			[](const lyric::common::Unknown& item) -> std::string { return item.value(); },
			[](const lyric::common::Unknown&) -> std::optional<std::string> { return std::nullopt; },
			[](const std::string& group, const std::string& content, const std::optional<std::string>&) {
				lyric::common::Unknown result;
				result.set_key(group);
				result.set_value(content);
				return result;
			});
	}

	lyric::common::LineAnnotation deriveParsedLineAnnotation(const google::protobuf::RepeatedPtrField<lyric::common::Word>& words) {
		lyric::common::LineAnnotation annotation;
		for (auto& item : deriveParsedLineUnknowns(words)) {
			*annotation.add_unknowns() = std::move(item);
		}
		for (auto& item : deriveParsedLineRomans(words)) {
			*annotation.add_romans() = std::move(item);
		}
		for (auto& item : deriveParsedLineTranslations(words)) {
			*annotation.add_translations() = std::move(item);
		}
		return annotation;
	}
} // namespace music_lyric_model::parsed
