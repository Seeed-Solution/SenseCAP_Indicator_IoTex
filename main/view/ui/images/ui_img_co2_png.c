// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.6
// Project name: sensecap

#include "../ui.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
    #define LV_ATTRIBUTE_MEM_ALIGN
#endif

// IMAGE DATA: assets/co2.png
const LV_ATTRIBUTE_MEM_ALIGN uint8_t ui_img_co2_png_data[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x10,0xFF,0xFF,0x60,0xFF,0xFF,0x9F,0xFF,0xFF,0xCF,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xFF,0xFF,0xBF,0xFF,0xFF,0x90,0xFF,0xFF,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x10,0xFF,0xFF,0x7F,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xFF,0xFF,0x5F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x6F,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xFF,0xFF,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x10,0xFF,0xFF,0x9F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x10,0xFF,0xFF,0xCF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xA0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x6F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xF7,0xFF,
    0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xF7,0xFF,0x9D,0xEF,0xFF,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0xFF,0xFF,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x20,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9D,0xEF,0xFF,0xAF,0x7D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x12,0x96,0xFF,0x75,0xAE,0xFF,0x0B,0x5D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xA0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9D,0xEF,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,
    0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x0B,0x5D,0xFF,0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x40,0x00,0x00,0x00,0xFF,0xFF,0x10,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x12,0x96,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBE,0x00,0x00,0x00,0xFF,0xFF,0x60,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,
    0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x10,0xFF,0xFF,0xA0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0xCF,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x6E,0x75,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,0xAF,0x7D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x60,0xFF,0xFF,0xCF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xD7,0xBE,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,0x7B,0xDF,0xFF,0x7B,0xDF,0xFF,0x9D,0xEF,0xFF,0xB6,0xB6,0xFF,0xEA,0x54,0xFF,0xD7,0xBE,0xFF,0xDE,0xF7,0xFF,0x3A,0xD7,0xFF,0x9D,0xEF,0xFF,0x6E,0x75,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,
    0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x8F,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xF7,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xB6,0xB6,0xFF,0xD7,0xBE,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,0xB6,0xB6,0xFF,0x12,0x96,0xFF,0x9D,0xEF,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0xB6,0xB6,0xFF,0x3A,0xD7,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0xCF,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x3A,0xD7,0xFF,0x53,0x9E,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x75,0xAE,0xFF,0x3A,0xD7,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0xDE,0xF7,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xBE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xD7,0xBE,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x19,0xCF,0xFF,0xB6,0xB6,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x12,0x96,0xFF,0x75,0xAE,0xFF,0x7B,0xDF,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xF1,0x8D,0xFF,0x9D,0xEF,0xFF,0x4D,0x6D,0xFF,0xD7,0xBE,0xFF,0xB6,0xB6,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBE,0xFF,0xFF,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0xCF,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0xDE,0xF7,0xFF,0x53,0x9E,0xFF,0xB6,0xB6,0xFF,0x9D,0xEF,0xFF,0x0B,0x5D,0xFF,0x9D,0xEF,0xFF,0xD7,0xBE,0xFF,0x12,0x96,0xFF,0x9D,0xEF,0xFF,0x75,0xAE,0xFF,0x0B,0x5D,0xFF,0xF1,0x8D,0xFF,0x3A,0xD7,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9E,0xFF,0xFF,0xBF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xF7,0xFF,0x0B,0x5D,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0xB6,0xB6,0xFF,0xB6,0xB6,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,0xB6,0xB6,0xFF,0xD7,0xBE,0xFF,0x12,0x96,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,0xDE,0xF7,0xFF,0x75,0xAE,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x12,0x96,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0xFF,0xFF,0x90,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0xCF,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x0B,0x5D,0xFF,0x4D,0x6D,0xFF,0x4D,0x6D,0xFF,0xEA,0x54,0xFF,0x4D,0x6D,0xFF,0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x40,0xFF,0xFF,0x40,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0x3A,0xD7,0xFF,0x6E,0x75,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xAF,0x7D,0xFF,0x9D,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xF7,0xFF,0x7B,0xDF,0xFF,0x6E,0x75,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x75,0xAE,0xFF,0x9D,0xEF,0xFF,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9E,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xDE,0xF7,0xFF,0x12,0x96,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0xEA,0x54,0xFF,0x0B,0x5D,0xFF,0xB6,0xB6,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x10,0xFF,0xFF,0xDE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9D,0xEF,0xFF,0xB6,0xB6,0xFF,0x53,0x9E,0xFF,0x53,0x9E,0xFF,0xD7,0xBE,0xFF,0xDE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x9E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x40,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xFF,0xFF,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x70,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0xFF,0xFF,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0xFF,0xFF,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x70,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDE,0xFF,0xFF,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x20,0xFF,0xFF,0xCE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0x8F,0xFF,0xFF,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x50,0xFF,0xFF,0xBE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xFF,0xFF,0x90,0xFF,0xFF,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x20,0xFF,0xFF,0x50,0xFF,0xFF,0x80,0xFF,0xFF,0x9E,0xFF,0xFF,0xBE,0xFF,0xFF,0xAF,0xFF,0xFF,0x9F,0xFF,0xFF,0x6F,0xFF,0xFF,0x40,0xFF,0xFF,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const lv_img_dsc_t ui_img_co2_png = {
    .header.always_zero = 0,
    .header.w = 30,
    .header.h = 30,
    .data_size = sizeof(ui_img_co2_png_data),
    .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
    .data = ui_img_co2_png_data
};

