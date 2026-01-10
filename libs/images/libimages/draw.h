#pragma once

#include <vector>

#include <libbase/point2.h>
#include <libimages/image.h>

#include "color.h"

template <typename T>
void drawPoint(Image<T>& image, point2i pixel, color c);

template <typename T>
void drawPoints(Image<T>& image, const std::vector<point2i>& pixels, color c);

extern template void drawPoint<std::uint8_t>(Image<std::uint8_t>& image, point2i pixel, color c);
extern template void drawPoint<float>(Image<float>& image, point2i pixel, color c);

extern template void drawPoints<std::uint8_t>(Image<std::uint8_t>& image, const std::vector<point2i>& pixels, color c);
extern template void drawPoints<float>(Image<float>& image, const std::vector<point2i>& pixels, color c);
