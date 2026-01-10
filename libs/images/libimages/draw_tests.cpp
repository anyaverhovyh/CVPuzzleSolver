#include "draw.h"

#include <gtest/gtest.h>

#include <libbase/configure_working_directory.h>

#include <libimages/debug_io.h>
#include <libimages/tests_utils.h>

TEST(draw, drawPoint_gray_u8) {
    configureWorkingDirectory();

    image8u img(10, 8, 1);
    img.fill(0);

    drawPoint(img, point2i{3, 4}, color8u(static_cast<std::uint8_t>(255)));

    EXPECT_EQ(img(4, 3), 255);

    debug_io::dump_image(getUnitCaseDebugDir() + "draw.jpg", img);
}

TEST(draw, drawPoints_gray_u8) {
    configureWorkingDirectory();

    image8u img(7, 7, 1);
    img.fill(0);

    std::vector<point2i> pts = { {1, 1}, {2, 2}, {3, 3} };
    drawPoints(img, pts, color8u(static_cast<std::uint8_t>(100)));

    EXPECT_EQ(img(1, 1), 100);
    EXPECT_EQ(img(2, 2), 100);
    EXPECT_EQ(img(3, 3), 100);

    debug_io::dump_image(getUnitCaseDebugDir() + "draw.jpg", img);
}

TEST(draw, drawPoint_rgb_f32) {
    configureWorkingDirectory();

    image32f img(6, 5, 3);
    img.fill(0.0f);

    drawPoint(img, point2i{2, 1}, color32f(1.5f, 2.5f, 3.5f));

    EXPECT_FLOAT_EQ(img(1, 2, 0), 1.5f);
    EXPECT_FLOAT_EQ(img(1, 2, 1), 2.5f);
    EXPECT_FLOAT_EQ(img(1, 2, 2), 3.5f);

    debug_io::dump_image(getUnitCaseDebugDir() + "draw.jpg", img);
}

TEST(draw, grayColor_to_rgbImage_replicates) {
    configureWorkingDirectory();

    image32f img(4, 4, 3);
    img.fill(0.0f);

    drawPoint(img, point2i{0, 0}, color32f(7.0f));

    EXPECT_FLOAT_EQ(img(0, 0, 0), 7.0f);
    EXPECT_FLOAT_EQ(img(0, 0, 1), 7.0f);
    EXPECT_FLOAT_EQ(img(0, 0, 2), 7.0f);

    debug_io::dump_image(getUnitCaseDebugDir() + "draw.jpg", img);
}

TEST(draw, rgbColor_to_grayImage_takes_first_channel) {
    configureWorkingDirectory();

    image8u img(4, 4, 1);
    img.fill(0);

    drawPoint(img, point2i{1, 2}, color8u(10, 20, 30)); // uses R = 10 for grayscale

    EXPECT_EQ(img(2, 1), 10);

    debug_io::dump_image(getUnitCaseDebugDir() + "draw.jpg", img);
}
