#include "runtime/line.h"

#include <algorithm>
#include <unordered_set>
#include <utility>

#include "common/time.h"
#include "runtime/word.h"

namespace music_lyric_model {
	namespace {
		/**
		 * Aggregates per-word annotations into line items, one per distinct group.
		 *
		 * `collect` selects a word's annotation items, `groupOf` names each item's group, `textOf` and `languageOf` read its text and language, `make` builds the line item.
		 *
		 * Text is joined in word order with spaces padded to follow word spacing; groups keep first-seen order.
		 */
		template <typename Item, typename Result, typename Collect, typename GroupOf, typename TextOf, typename LanguageOf, typename Make>
		std::vector<Result> aggregate(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words, Collect collect, GroupOf groupOf, TextOf textOf, LanguageOf languageOf, Make make) {
			std::vector<std::string> groups;
			for (const auto& word : words) {
				if (word.body_case() != lyric::runtime::Word::kNormal) {
					continue;
				}
				const google::protobuf::RepeatedPtrField<Item>* items = collect(word.normal());
				if (!items) {
					continue;
				}
				for (const auto& item : *items) {
					std::string group = groupOf(item);
					if (std::find(groups.begin(), groups.end(), group) == groups.end()) {
						groups.push_back(group);
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
					if (word.body_case() == lyric::runtime::Word::kSpace) {
						pending += word.space().count();
						continue;
					}
					if (word.body_case() != lyric::runtime::Word::kNormal) {
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

	lyric::runtime::LineContent makeLineContent(const lyric::runtime::LineContent& content) {
		return content;
	}

	lyric::runtime::Line makeLineNormal(const lyric::runtime::LineNormal& normal, const lyric::common::Time* time) {
		lyric::runtime::Line line;
		if (time) {
			*line.mutable_time() = *time;
		}
		*line.mutable_normal() = normal;
		return line;
	}

	lyric::runtime::LineBackground makeLineBackground(const lyric::runtime::LineBackground& background) {
		return background;
	}

	lyric::runtime::Line makeLineInterlude(const lyric::common::Time* time) {
		lyric::runtime::Line line;
		if (time) {
			*line.mutable_time() = *time;
		}
		line.mutable_interlude();
		return line;
	}

	lyric::runtime::LineAnnotationUnknown makeLineAnnotationUnknown(const lyric::runtime::LineAnnotationUnknown& unknown) {
		return unknown;
	}

	lyric::runtime::LineAnnotationRoman makeLineAnnotationRoman(const lyric::runtime::LineAnnotationRoman& roman) {
		return roman;
	}

	lyric::runtime::LineAnnotationTranslate makeLineAnnotationTranslate(const lyric::runtime::LineAnnotationTranslate& translate) {
		return translate;
	}

	lyric::runtime::LineAnnotation makeLineAnnotation(const lyric::runtime::LineAnnotation& annotation) {
		return annotation;
	}

	bool isLineNormal(const lyric::runtime::Line& line) {
		return line.body_case() == lyric::runtime::Line::kNormal;
	}

	bool isLineInterlude(const lyric::runtime::Line& line) {
		return line.body_case() == lyric::runtime::Line::kInterlude;
	}

	const lyric::runtime::LineNormal* asLineNormal(const lyric::runtime::Line& line) {
		return isLineNormal(line) ? &line.normal() : nullptr;
	}

	const lyric::runtime::LineInterlude* asLineInterlude(const lyric::runtime::Line& line) {
		return isLineInterlude(line) ? &line.interlude() : nullptr;
	}

	const lyric::common::Time* getLineTime(const lyric::runtime::Line& line) {
		return line.has_time() ? &line.time() : nullptr;
	}

	int64_t getLineDuration(const lyric::runtime::Line& line) {
		return getTimeDuration(getLineTime(line));
	}

	const lyric::runtime::LineContent* getLineContent(const lyric::runtime::Line& line) {
		if (line.body_case() != lyric::runtime::Line::kNormal) {
			return nullptr;
		}
		const lyric::runtime::LineNormal& normal = line.normal();
		return normal.has_content() ? &normal.content() : nullptr;
	}

	const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& getLineWords(const lyric::runtime::Line& line) {
		static const google::protobuf::RepeatedPtrField<lyric::runtime::Word> empty;
		const lyric::runtime::LineContent*                                    content = getLineContent(line);
		return content ? content->words() : empty;
	}

	std::string getWordsText(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words) {
		std::string result;
		for (const auto& word : words) {
			result += getWordText(word);
		}
		return result;
	}

	std::string getLineText(const lyric::runtime::Line& line) {
		return getWordsText(getLineWords(line));
	}

	std::vector<std::string> getWordsLanguages(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words) {
		std::vector<std::string>        result;
		std::unordered_set<std::string> seen;
		for (const auto& word : words) {
			if (word.body_case() != lyric::runtime::Word::kNormal) {
				continue;
			}
			const lyric::runtime::WordNormal& normal = word.normal();
			if (!normal.has_language() || normal.language().empty()) {
				continue;
			}
			if (seen.insert(normal.language()).second) {
				result.push_back(normal.language());
			}
		}
		return result;
	}

	std::vector<std::string> getLineLanguages(const lyric::runtime::Line& line) {
		const lyric::runtime::LineContent* content = getLineContent(line);
		if (!content) {
			return {};
		}
		if (!content->languages().empty()) {
			return {content->languages().begin(), content->languages().end()};
		}
		return getWordsLanguages(content->words());
	}

	const lyric::runtime::LineAnnotation* getLineAnnotation(const lyric::runtime::Line& line) {
		const lyric::runtime::LineContent* content = getLineContent(line);
		return content && content->has_annotation() ? &content->annotation() : nullptr;
	}

	int getActiveLineIndex(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines, int64_t ms) {
		for (int i = 0, len = lines.size(); i < len; i++) {
			if (isTimeActive(getLineTime(lines.Get(i)), ms)) {
				return i;
			}
		}
		return -1;
	}

	const lyric::runtime::Line* getActiveLine(const google::protobuf::RepeatedPtrField<lyric::runtime::Line>& lines, int64_t ms) {
		const int index = getActiveLineIndex(lines, ms);
		return index == -1 ? nullptr : &lines.Get(index);
	}

	std::optional<std::string> getLineTranslate(const lyric::runtime::Line& line, const std::optional<std::string>& language) {
		const lyric::runtime::LineAnnotation* annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::runtime::LineAnnotationTranslate* item = getFirstAnnotation(annotation->translates(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}

	std::optional<std::string> getLineRoman(const lyric::runtime::Line& line, const std::optional<std::string>& language) {
		const lyric::runtime::LineAnnotation* annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::runtime::LineAnnotationRoman* item = getFirstAnnotation(annotation->romans(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}

	std::vector<lyric::runtime::LineAnnotationRoman> deriveLineRomans(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words) {
		return aggregate<lyric::runtime::WordAnnotationRoman, lyric::runtime::LineAnnotationRoman>(
			words,
			[](const lyric::runtime::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::runtime::WordAnnotationRoman>* {
				return word.has_annotation() ? &word.annotation().romans() : nullptr;
			},
			[](const lyric::runtime::WordAnnotationRoman& item) -> std::string { return item.has_language() ? item.language() : std::string(); },
			[](const lyric::runtime::WordAnnotationRoman& item) -> std::string { return getWordAnnotationText(item); },
			[](const lyric::runtime::WordAnnotationRoman& item) -> std::optional<std::string> { return item.has_language() ? std::optional<std::string>(item.language()) : std::nullopt; },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				lyric::runtime::LineAnnotationRoman result;
				result.set_derived(true);
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	std::vector<lyric::runtime::LineAnnotationTranslate> deriveLineTranslates(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words) {
		return aggregate<lyric::runtime::WordAnnotationTranslate, lyric::runtime::LineAnnotationTranslate>(
			words,
			[](const lyric::runtime::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::runtime::WordAnnotationTranslate>* {
				return word.has_annotation() ? &word.annotation().translates() : nullptr;
			},
			[](const lyric::runtime::WordAnnotationTranslate& item) -> std::string { return item.has_language() ? item.language() : std::string(); },
			[](const lyric::runtime::WordAnnotationTranslate& item) -> std::string { return item.content(); },
			[](const lyric::runtime::WordAnnotationTranslate& item) -> std::optional<std::string> { return item.has_language() ? std::optional<std::string>(item.language()) : std::nullopt; },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				lyric::runtime::LineAnnotationTranslate result;
				result.set_derived(true);
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	std::vector<lyric::runtime::LineAnnotationUnknown> deriveLineUnknowns(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words) {
		return aggregate<lyric::runtime::WordAnnotationUnknown, lyric::runtime::LineAnnotationUnknown>(
			words,
			[](const lyric::runtime::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::runtime::WordAnnotationUnknown>* {
				return word.has_annotation() ? &word.annotation().unknowns() : nullptr;
			},
			[](const lyric::runtime::WordAnnotationUnknown& item) -> std::string { return item.key(); },
			[](const lyric::runtime::WordAnnotationUnknown& item) -> std::string { return item.value(); },
			[](const lyric::runtime::WordAnnotationUnknown&) -> std::optional<std::string> { return std::nullopt; },
			[](const std::string& group, const std::string& content, const std::optional<std::string>&) {
				lyric::runtime::LineAnnotationUnknown result;
				result.set_derived(true);
				result.set_key(group);
				result.set_value(content);
				return result;
			});
	}

	lyric::runtime::LineAnnotation deriveLineAnnotation(const google::protobuf::RepeatedPtrField<lyric::runtime::Word>& words) {
		lyric::runtime::LineAnnotation annotation;
		for (auto& item : deriveLineUnknowns(words)) {
			*annotation.add_unknowns() = std::move(item);
		}
		for (auto& item : deriveLineTranslates(words)) {
			*annotation.add_translates() = std::move(item);
		}
		for (auto& item : deriveLineRomans(words)) {
			*annotation.add_romans() = std::move(item);
		}
		return annotation;
	}
} // namespace music_lyric_model
