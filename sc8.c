/* sc8.c
 *
 * Convert images to MSX Screen 8.
 *
 * Compile:
 *  gcc -O2 -Wall -Wextra `Wand-config --cflags` -s -o sc8 sc8.c \
 *    `Wand-config --libs`
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <wand/magick_wand.h>

typedef unsigned char BYTE;
typedef BYTE          SC8Image[212][256];

static void
usage(void)
{
    fprintf(stderr,
        "usage: sc8 [-b color] [-d 1|2|3] [-p] input output\n\n"
        " -b color           Set the background color to color, "
        "defaults to black\n"
        " -d 1|2|3           Set the dither type to either:\n"
        "                     1: 2x2, 2: 4x4 (default), 3: 8x8\n"
        " -p                 Generate a preview image, 'output.png'\n\n");
    exit(1);
}

static void
exit_error(char *message)
{
    fputs(message, stderr);
    exit(1);
}

int
main(int argc, char **argv)
{
    BYTE           sc8header[7] =
        { 0xfe, 0x00, 0x00, 0x00, 0xd4, 0x00, 0x00 };
    SC8Image       sc8;
    char           dither_type[11], preview_name[1024];
    char          *input_name, *output_name, *bg_color = "black";
    FILE          *fout;
    size_t         width, height, qdepth;
    int            c, x, y, dopt = 2, popt = 0;
    MagickWand    *mw;
    PixelIterator *pi;
    PixelWand     *pw, **pixelrow;

    /* parse arguments */
    while ((c = getopt(argc, argv, "b:d:hp")) != -1) {
        switch (c) {
        case 'b':
            bg_color = optarg;
            break;
        case 'd':
            dopt = atoi(optarg);
            if (dopt < 1 || dopt > 3)
                exit_error("error: unknown dither type\n");
            break;
        case 'h':
            usage();
        case 'p':
            popt = 1;
            break;
        }
    }

    if (argc - optind != 2)
        usage();
    input_name = argv[optind++];
    output_name = argv[optind++];

    /* Initialise magick wand */
    MagickWandGenesis();
    if ((mw = NewMagickWand()) == NULL)
        exit_error("error: can't initialise MagickWand\n");

    /* Read the input image */
    if (!MagickReadImage(mw, input_name))
        exit_error("error: can't read input image\n");

    /* resize it to 256x212 */
    width = MagickGetImageWidth(mw);
    height = MagickGetImageHeight(mw);
    if (width != 256 || height != 212) {
        double aspect = (double) width / height;
        if (width > height)
            width = 256, height = 212 / aspect;
        else if (height > width)
            height = 212, width = 256 * aspect;
        pw = NewPixelWand();
        if (!PixelSetColor(pw, bg_color))
            exit_error("error: unknown background color\n");
        MagickSetImageBackgroundColor(mw, pw);
        MagickResizeImage(mw, width, height, LanczosFilter, 1);
        MagickExtentImage(mw, 256, 212, -((256 - width) >> 1),
              -((212 - height) >> 1));
        pw = DestroyPixelWand(pw);
    }

    /* Convert it to 332 palette */
    switch (dopt) {
    case 1:
        strcpy(dither_type, "o2x2,8,8,4");
        break;
    case 2:
        strcpy(dither_type, "o4x4,8,8,4");
        break;
    case 3:
        strcpy(dither_type, "o8x8,8,8,4");
        break;
    }
    MagickOrderedPosterizeImage(mw, dither_type);

    /* Convert it to a screen 8 image */
    MagickGetQuantumDepth(&qdepth);
    if ((pi = NewPixelIterator(mw)) == NULL)
        exit_error("error: can't create a pixel iterator\n");

    for (y = 0; y < 212; ++y) {
        pixelrow = PixelGetNextIteratorRow(pi, &width);
    for (x = 0; x < 256; ++x) {
        sc8[y][x] =
            (PixelGetBlueQuantum(pixelrow[x]) >> (qdepth - 2)) +
            (PixelGetRedQuantum(pixelrow[x]) >> (qdepth - 3) << 2) +
            (PixelGetGreenQuantum(pixelrow[x]) >> (qdepth - 3) << 5);
    }
    }
    DestroyPixelIterator(pi);

    /* write a png if preview option is specified */
    if (popt) {
        strncpy(preview_name, output_name, 1024);
        strncat(preview_name, ".png", 1024);
        preview_name[1023] = '\0';
        MagickSetCompressionQuality(mw, 95);
        if (!MagickWriteImage(mw, preview_name))
            exit_error("error: can't write preview image\n");
    }
    mw = DestroyMagickWand(mw);
    MagickWandTerminus();

    /* write the .sc8 file */
    if ((fout = fopen(output_name, "wb")) == NULL ||
        fwrite(&sc8header, sizeof(sc8header), 1, fout) != 1 ||
        fwrite(sc8, sizeof(SC8Image), 1, fout) != 1)
        exit_error("error: can't write output file\n");

    fclose(fout);
    return EXIT_SUCCESS;
}
