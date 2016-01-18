#include "bitmap.h"

namespace resources {
namespace image {

namespace {
template<typename T>
void read_data(std::ifstream& is, T& field, std::size_t size) {
    for (auto i = (signed int)size-1; i >= 0; --i) {
        field |= is.get() << (8u*i);
    }
}

template<typename T>
void read_data(T& field, std::ifstream& is) {
    read_data(is, field, sizeof(field));
}

template<typename T>
void reverse_read_data(T& field, std::ifstream& is) {
    for (auto i = 0; i < sizeof(field); ++i) {
        field |= is.get() << (8u * i);
    }
}

}

void load_file(const std::string& filename, BMP& bmp) {
    using COLORREF = std::uint32_t;
    bmp.filename = filename;
    std::ifstream is{filename, std::ios::binary};
    reverse_read_data(bmp.type, is);
    bmp.size = is.get();
    read_data(bmp.reserved_06_08, is);
//    bmp.reserved_06_08 = is.get() | (is.get() << 8) | (is.get() << 16) | (is.get() << 24);
    read_data(bmp.bid_offset, is);
    read_data(bmp.header_size, is);
    read_data(bmp.width, is);
    read_data(bmp.height, is);
    read_data(bmp.color_planes_count, is);
    read_data(bmp.bpp, is);
    read_data(bmp.compression_method, is);
    read_data(bmp.image_size, is);
    read_data(bmp.h_ppm, is);
    read_data(bmp.v_ppm, is);
    read_data(bmp.palette_length, is);
    read_data(bmp.important_colors, is);
    is.seekg(bmp.bid_offset);
    while (!is.eof()) {
        COLORREF pixel{0};
        for (auto j = 0; j < bmp.width; ++j) {
            for (auto i = 0; i < bmp.bpp/8; ++i) {
                pixel |= (is.get() << (16 - 8*i));
            }
            bmp.map.push_back(pixel);
            auto dummy = is.get() | is.get() | is.get() | is.get();
        }
    }
}

}
}

