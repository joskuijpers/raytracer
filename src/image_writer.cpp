#include "image_writer.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>

bool image::writeImage(const char *filename)
{
    FILE *file;
    int t;

    file = fopen(filename, "wb");
    if (!file) {
        printf("Dump file problem... fopen\n");
        return false;
    }

    fprintf(file, "P6\n%i %i\n255\n", _width, _height);

    std::vector<unsigned char> imageC(_image.size());

    for (unsigned int i = 0; i < _image.size(); ++i) {
        imageC[i] = (unsigned char)(_image[i] * 255.f);
    }

    t = (int)fwrite(&(imageC[0]), _width * _height * 3, 1, file);

    if (t != 1)
    {
        printf("Dump file problem... fwrite\n");
        return false;
    }

    fclose(file);
    
    return true;
}
