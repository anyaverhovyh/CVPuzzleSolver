#include "grayscale.h"

#include <libbase/runtime_assert.h>

image32f to_grayscale_float(const image8u& img) {
    rassert(img.channels() == 1 || img.channels() == 3 || img.channels() == 4, "Unsupported channel count", img.channels());

    image32f gray(img.width(), img.height(), 1);

    if (img.channels() == 1) {
        for (int j = 0; j < img.height(); ++j)
            for (int i = 0; i < img.width(); ++i)
                gray(j, i) = (float) img(j, i);
        return gray;
    }

    for (int j = 0; j < img.height(); ++j) {
        for (int i = 0; i < img.width(); ++i) {
            const float r = (float) img(j, i, 0);
            const float g = (float) img(j, i, 1);
            const float b = (float) img(j, i, 2);
            gray(j, i) = 0.299f * r + 0.587f * g + 0.114f * b;
        }
    }
    return gray;
}
