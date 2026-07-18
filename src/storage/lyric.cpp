#include "storage/lyric.h"

#include <algorithm>
#include <stdexcept>

#include "bridge/storage/lyric.gen.h"
#include "storage/line.h"
#include "storage/lyric.pb.h"
#include "version.h"

namespace music_lyric_model::storage {
	Lyric makeStorageLyric(Lyric init) {
		init.version = SCHEMA_VERSION;
		return init;
	}

	std::vector<uint8_t> encodeStorageLyric(const Lyric& lyric) {
		const lyric::storage::Lyric pb = internal::toPb(lyric);
		std::vector<uint8_t>        out(pb.ByteSizeLong());
		if (!pb.SerializeToArray(out.data(), static_cast<int>(out.size()))) {
			throw std::runtime_error("failed to encode Lyric to binary");
		}
		return out;
	}

	Lyric decodeStorageLyric(const std::vector<uint8_t>& data) {
		lyric::storage::Lyric pb;
		if (!pb.ParseFromArray(data.data(), static_cast<int>(data.size()))) {
			throw std::runtime_error("failed to decode Lyric from binary");
		}
		return internal::fromPb(pb);
	}

	void sortStorageLinesByTime(Lyric& lyric) {
		std::stable_sort(lyric.lines.begin(), lyric.lines.end(), [](const Line& a, const Line& b) {
			const common::Time* ta = getStorageLineTime(a);
			const common::Time* tb = getStorageLineTime(b);
			return (ta ? ta->start : 0) < (tb ? tb->start : 0);
		});

		for (auto& line : lyric.lines) {
			std::stable_sort(line.backgrounds.begin(), line.backgrounds.end(), [](const LineBackground& a, const LineBackground& b) {
				const uint32_t sa = a.time.has_value() ? a.time->start : 0;
				const uint32_t sb = b.time.has_value() ? b.time->start : 0;
				return sa < sb;
			});
		}
	}
} // namespace music_lyric_model::storage
