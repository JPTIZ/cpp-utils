#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>

#include "resources/bitmap.h"

template<typename T>
std::string to_string(const T& v) {
    std::string str = "";
    for (auto i = (signed int)sizeof(T); i > 0; --i) {
        str += (char)(v >> (8*(sizeof(T)-i)));
    }
    return str;
}

void print_bitmap_data(const resources::image::BMP& bmp) {
    std::cout
        << "|===============================|\n"
        << "| " << bmp.filename << std::string(29-bmp.filename.size(), ' ') << " |\n"
        << "|-------------------------------|\n"
        << "| File Type  \t| " << to_string(bmp.type) << "\t\t|\n"
        << "| File Size  \t| " << bmp.size << "B\t\t|\n"
        << "| App-Resv   \t| " << bmp.reserved_06_08 << "\t|\n"
        << "| BID Offset \t| " << bmp.bid_offset << "\t\t|\n"
        << "| Header Size\t| " << bmp.header_size << "\t\t|\n"
        << "| Width      \t| " << bmp.width << "\t\t|\n"
        << "| Height     \t| " << bmp.height << "\t\t|\n"
        << "| ColorPlanes\t| " << bmp.color_planes_count << "\t\t|\n"
        << "| BitsPerPixl\t| " << bmp.bpp << "\t\t|\n"
        << "| CompressMtd\t| " << bmp.compression_method << "\t|\n"
        << "| Image Size \t| " << bmp.image_size << "\t\t|\n"
        << "| Horz PPM   \t| " << bmp.h_ppm << "\t|\n"
        << "| Vert PPM   \t| " << bmp.v_ppm << "\t|\n"
        << "| PaletteLeng\t| " << bmp.palette_length << "\t\t|\n"
        << "| ImportantCl\t| " << bmp.important_colors << "\t\t|\n"
        << "| Map Entries\t| " << bmp.map.size() << "\t|\n"
        << "|===============================|\n";
}

void index_bitmap_palette(const resources::image::BMP& bmp,
        std::set<std::uint32_t>& palette,
        std::vector<std::uint8_t>& data) {
    constexpr auto ratio = ((double)(2<<5)) / ((double)(2<<8));
    std::vector<std::uint32_t> fuck;
    for (auto original : bmp.map) {
        unsigned int r = (original & 0xFF) * ratio;
        unsigned int g = ((original>>8) & 0xFF) * ratio;
        unsigned int b = ((original>>16) & 0xFF) * ratio;
        std::cout << "original = " << (original) << " - [" << r << ";" << g << ";" << b << "]\n";
        auto entry = r | (g << 8) | (b << 16);
        palette.insert(entry);
        data.push_back(std::distance(std::begin(palette), palette.find(entry)));
    }
}

void save_header(const std::set<std::uint32_t>& palette,
        const std::vector<std::uint8_t>& data,
        const std::string& filename_) {
    std::cout << "saving file: " << filename_ << ".h\n";
    std::ofstream os{filename_+".h", std::ios::binary};
    auto filename = filename_;
    std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);
    auto guard_name = "GBA_GENERATED_BMP_" + filename + "_H";
    os << "#ifndef " << guard_name
        << "\n#define " << guard_name
        << "\n\nconst auto " << filename << "_palette = {\n    ";
    auto i = 0u;
    for (auto it : palette) {
        if (i < palette.size()-1) {
            //os << "[" << (it & 0xFF) << ";" << ((it>>8)&0xFF) << ";" << ((it>>16)&0xFF) << "]" << ", ";
            os << it << ", ";
            if (i+1 % 40 == 0) os << "\n    ";
        } else {
            //os << "[" << (it & 0xFF) << ";" << ((it>>8)&0xFF) << ";" << ((it>>16)&0xFF) << "]" << ", ";
            os << it << "\n};\n";
        }
        ++i;
    }
    os << "\nconst auto " << filename << "_data = {\n    ";
    i = 1u;
    for (auto it : data) {
        if (i < data.size()) {
            os << (unsigned int)it << ", ";
            if (i % 30 == 0) os << "\n    ";
        } else {
            os << (unsigned int)it << "\n};\n\n";
        }
        ++i;
    }
    os << "\n#endif\n";
}

int main(int argc, char* argv[]) {
    std::cout << "oi\n" << std::endl;
    if (argc > 1) {
        for (auto i = 1u; i < argc; ++i) {
            resources::image::BMP bmp;
            resources::image::load_file(argv[i], bmp);
            print_bitmap_data(bmp);
            std::set<std::uint32_t> palette = {0};
            std::vector<std::uint8_t> data;
            index_bitmap_palette(bmp, palette, data);
            std::string filename{argv[1]};
            std::cout << "\nfilename = " << filename << "\n";
            filename = filename.substr(0, filename.find_last_of('.'));
            save_header(palette, data, filename);
            std::cout << "\nDone =)\n";
        }
    } else {
        std::cout << "Usage: imgreader filename.bmp\n";
    }
    return 0;
}

