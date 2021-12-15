#include "GDT.h"

GDT DefaultGDT
{
    {0, 0, 0, 0, 0, 0}, // null entry
    {0, 0, 0, 0b10011010, 0b10100000, 0}, // kernel code
    {0, 0, 0, 0b10010010, 0b10100000, 0}, // kernel data
    {0, 0, 0, 0, 0, 0}, // user null
    {0, 0, 0, 0b10011010, 0b10100000, 0}, // user code
    {0, 0, 0, 0b10010010, 0b10100000, 0} // user data
};