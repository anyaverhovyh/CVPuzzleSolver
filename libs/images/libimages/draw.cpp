#include "draw.h"

#include <libbase/runtime_assert.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <variant>

namespace {

template <typename OutT>
OutT convertComponent(std::uint8_t v) {
    if constexpr (std::is_same_v<OutT, std::uint8_t>) {
        return v;
    } else {
        return static_cast<OutT>(v);
    }
}

template <typename OutT>
OutT convertComponent(float v) {
    if constexpr (std::is_same_v<OutT, std::uint8_t>) {
        const float clamped = std::clamp(v, 0.0f, 255.0f);
        return static_cast<std::uint8_t>(std::lround(clamped));
    } else {
        return static_cast<OutT>(v);
    }
}

template <typename T, typename C>
void drawPointImpl(Image<T>& image, point2i pixel, const C& cc) {
    rassert(pixel.x >= 0 && pixel.x < image.width() && pixel.y >= 0 && pixel.y < image.height(),
            98237123, "Pixel out of bounds");

    const int x = pixel.x;
    const int y = pixel.y;

    if (image.channels() == 1) {
        // If color is RGB -> use channel 0 (R).
        const auto v = (cc.channels() == 1) ? cc(0) : cc(0);
        image(y, x) = convertComponent<T>(v);
        return;
    }

    rassert(image.channels() == 3, 98237124, "Only 1 or 3 channel images supported");

    if (cc.channels() == 1) {
        const auto v = convertComponent<T>(cc(0));
        image(y, x, 0) = v;
        image(y, x, 1) = v;
        image(y, x, 2) = v;
        return;
    }

    image(y, x, 0) = convertComponent<T>(cc(0));
    image(y, x, 1) = convertComponent<T>(cc(1));
    image(y, x, 2) = convertComponent<T>(cc(2));
}

} // namespace

template <typename T>
void drawPoint(Image<T>& image, point2i pixel, color c) {
    std::visit([&](const auto& cc) { drawPointImpl(image, pixel, cc); }, c);
}

template <typename T>
void drawPoints(Image<T>& image, const std::vector<point2i>& pixels, color c) {
    for (const auto& p : pixels) {
        drawPoint(image, p, c);
    }
}

// Explicit instantiations
template void drawPoint<std::uint8_t>(Image<std::uint8_t>& image, point2i pixel, color c);
template void drawPoint<float>(Image<float>& image, point2i pixel, color c);

template void drawPoints<std::uint8_t>(Image<std::uint8_t>& image, const std::vector<point2i>& pixels, color c);
template void drawPoints<float>(Image<float>& image, const std::vector<point2i>& pixels, color c);
