//#include "bitmap.h"
#include <iostream>
#include "bmp.h"

void nice() {
    std::cout << "------------------_" << std::endl;
    std::cout << "nice!" << std::endl;
}

int main(int argc, const char* argv[]) {
    std::cout << "argc = " << argc << std::endl;
    for (auto i = 0u; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    BMP bmp;
    load_bmp(bmp, argv[1]);
    show_bmp_data(bmp);
    convert_to_16bpp(bmp);
    nice();
    return 0;
}
