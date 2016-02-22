#include "bmp.h"

#include <iostream>
#include <fstream>

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
    //while (!is.eof()) {
        for (auto i = 0u; i < bmp.width*bmp.height; ++i) {
            std::cout <<
                "{"
                << ((unsigned int)is.get()*(2<<5)/(2<<8)) << ",\t"
                << ((unsigned int)is.get()*(2<<5)/(2<<8)) << ",\t"
                << ((unsigned int)is.get()*(2<<5)/(2<<8)) <<
                "\t} ";
        }
        std::cout << std::endl;
    //}
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
}
