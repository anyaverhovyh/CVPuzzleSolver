#include <libimages/algorithms/grayscale.h>
#include <libimages/algorithms/threshold_masking.h>
#include <libimages/algorithms/morphology.h>

#include <libbase/stats.h>
#include <libbase/timer.h>
#include <libbase/runtime_assert.h>
#include <libbase/configure_working_directory.h>
#include <libimages/debug_io.h>
#include <libimages/image.h>
#include <libimages/image_io.h>

#include <iostream>

int main() {
    try {
        configureWorkingDirectory();

        Timer total_t;
        Timer t;

        image8u image = load_image("data/00_photo_six_parts_downscaled_x4.jpg");
        auto [w, h, c] = image.size();
        rassert(c == 3, 237045347618912, image.channels());
        std::cout << "image loaded in " << t.elapsed() << " sec" << std::endl;
        debug_io::dump_image("debug/00_input.jpg", image);

        image32f grayscale = to_grayscale_float(image);
        rassert(grayscale.channels() == 1, 2317812937193);
        rassert(grayscale.width() == w && grayscale.height() == h, 7892137419283791);
        debug_io::dump_image("debug/01_grayscale.jpg", grayscale);

        std::vector<float> intensities_on_border;
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                // skip everything except borders
                if (i != 0 && i != w - 1 && j != 0 && j != h - 1)
                    continue;
                intensities_on_border.push_back(grayscale(j, i));
            }
        }
        // TODO what invariant we can check with rassert about intensities_on_border.size()?
        rassert(intensities_on_border.size() == 2 * w + 2 * h - 4, 7283197129381312);
        std::cout << "intensities on border: " << stats::summaryStats(intensities_on_border) << std::endl;

        // TODO find background_threshold
        double background_threshold = 1.5 * stats::percentile(intensities_on_border, 90);
        std::cout << "background threshold=" << background_threshold << std::endl;

        // TODO build is_foreground_mask + save its visualization on disk + print into log percent of background
        image8u is_foreground_mask = threshold_masking(grayscale, background_threshold);
        double is_foreground_sum = stats::sum(is_foreground_mask.toVector());
        std::cout << "thresholded background: " << stats::toPercent(w * h - is_foreground_sum / 255.0, 1.0 * w * h) << std::endl;
        debug_io::dump_image("debug/02_is_foreground_mask.png", is_foreground_mask);

        t.restart();
        // TODO make mask more smooth thanks to morphology
        // TODO firstly try dilation + erosion, is the mask robust? no outliers?
        int strength = 3;
        image8u dilated_mask = morphology::dilate(is_foreground_mask, strength);
        image8u dilated_eroded_mask = morphology::erode(dilated_mask, strength);
        image8u dilated_eroded_eroded_mask = morphology::erode(dilated_eroded_mask, strength);
        image8u dilated_eroded_eroded_dilated_mask = morphology::dilate(dilated_eroded_eroded_mask, strength);
        is_foreground_mask = dilated_eroded_eroded_dilated_mask;
        std::cout << "morphology in " << t.elapsed() << " sec" << std::endl;
        debug_io::dump_image("debug/03_is_foreground_dilated.png", dilated_mask);
        debug_io::dump_image("debug/04_is_foreground_dilated_eroded.png", dilated_eroded_mask);
        debug_io::dump_image("debug/05_is_foreground_dilated_eroded_eroded.png", dilated_eroded_eroded_mask);
        debug_io::dump_image("debug/06_is_foreground_dilated_eroded_eroded_dilated.png", dilated_eroded_eroded_dilated_mask);

        std::cout << "processed in " << total_t.elapsed() << " sec" << std::endl;

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }
}
