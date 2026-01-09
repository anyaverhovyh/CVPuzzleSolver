#include "debug_io.h"

#include <gtest/gtest.h>

#include <libbase/configure_working_directory.h>
#include <libbase/runtime_assert.h>
#include <libimages/image_io.h>
#include <libimages/tests_utils.h>

TEST(debug_io, loadImageAndSaveCopy) {
    configureWorkingDirectory();

    image8u img = load_image("data/00_photo_six_parts_downscaled_x4.jpg");
    debug_io::dump_image(getUnitCaseDebugDir() + "copy.jpg", img);
}

TEST(debug_io, colorize32f) {
    configureWorkingDirectory();

    int size = 10;
    image32f values(size, size, 1);
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            values(j, i) = i + j;
        }
    }
    debug_io::dump_image(getUnitCaseDebugDir() + "colorized32f.jpg", values);
}

TEST(debug_io, colorizeLabels) {
    configureWorkingDirectory();

    int size = 10;
    image32i labels(size, size, 1);
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            labels(j, i) = i;
        }
    }
    int void_value = -1;
    labels(5, 5) = void_value;
    labels(5, 6) = void_value;
    labels(6, 5) = void_value;
    labels(6, 6) = void_value;
    debug_io::dump_image(getUnitCaseDebugDir() + "colorized32i.jpg", debug_io::colorize_labels(labels, void_value));
}