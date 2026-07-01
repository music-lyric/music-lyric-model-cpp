#include "line/annotation.h"

#include <algorithm>

#include "line/content.h"
#include "word/annotation.h"

namespace music_lyric_model {
	namespace {
		/**
		 * Aggregates per-word annotations into line items, one per distinct group.
		 *
		 * `collect` selects a word's annotation tokens, `groupOf` names each token's group, `make` builds the line item.
		 *
		 * Tokens are joined in word order with spaces padded to follow word spacing; groups keep first-seen order.
		 */
		template <typename Item, typename Result, typename Collect, typename GroupOf, typename Make>
		std::vector<Result> aggregate(const google::protobuf::RepeatedPtrField<lyric::Word>& words, Collect collect, GroupOf groupOf, Make make) {
			std::vector<std::string> groups;
			for (const auto& word : words) {
				if (word.body_case() != lyric::Word::kNormal) {
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
					if (word.body_case() == lyric::Word::kSpace) {
						pending += word.space().count();
						continue;
					}
					if (word.body_case() != lyric::Word::kNormal) {
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
						content += getWordAnnotationText(*found);
						if (!language.has_value() && found->has_language()) {
							language = found->language();
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

	lyric::LineAnnotationUnknown makeLineAnnotationUnknown() {
		return {};
	}

	lyric::LineAnnotationRoman makeLineAnnotationRoman() {
		return {};
	}

	lyric::LineAnnotationTranslate makeLineAnnotationTranslate() {
		return {};
	}

	lyric::LineAnnotation makeLineAnnotation() {
		return {};
	}

	std::optional<std::string> getLineTranslate(const lyric::Line& line, const std::optional<std::string>& language) {
		const lyric::LineAnnotation* annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::LineAnnotationTranslate* item = getFirstAnnotation(annotation->translates(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}

	std::optional<std::string> getLineRoman(const lyric::Line& line, const std::optional<std::string>& language) {
		const lyric::LineAnnotation* annotation = getLineAnnotation(line);
		if (!annotation) {
			return std::nullopt;
		}
		const lyric::LineAnnotationRoman* item = getFirstAnnotation(annotation->romans(), language);
		return item ? std::optional<std::string>(item->content()) : std::nullopt;
	}

	std::vector<lyric::LineAnnotationRoman> deriveLineRomans(const google::protobuf::RepeatedPtrField<lyric::Word>& words) {
		return aggregate<lyric::WordAnnotationRoman, lyric::LineAnnotationRoman>(
			words,
			[](const lyric::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::WordAnnotationRoman>* {
				return word.has_annotation() ? &word.annotation().romans() : nullptr;
			},
			[](const lyric::WordAnnotationRoman& item) -> std::string { return item.has_language() ? item.language() : std::string(); },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				lyric::LineAnnotationRoman result;
				result.set_derived(true);
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	std::vector<lyric::LineAnnotationTranslate> deriveLineTranslates(const google::protobuf::RepeatedPtrField<lyric::Word>& words) {
		return aggregate<lyric::WordAnnotationTranslate, lyric::LineAnnotationTranslate>(
			words,
			[](const lyric::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::WordAnnotationTranslate>* {
				return word.has_annotation() ? &word.annotation().translates() : nullptr;
			},
			[](const lyric::WordAnnotationTranslate& item) -> std::string { return item.has_language() ? item.language() : std::string(); },
			[](const std::string&, const std::string& content, const std::optional<std::string>& language) {
				lyric::LineAnnotationTranslate result;
				result.set_derived(true);
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	std::vector<lyric::LineAnnotationUnknown> deriveLineUnknowns(const google::protobuf::RepeatedPtrField<lyric::Word>& words) {
		return aggregate<lyric::WordAnnotationUnknown, lyric::LineAnnotationUnknown>(
			words,
			[](const lyric::WordNormal& word) -> const google::protobuf::RepeatedPtrField<lyric::WordAnnotationUnknown>* {
				return word.has_annotation() ? &word.annotation().unknowns() : nullptr;
			},
			[](const lyric::WordAnnotationUnknown& item) -> std::string { return item.key(); },
			[](const std::string& group, const std::string& content, const std::optional<std::string>& language) {
				lyric::LineAnnotationUnknown result;
				result.set_key(group);
				result.set_derived(true);
				if (language.has_value()) {
					result.set_language(*language);
				}
				result.set_content(content);
				return result;
			});
	}

	lyric::LineAnnotation deriveLineAnnotation(const google::protobuf::RepeatedPtrField<lyric::Word>& words) {
		lyric::LineAnnotation annotation;
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
