# Questions

## What's `stdint.h`?

It is a header that suggests and declares the width of integers.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Defines the length of the integer, and defines whether it is signed or unsigned.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1 byte, 4 bytes, 4 bytes, 2 bytes.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

B and M

## What's the difference between `bfSize` and `biSize`?

bfSize includes BITMAPFILEHEADER and BITMAPINFOHEADER, biSize is only the size of the image.

## What does it mean if `biHeight` is negative?

It means that the Height is calculated top down, starting from the top-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biSize

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

Since the file might not exist.

## Why is the third argument to `fread` always `1` in our code?

Because each pixel is of sized 1 byte.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

To reset the write pointer to an offset from a certain location.

## What is `SEEK_CUR`?

Current position of the pointer.
