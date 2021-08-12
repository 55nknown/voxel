#ifndef TEXTURE_H
#define TEXTURE_H

#include "renderer.h"

class Texture
{
private:
    unsigned int _id;
    const std::string &_filepath;
    unsigned char *_buffer;
    int _width, _height, _bpp;

public:
    Texture(const std::string &filepath);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return _width; }
    inline int GetHeight() const { return _height; }
};

#endif // TEXTURE_H