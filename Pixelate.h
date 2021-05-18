/*
    Pixelate module interface
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "lodepng.h"


/*** TYPE String ***/

#define MAX_STRING		256
typedef char String[MAX_STRING];


/*** TYPE Int2 ***/

/* This type is used to represent the two coordinates of a pixel in an image */
typedef struct {
	int x, y;
} Int2;

extern Int2 int2Zero, int2Error;

extern Int2 int2(int x, int y);
extern bool int2Equals(Int2 a, Int2 b);
extern bool int2LessThan(Int2 a, Int2 b);
extern bool int2LessThanOrEqual(Int2 a, Int2 b);
extern bool int2IsError(Int2 n);
extern bool int2Between(Int2 i, Int2 a, Int2 b);
extern bool int2Between2(Int2 i, Int2 a, Int2 b);
extern Int2 int2Min(Int2 a, Int2 b);
extern Int2 int2Max(Int2 a, Int2 b);
extern Int2 int2Add(Int2 a, Int2 b);
extern Int2 int2Half(Int2 a);
extern double int2Distance(Int2 a, Int2 b);


/*** TYPE Pixel ***/

#define MAX_COLOR			255

typedef unsigned char Byte;

/*
The Pixel struct stores 3 bytes, each representing the shades of the colors red, green and blue. Pixelate uses the standart 256 value capacity
for the intensity of the RGB colors. 
*/
typedef struct {
	Byte red, green, blue;
} Pixel;

extern Pixel white, red, green, gray, blue, black; // Pre-defined colors

extern Pixel pixel(int red, int green, int blue);
extern Pixel pixelGray(int b);
extern bool pixelEquals(Pixel a, Pixel b);
extern int pixelGrayAverage(Pixel p);


/*** TYPE Image ***/

/* Maximum pixel height and width of the input and output images png. */
#define MAX_X			1024
#define MAX_Y			1024

/* Matrix containing all the pixels that represent the image. */
typedef Pixel Image[MAX_X][MAX_Y];

typedef double (DoubleFun)(double);

extern String colorsFileName; // file that contains color names and hexadecimal RGB codes

/* Registers A and B handling */
extern Int2 imageLoad(String ficheiro, Image res);
extern bool imageStore(String ficheiro, Image img, Int2 n);

/* Image manipulation functions */
extern Int2 imageCopy(Image img, Int2 n, Image res);
extern Int2 imageGrayscale(Image img, Int2 n, Image res);
extern Int2 imageNegative(Image img, Int2 n, Image res);
extern Int2 imageHalf(Image img, Int2 n, Image res);
extern Int2 imagePaint(String cor, Int2 n, Image res);
extern Int2 imageRotation90(Image img, Int2 n, Image res);
extern Int2 imagePosterize(Image img, Int2 n, int factor, Image res);
extern Int2 imageDroplet(Int2 n, Image res);
extern Int2 imageBlur(Image img, Int2 n, int nivel, Image res);
extern Int2 imageOrderedDithering(Image img, Int2 n, Image res);

/* Graph plotting and image creation functions */
extern Int2 circlePlotting(Int2 n, Image res);
extern Int2 voguelEquation(Int2 n, Image res);
extern Int2 randDistribution(Int2 n, Image res);
extern Int2 hypocycloid(Int2 n, Image res);
extern Int2 parametricCurve(Int2 n, Image res);
extern Int2 recursiveBraid(Int2 n, Image res);

