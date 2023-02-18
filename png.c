#include <png.h>
#include <cstdlib>

int writeImage(char* filename, int width, int height, unsigned char *buffer, char* title)
{
   int code = 0;
   FILE *fp = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
   png_bytep row = NULL;

   // Open file for writing (binary mode)
   fp = fopen(filename, "wb");
   if (fp == NULL) {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
      goto finalise;
   }

   // Initialize write structure
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL) {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalise;
   }

   // Initialize info structure
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      fprintf(stderr, "Could not allocate info struct\n");
      code = 1;
      goto finalise;
   }

   // Setup Exception handling
   if (setjmp(png_jmpbuf(png_ptr))) {
      fprintf(stderr, "Error during png creation\n");
      code = 1;
      goto finalise;
   }

   png_init_io(png_ptr, fp);

   // Write header (8 bit colour depth)
   png_set_IHDR(png_ptr, info_ptr, width, height,
         8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   // Set title
   if (title != NULL) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }

   png_write_info(png_ptr, info_ptr);

   // Allocate memory for one row (3 bytes per pixel - RGB)
   row = (png_bytep) malloc(3 * width * sizeof(png_byte));

   // Write image data
   int x, y;
   for (y=0 ; y<height ; y++) {
      for (x=0 ; x<width ; x++) {
         row[x*3+0] = buffer[4*(y*width+x)+0];
         row[x*3+1] = buffer[4*(y*width+x)+1];
         row[x*3+2] = buffer[4*(y*width+x)+2];
      }
      png_write_row(png_ptr, row);
   }

   // End write
   png_write_end(png_ptr, NULL);

   finalise:
   if (fp != NULL) fclose(fp);
   if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
   if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
   if (row != NULL) free(row);

   return code;
}

void genImage(int width, int height, unsigned char *buffer) {
  for(int y=0;y<height;y++) {
    for(int x=0;x<width;x++) {
      float fx = (float)x/width;
      float fy = (float)y/height;

      if(x%10==0) {
        buffer[4*(y*width+x)+0]=255;
      }

      if(y%10==0) {
        buffer[4*(y*width+x)+1]=255;
      }

      if(fx>0.5 || fy>0.25) {
        buffer[4*(y*width+x)+2]=255;
      }

      buffer[4*(y*width+x)+0]=(unsigned char)(255*fx);
      buffer[4*(y*width+x)+1]=(unsigned char)(255*fy);
      buffer[4*(y*width+x)+2]=255; //(unsigned char)(255*fx*fy);
    }
  }
}
