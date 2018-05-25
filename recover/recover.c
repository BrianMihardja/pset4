#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t BYTE; //uint8_t is unsigned integer size 8 bits.

int main(int argc, char *argv[])
{
        if (argc != 2)
    {
        fprintf(stderr, "Please input card.raw\n");
        return 1;
    }

    // open input file
    FILE *rawfile = fopen(argv[1], "r");
    if (rawfile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    BYTE buffer[512]; //

    int imgcounter = 0; //total images found.
    int jpegwritestart = 0;
    // FILE *jpegimg = NULL;
    char jpegname[8];
    FILE *jpegimg = NULL;

    fread(buffer, 512, 1, rawfile);

    //reading the file as long as it returns 1 set of 512 bytes.
    while(fread(buffer, 512, 1, rawfile) == 1)
    {
        //checking the 4 start bytes of JPEG
        if(buffer[0] == 0xff
            && buffer[1] == 0xd8
            && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
        {
            if(jpegwritestart != 0)
            {
                fclose(jpegimg);
            }

            jpegwritestart = 1; //signals start of writing JPEG.

            //opens up a new file with name 'imgcounter.jpg'
            sprintf(jpegname, "%03i.jpg", imgcounter);
            jpegimg = fopen(jpegname, "w");
            fwrite(buffer, 512, 1, jpegimg);
            imgcounter++;

            printf("start of JPEG: %04x ", buffer[0]);
            printf("%04x ", buffer[1]);
            printf("%04x ", buffer[2]);
            printf("%04x\n", buffer[3]);
        }

        while((buffer[0] != 0xff
            || buffer[1] != 0xd8
            || buffer[2] != 0xff
            || (buffer[3] & 0xf0) != 0xe0)
            && jpegwritestart != 0)
            {
                fwrite(buffer, 512, 1, jpegimg);
                if(fread(buffer, 512, 1, jpegimg) == 0)
                {
                    break;
                }
            }



    }

    printf("total images: %i\n", imgcounter);

}