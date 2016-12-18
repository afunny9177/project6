// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC
#include "stb_image_resize.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char *hex_table[] = {
    "00", "01", "02", "03", "04", "05", "06", "07",
    "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
    "10", "11", "12", "13", "14", "15", "16", "17",
    "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
    "20", "21", "22", "23", "24", "25", "26", "27",
    "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
    "30", "31", "32", "33", "34", "35", "36", "37",
    "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
    "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
    "50", "51", "52", "53", "54", "55", "56", "57",
    "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
    "60", "61", "62", "63", "64", "65", "66", "67",
    "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
    "70", "71", "72", "73", "74", "75", "76", "77",
    "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
    "80", "81", "82", "83", "84", "85", "86", "87",
    "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
    "90", "91", "92", "93", "94", "95", "96", "97",
    "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7",
    "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7",
    "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7",
    "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
    "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7",
    "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7",
    "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
};

bool generate_thumbnail(char* input_file, char* output_file, int width, int height)
{
    int inWidth, inHeight, channels;
    unsigned char* input_pixels = stbi_load(input_file, &inWidth, &inHeight, &channels, 0);
    if (!input_pixels)
        return false;

    unsigned char* output_pixels = (unsigned char*)malloc(width * height * channels);
    //stbir_resize_uint8_srgb(input_pixels, w, h, 0, output_pixels, out_w, out_h, 0, n, -1,0);
    stbir_resize_uint8(input_pixels, inWidth, inHeight, 0, output_pixels, width, height, 0, channels);

    stbi_write_png(output_file, width, height, channels, output_pixels, 0);
    free(output_pixels);
    free(input_pixels);
    return true;
}

bool generate_ascii_art(char* input_file, char* output_file, int width, int height)
{
    int inWidth, inHeight, channels;
    unsigned char* input_pixels = stbi_load(input_file, &inWidth, &inHeight, &channels, 0);
    if (!input_pixels)
        return false;

    channels = 3;
    unsigned char* output_pixels = (unsigned char*)malloc(width * height * channels);
    //stbir_resize_uint8_srgb(input_pixels, w, h, 0, output_pixels, out_w, out_h, 0, n, -1,0);
    stbir_resize_uint8(input_pixels, inWidth, inHeight, 0, output_pixels, width, height, 0, channels);

    unsigned char* p = output_pixels;

    std::string html;
    html.append("<!DOCTYPE html>");
    html.append("<html lang=\"en\">");
    html.append("<head>");
    html.append("<title>ascii art</title>");
    html.append("<meta charset=\"UTF-8\">");
    html.append("<style type=\"text/css\">");
    html.append("html, body{");
    html.append("height: 100%;");
    //html.append("overflow: hidden;");
    html.append("}");
    html.append("body{");
    html.append("width: 100%;");
    html.append("display: table;");
    html.append("margin: 0;");
    html.append("}");
    html.append("#main{");
    html.append("display: table-cell;");
    html.append("vertical-align: middle;");
    html.append("text-align: center;");
    html.append("}");
    html.append("</style>");
    html.append("</head>");
    html.append("<body>");
    html.append("<div id=\"main\">");
    html.append("<pre style=\"font: 8px/4px monospace;\">");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            html.append("<span style=\"color:#");
            html.append(hex_table[p[0]]);
            html.append(hex_table[p[1]]);
            html.append(hex_table[p[2]]);
            html.append("\">#</span>");

            p += 3;
        }

        html.append("\n");
    }

    html.append("</pre>");
    html.append("</div>");
    html.append("</html>");

    FILE *fp = fopen(output_file, "w");
    fwrite(html.c_str(), html.length(), 1, fp);
    fclose(fp);

    free(output_pixels);
    free(input_pixels);
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc != 5)
        return 0;

    char* input_file = argv[1];
    char* output_file = argv[2];
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);

    generate_ascii_art(input_file, output_file, width, height);
    return 0;
}