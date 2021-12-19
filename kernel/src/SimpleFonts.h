#ifndef LUFTOS_SIMPLEFONTS_H
#define LUFTOS_SIMPLEFONTS_H

struct Psf1Header
{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charSize;
};

struct Psf1Font
{
    Psf1Header* psf1Header;
    char* glyphBuffer;
};

#endif
