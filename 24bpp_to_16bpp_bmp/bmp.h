#ifndef TO16BPP_BMP_H
#define TO16BPP_BMP_H

#include <string>
#include <cstdint>
#include <vector>

struct RGB {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

struct BMP {
    std::uint16_t type;
    std::uint32_t size;
    std::uint32_t res_06_08;
    std::uint32_t offset;
    std::uint32_t header_size;
    std::uint32_t width;
    std::uint32_t height;
    std::uint16_t planes;
    std::uint16_t bpp;
    std::uint32_t compress_type;
    std::uint32_t data_size;
    std::uint32_t hor_res;
    std::uint32_t ver_res;
    std::uint32_t color_count;
    std::uint32_t important_color_count;
    std::vector<std::uint16_t> palette;
    std::vector<std::uint8_t> data;
};

void load_bmp(BMP&, const std::string& filename);
void show_bmp_data(BMP&);
void convert_to_16bpp(BMP&);
void save_header(BMP& bmp, const std::string& filename);

#endif
