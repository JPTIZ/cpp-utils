#include "bmp.h"

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

template<typename T>
void load4(std::istream& is, T& field) {
    field = is.get() | is.get()<<8 | is.get()<<16 | is.get()<<24;
}

template<typename T>
void load2(std::istream& is, T& field) {
    field = is.get() | is.get()<<8;
}

void load_bmp(BMP& bmp, const std::string& filename) {
    std::ifstream is{filename, std::ios::binary};
    load2(is, bmp.type);
    load4(is, bmp.size);
    load4(is, bmp.res_06_08);
    load4(is, bmp.offset);
    load4(is, bmp.header_size);
    load4(is, bmp.width);
    load4(is, bmp.height);
    load2(is, bmp.planes);
    load2(is, bmp.bpp);
    load4(is, bmp.compress_type);
    load4(is, bmp.data_size);
    load4(is, bmp.hor_res);
    load4(is, bmp.ver_res);
    load4(is, bmp.color_count);
    load4(is, bmp.important_color_count);
    is.seekg(bmp.offset);
    std::cout << "remaining bytes: \n";
    std::map<std::uint16_t, std::uint8_t> palette;
    auto& colors = bmp.palette;
    auto& data = bmp.data;
    for (auto i = 0u; i < bmp.width*bmp.height; ++i) {
        auto b = ((std::uint16_t)is.get()*(2<<5)/(2<<8));
        auto g = ((std::uint16_t)is.get()*(2<<5)/(2<<8));
        auto r = ((std::uint16_t)is.get()*(2<<5)/(2<<8));
        std::uint16_t pixel = r + (g<<5) + (b<<10);
        if (not palette.count(pixel)) {
            std::cout << "inserting {"
                << r << " , "
                << g << " , "
                << b << "} = { " << pixel << " } to palette" << std::endl;
            colors.push_back(pixel);
            palette.insert(
                    std::pair<std::uint16_t, std::uint8_t>(
                        pixel, colors.size()-1)
                    );
        }
        data.push_back(palette[pixel]);
    }
    // 180flip

    for (auto i = 0u; i < bmp.width; ++i) {
        for (auto j = 0u; j < bmp.height/2; ++j) {
            std::cout << "swap["
                << i << ", "
                << j << "] <-> ["
                << i << ", "
                << (bmp.height - (j+1))
                << "]" << std::endl;
            std::swap(
                    data[i + bmp.width * j],
                    data[i + bmp.width * (bmp.height - (j + 1))]
                    );
        }
    }

    std::cout << "palette: " << std::endl;
    auto i = 0u;
    for (auto color : bmp.palette) {
        std::cout << "\t"
            << i << ": \t{\t"
            << ((color>>10)&31) << ", \t"
            << ((color>>5)&31) << ", \t"
            << (color&31) << "\t}"
            << std::endl;
        ++i;
    }
    i = 0u;
    std::cout << "data: ";
    for (auto index : bmp.data) {
        if (i % bmp.width == 0) std::cout << std::endl;
        std::cout << (int)index << " : ";
        ++i;
    }
    std::cout << std::endl;
}

void show_bmp_data(BMP& bmp) {
    std::cout << "\n| type: " << (char)bmp.type << (char)(bmp.type>>8) << std::endl;
    std::cout << "| size: " << bmp.size << "B" << std::endl;
    std::cout << "| offset: " << bmp.offset << std::endl;
    std::cout << "| header_size: " << bmp.header_size << std::endl;
    std::cout << "| size: " << bmp.width << "x" << bmp.height << std::endl;
    std::cout << "| planes: " << bmp.planes << std::endl;
    std::cout << "| bpp: " << bmp.bpp << std::endl;
    std::cout << "| compress_type: " << bmp.compress_type << std::endl;
    std::cout << "| data_size: " << bmp.data_size << "B" << std::endl;
    std::cout << "| res: " << bmp.hor_res << "x" << bmp.ver_res << std::endl;
    std::cout << "| colors: " << bmp.color_count << std::endl;
    std::cout << "| important colors: " << bmp.important_color_count << std::endl;
}

void convert_to_16bpp(BMP& bmp) {
    std::cout << "conversion is done on loading (but shouldn't)" << std::endl;
}

void save_header(BMP& bmp, const std::string& filename) {
    std::cout << "saving header..." << std::endl;
    std::ofstream os{filename+".h", std::ios::binary};
    std::string filename_{filename};
    filename_ = filename_.substr(0, filename_.find_last_of('.'));
    std::transform(
            filename_.begin(),
            filename_.end(),
            filename_.begin(),
            ::toupper);
    auto guard_name = "GBA_GENERATED_BMP_" + filename_ + "_H";
    os << "#ifndef " << guard_name
        << "\n#define " << guard_name
        << "\n\n#include <cstdint>\n\nconst std::uint16_t " << filename_ << "_palette[] = {\n  ";
    auto i = 0u;
    for (auto it : bmp.palette) {
        if (i < bmp.palette.size()-1) {
            os << it << ", ";
            if (i+1 % 40 == 0) os << "\n    ";
        } else {
            os << it << "\n};\n";
        }
        ++i;
    }
    os << "\nconst std::uint8_t " << filename_ << "_data[] = {\n  ";
    i = 1u;
    for (auto it : bmp.data) {
        if (i < bmp.data.size()) {
            os << (unsigned int)it << ", ";
            if (i % 20 == 0) os << "\n  ";
        } else {
            os << (unsigned int)it << "\n};\n";
        }
        ++i;
    }
    os << "\n#endif\n";
}
