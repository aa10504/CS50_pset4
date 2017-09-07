/**
 * BMP-related data types based on Microsoft's own.
 */

#include <stdint.h>

/**
 * Common Data Types 
 *
 * The data types in this section are essentially aliases for C/C++ 
 * primitive data types.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE; //定義資料型態 "BYTE", 佔了8 bits, 1byte
typedef uint32_t DWORD; //32 bits, 4 bytes
typedef int32_t  LONG; //32 bits, 4 bytes
typedef uint16_t WORD; //16 bits, 2 bytes

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx.
 */
typedef struct 
{ 
    WORD bfType; //The file type; must be BM
    DWORD bfSize; //bfSize contains the total size of file (in byte), includes pixels, padding, and headers; The size, in bytes, of the bitmap file.
    WORD bfReserved1; //Reserved; must be zero.
    WORD bfReserved2; //Reserved; must be zero.
    DWORD bfOffBits; //The offset, in bytes, from the beginning of the BITMAPFILEHEADER structure to the bitmap bits.
} __attribute__((__packed__)) 
BITMAPFILEHEADER; 

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the 
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx.
 */
typedef struct
{
    DWORD biSize; //The number of bytes required by the structure.
    LONG biWidth; //The width of the bitmap, in pixels.
    LONG biHeight; //The height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
    WORD biPlanes; //The number of planes for the target device. This value must be set to 1.
    WORD biBitCount; //The number of bits-per-pixel.
    DWORD biCompression; //The type of compression for a compressed bottom-up bitmap (top-down DIBs cannot be compressed).
    DWORD biSizeImage; //The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
    LONG biXPelsPerMeter; //The horizontal resolution, in pixels-per-meter, of the target device for the bitmap. An application can use this value to select a bitmap from a resource group that best matches the characteristics of the current device.
    LONG biYPelsPerMeter; //The vertical resolution, in pixels-per-meter, of the target device for the bitmap.
    DWORD biClrUsed; //The number of color indexes in the color table that are actually used by the bitmap. If this value is zero, the bitmap uses the maximum number of colors corresponding to the value of the biBitCount member for the compression mode specified by biCompression.
    DWORD biClrImportant; //The number of color indexes that are required for displaying the bitmap. If this value is zero, all colors are required.
} __attribute__((__packed__)) //打包屬性,確保clang不會對齊data type的內容(每個成員的第一個byte的位址都是4的倍數),導致memory出現間隙
BITMAPINFOHEADER; 

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx.
 */
typedef struct
{
    BYTE rgbtBlue; //The intensity of blue in the color.
    BYTE rgbtGreen; //The intensity of green in the color.
    BYTE rgbtRed; //The intensity of red in the color.
} __attribute__((__packed__))
RGBTRIPLE;
//宣告一個叫RGBTRIPLE的data type