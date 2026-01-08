#include <libimages/image.h>
#include <libimages/image_io.h>

#include <iostream>

int main() {
    try {
        image8u image = load_image("data/00_photo_six_parts.jpg");

        image8u image_copy(image.width(), image.height(), image.channels());

        save_image(image_copy, "copy.jpg");
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }
}
