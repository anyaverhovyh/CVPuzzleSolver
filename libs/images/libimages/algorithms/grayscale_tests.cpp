#include "grayscale.h"

#include <gtest/gtest.h>

#include <libbase/configure_working_directory.h>
#include <libbase/runtime_assert.h>
#include <libimages/debug_io.h>
#include <libimages/image_io.h>
#include <libimages/tests_utils.h>

TEST(grayscale, loadImageAndThresholdByConstant) {
    configureWorkingDirectory();

    image8u img = load_image("data/00_photo_six_parts_downscaled_x4.jpg");
    image32f grayscale = to_grayscale_float(img);
    debug_io::dump_image(getUnitCaseDebugDir() + "grayscale.jpg", grayscale);
}
