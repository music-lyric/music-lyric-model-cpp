#include "meta/meta.h"

namespace music_lyric_model {
	lyric::MetaCreator makeMetaCreator() {
		return {};
	}

	lyric::MetaItem makeMetaItem() {
		return {};
	}

	bool hasMeta(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, lyric::MetaItem::ContentCase kind) {
		for (const auto& meta : metas) {
			if (meta.content_case() == kind) {
				return true;
			}
		}
		return false;
	}

	std::vector<const lyric::MetaItem*> getAllMeta(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, lyric::MetaItem::ContentCase kind) {
		std::vector<const lyric::MetaItem*> result;
		for (const auto& meta : metas) {
			if (meta.content_case() == kind) {
				result.push_back(&meta);
			}
		}
		return result;
	}

	const lyric::MetaItem* getFirstMeta(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, lyric::MetaItem::ContentCase kind) {
		for (const auto& meta : metas) {
			if (meta.content_case() == kind) {
				return &meta;
			}
		}
		return nullptr;
	}

	std::vector<const lyric::MetaItem*> getMetaByKey(const google::protobuf::RepeatedPtrField<lyric::MetaItem>& metas, const std::string& key) {
		std::vector<const lyric::MetaItem*> result;
		for (const auto& meta : metas) {
			if (meta.key() == key) {
				result.push_back(&meta);
			}
		}
		return result;
	}
} // namespace music_lyric_model
