#include "texture.h"
#include "renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string &filepath) : _id(0), _filepath(filepath), _buffer(0), _width(0), _height(0), _bpp(0)
{
    stbi_set_flip_vertically_on_load(true);
    _buffer = stbi_load(filepath.c_str(), &_width, &_height, &_bpp, 4);

    glCall(glGenTextures(1, &_id));;
    glCall(glBindTexture(GL_TEXTURE_2D, _id));

    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _buffer));
    Unbind();

    if (_buffer)
        stbi_image_free(_buffer);
}

Texture::~Texture() 
{
    glCall(glDeleteTextures(1, &_id));
}

void Texture::Bind(unsigned int slot) const
{
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, _id));
}

void Texture::Unbind() const
{
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
