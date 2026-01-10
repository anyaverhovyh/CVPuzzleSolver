#include "debug_io.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <libbase/runtime_assert.h>
#include <libbase/fast_random.h>

#include <libimages/image_io.h>
#include <limits>
#include <map>

namespace debug_io {

void ensure_dir_exists_for_file(const std::string &filepath) {
    namespace fs = std::filesystem;
    fs::path p(filepath);
    fs::path dir = p.parent_path();
    if (dir.empty()) {
        return;
    }

    std::error_code ec;
    fs::create_directories(dir, ec);
    rassert(!ec, "Failed to create directories", dir.string(), filepath, ec.message());
}

image8u normalize(const image32f &img, float void_value) {
    rassert(img.channels() == 1 || img.channels() == 3, "normalize expects 1/3-channel float image", img.channels());

    float maxv = 0.0f;
    for (int j = 0; j < img.height(); ++j) {
        for (int i = 0; i < img.width(); ++i) {
            for (int c = 0; c < img.channels(); ++c) {
                float v = img(j, i, c);
                if (v == void_value)
                    continue;
                maxv = std::max(maxv, v);
            }
        }
    }

    image8u out(img.width(), img.height(), 3);

    const float inv = 255.0f / maxv;
    for (int j = 0; j < img.height(); ++j) {
        for (int i = 0; i < img.width(); ++i) {
            for (int c = 0; c < img.channels(); ++c) {
                float v = img(j, i, c);
                if (v == void_value) {
                    // green pixel if void value
                    out(j, i, 0) = 0;
                    out(j, i, 1) = 255;
                    out(j, i, 2) = 0;
                    continue;
                }
                float x = v * inv;
                for (int c = 0; c < 3; ++c) {
                    out(j, i, c) = (uint8_t) std::lround(x);
                }
            }
            for (int c = img.channels(); c < out.channels(); ++c) {
                out(j, i, c) = out(j, i, c - 1);
            }
        }
    }
    return out;
}

image8u colorize_labels(const image32i &labels, int void_value, std::uint32_t seed) {
    rassert(labels.channels() == 1, "colorize_labels expects 1-channel labels", labels.channels());

    FastRandom r(seed);
    std::map<int, std::tuple<uint8_t, uint8_t, uint8_t>> mapped_colors;

    int w = labels.width();
    int h = labels.height();
    image8u out(w, h, 3);
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            const int label = labels(j, i);
            if (label == void_value) {
                // black color
                out(j, i, 0) = 0;
                out(j, i, 1) = 0;
                out(j, i, 2) = 0;
                continue;
            }

            if (mapped_colors.count(label) == 0) {
                std::tuple<uint8_t, uint8_t, uint8_t> random_color = {r.nextInt(0, 255), r.nextInt(0, 255), r.nextInt(0, 255)};
                mapped_colors[label] = random_color;
            }

            std::tuple<uint8_t, uint8_t, uint8_t> label_color = mapped_colors[label];
            out(j, i, 0) = std::get<0>(label_color);
            out(j, i, 1) = std::get<1>(label_color);
            out(j, i, 2) = std::get<2>(label_color);
        }
    }

    return out;
}

void dump_image(const std::string &path, const image8u &img) {
    std::cerr << "[debug_io] saving " << path << " (" << img.width() << "x" << img.height() << "x" << img.channels() << ")" << std::endl;
    ensure_dir_exists_for_file(path);
    save_image(img, path);
}

void dump_image(const std::string &path, const image32f &img32f, float void_value) {
    image8u img = normalize(img32f, void_value);
    dump_image(path, img);
}

} // namespace debug_io