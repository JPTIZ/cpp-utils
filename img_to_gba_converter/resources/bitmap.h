#ifndef GENERATORS_BITMAP_H
#define GENERATORS_BITMAP_H

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

namespace resources {
namespace image {

struct BMP {
    std::string filename{""};
    std::uint16_t type{0};
    std::uint32_t size{0};
    std::uint32_t reserved_06_08{0};
    std::uint32_t bid_offset{0}; // Bitmap Image Data (Pixel Array) beginning offset
    std::uint32_t header_size{40};
    std::uint32_t width{0};
    std::uint32_t height{0};
    std::uint16_t color_planes_count{1};
    std::uint16_t bpp{8};
    std::uint32_t compression_method{0};
    std::uint32_t image_size{0};
    std::int32_t h_ppm{0};
    std::int32_t v_ppm{0};
    std::uint32_t palette_length{0};   // 0 = 2^n
    std::uint32_t important_colors{0}; // 0 = every color is important
    std::vector<std::uint32_t> map;
};

void load_file(const std::string& filename, BMP&);

}
}

#endif

