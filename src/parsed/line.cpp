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
		const std::vector<common::Word>& emptyWords() {
			static const std::vector<common::Word> empty;
			return empty;
		}

		/**
		 * Explicit languages, or those derived from words when empty.
		 */
		template <typename T>
		std::vector<std::string> languagesOf(const T& item) {
			if (!item.languages.empty()) {
				return item.languages;
			}
			return common::getWordsLanguages(item.words);
		}

		/**
		 * Translation content from an annotation, preferring a language match.
		 */
		std::optional<std::string> translationOf(const common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const common::LineAnnotationTranslation* item = common::getFirstAnnotation(annotation->translations, language);
			return item ? std::optional<std::string>(item->content) : std::nullopt;
		}

		/**
		 * Roman content from an annotation, preferring a language match.
		 */
		std::optional<std::string> romanOf(const common::LineAnnotation* annotation, const std::optional<std::string>& language) {
			if (!annotation) {
				return std::nullopt;
			}
			const common::LineAnnotationRoman* item = common::getFirstAnnotation(annotation->romans, language);
			return item ? std::optional<std::string>(item->content) : std::nullopt;
		}

		/**
		 * Aggregates per-word annotations into line items, one per distinct group.
		 */
		template <typename Item, typename Result, typename Collect, typename GroupOf, typename TextOf, typename LanguageOf, typename Make>
		std::vector<Result> aggregate(const std::vector<common::Word>& words, Collect collect, GroupOf groupOf, TextOf textOf, LanguageOf languageOf, Make make) {
			std::vector<std::string> groups;
			for (const auto& word : words) {
				const common::WordNormal* normal = common::asWordNormal(word);
				if (!normal) {
					continue;
				}
				const std::vector<Item>* items = collect(*normal);
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
					if (const common::WordSpace* space = common::asWordSpace(word)) {
						pending += space->count;
						continue;
					}
					const common::WordNormal* normal = common::asWordNormal(word);
					if (!normal) {
						continue;
					}
					const std::vector<Item>* items = collect(*normal);
					const Item*              found = nullptr;
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

	Line makeParsedLineNormal(LineNormal normal) {
		return Line{std::move(normal)};
	}

	LineBackground makeParsedLineBackground(LineBackground init) {
		return init;
	}

	Line makeParsedLineInterlude(LineInterlude interlude) {
		return Line{std::move(interlude)};
	}

	bool isParsedLineNormal(const Line& line) {
		return std::holds_alternative<LineNormal>(line);
	}

	bool isParsedLineInterlude(const Line& line) {
		return std::holds_alternative<LineInterlude>(line);
	}

	const LineNormal* asParsedLineNormal(const Line& line) {
		return std::get_if<LineNormal>(&line);
	}

	const LineInterlude* asParsedLineInterlude(const Line& line) {
		return std::get_if<LineInterlude>(&line);
	}

	const common::Time* getParsedLineTime(const Line& line) {
		if (const LineNormal* normal = asParsedLineNormal(line)) {
			return getParsedLineTime(*normal);
		}
		if (const LineInterlude* interlude = asParsedLineInterlude(line)) {
			return getParsedLineTime(*interlude);
		}
		return nullptr;
	}

	const common::Time* getParsedLineTime(const LineNormal& line) {
		return line.time.has_value() ? &*line.time : nullptr;
	}

	const common::Time* getParsedLineTime(const LineBackground& line) {
		return line.time.has_value() ? &*line.time : nullptr;
	}

	const common::Time* getParsedLineTime(const LineInterlude& line) {
		return line.time.has_value() ? &*line.time : nullptr;
	}

	int64_t getParsedLineDuration(const Line& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	int64_t getParsedLineDuration(const LineNormal& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	int64_t getParsedLineDuration(const LineBackground& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	int64_t getParsedLineDuration(const LineInterlude& line) {
		return common::getTimeDuration(getParsedLineTime(line));
	}

	const std::vector<common::Word>& getParsedLineWords(const Line& line) {
		const LineNormal* normal = asParsedLineNormal(line);
		return normal ? normal->words : emptyWords();
	}

	const std::vector<common::Word>& getParsedLineWords(const LineNormal& line) {
		return line.words;
	}

	const std::vector<common::Word>& getParsedLineWords(const LineBackground& line) {
		return line.words;
	}

	std::string getParsedLineText(const Line& line) {
		return common::getWordsText(getParsedLineWords(line));
	}

	std::string getParsedLineText(const LineNormal& line) {
		return common::getWordsText(getParsedLineWords(line));
	}

	std::string getParsedLineText(const LineBackground& line) {
		return common::getWordsText(getParsedLineWords(line));
	}

	std::vector<std::string> getParsedLineLanguages(const Line& line) {
		const LineNormal* normal = asParsedLineNormal(line);
		return normal ? languagesOf(*normal) : std::vector<std::string>{};
	}

	std::vector<std::string> getParsedLineLanguages(const LineNormal& line) {
		return languagesOf(line);
	}

	std::vector<std::string> getParsedLineLanguages(const LineBackground& line) {
		return languagesOf(line);
	}

	const common::LineAnnotation* getParsedLineAnnotation(const Line& line) {
		const LineNormal* normal = asParsedLineNormal(line);
		return normal ? getParsedLineAnnotation(*normal) : nullptr;
	}

	const common::LineAnnotation* getParsedLineAnnotation(const LineNormal& line) {
		return line.annotation.has_value() ? &*line.annotation : nullptr;
	}

	const common::LineAnnotation* getParsedLineAnnotation(const LineBackground& line) {
		return line.annotation.has_value() ? &*line.annotation : nullptr;
	}

	int getParsedActiveLineIndex(const std::vector<Line>& lines, int64_t ms) {
		for (int i = 0, len = static_cast<int>(lines.size()); i < len; i++) {
			if (common::isTimeActive(getParsedLineTime(lines[static_cast<size_t>(i)]), ms)) {
				return i;
			}
		}
		return -1;
	}

	const Line* getParsedActiveLine(const std::vector<Line>& lines, int64_t ms) {
		const int index = getParsedActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines[static_cast<size_t>(index)];
	}

	std::optional<std::string> getParsedLineTranslation(const Line& line, const std::optional<std::string>& language) {
		return translationOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineTranslation(const LineNormal& line, const std::optional<std::string>& language) {
		return translationOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineTranslation(const LineBackground& line, const std::optional<std::string>& language) {
		return translationOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineRoman(const Line& line, const std::optional<std::string>& language) {
		return romanOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineRoman(const LineNormal& line, const std::optional<std::string>& language) {
		return romanOf(getParsedLineAnnotation(line), language);
	}

	std::optional<std::string> getParsedLineRoman(const LineBackground& line, const std::optional<std::string>& language) {
		return romanOf(getParsedLineAnnotation(line), language);
	}

	std::vector<common::LineAnnotationRoman> deriveParsedLineRomans(const std::vector<common::Word>& words) {
		return aggregate<common::WordAnnotationRoman, common::LineAnnotationRoman>(
			words,
			[](const common::WordNormal& word) -> const std::vector<common::WordAnnotationRoman>* {
				return word.annotation.has_value() ? &word.annotation->romans : nullptr;
			},
			[](const common::WordAnnotationRoman& item) -> std::string { return item.language.value_or(std::string()); },
			[](const common::WordAnnotationRoman& item) -> std::string { return common::getWordAnnotationText(item); },
			[](const common::WordAnnotationRoman& item) -> std::optional<std::string> { return item.language; },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				return common::LineAnnotationRoman{language, content};
			});
	}

	std::vector<common::LineAnnotationTranslation> deriveParsedLineTranslations(const std::vector<common::Word>& words) {
		return aggregate<common::WordAnnotationTranslation, common::LineAnnotationTranslation>(
			words,
			[](const common::WordNormal& word) -> const std::vector<common::WordAnnotationTranslation>* {
				return word.annotation.has_value() ? &word.annotation->translations : nullptr;
			},
			[](const common::WordAnnotationTranslation& item) -> std::string { return item.language.value_or(std::string()); },
			[](const common::WordAnnotationTranslation& item) -> std::string { return item.content; },
			[](const common::WordAnnotationTranslation& item) -> std::optional<std::string> { return item.language; },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				return common::LineAnnotationTranslation{language, content};
			});
	}

	std::vector<common::Unknown> deriveParsedLineUnknowns(const std::vector<common::Word>& words) {
		return aggregate<common::Unknown, common::Unknown>(
			words,
			[](const common::WordNormal& word) -> const std::vector<common::Unknown>* {
				return word.annotation.has_value() ? &word.annotation->unknowns : nullptr;
			},
			[](const common::Unknown& item) -> std::string { return item.key; },
			[](const common::Unknown& item) -> std::string { return item.value; },
			[](const common::Unknown&) -> std::optional<std::string> { return std::nullopt; },
			[](const std::string& group, const std::string& content, const std::optional<std::string>&) {
				return common::Unknown{group, content};
			});
	}

	common::LineAnnotation deriveParsedLineAnnotation(const std::vector<common::Word>& words) {
		common::LineAnnotation annotation;
		annotation.unknowns     = deriveParsedLineUnknowns(words);
		annotation.romans       = deriveParsedLineRomans(words);
		annotation.translations = deriveParsedLineTranslations(words);
		return annotation;
	}
} // namespace music_lyric_model::parsed
