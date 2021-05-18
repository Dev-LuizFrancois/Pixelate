/*
	Pixelate by Luiz Francois

	A detailed explation for the algorithms here used is in the README file, under the 3. session.

*/

#include "Pixelate.h"

/* AUXILIARY FUNCTIONS */

/* String to upper case */
void upperCase(String s)
{
	int i;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		s[i] = toupper(s[i]);
	}
}

/* Generates random intensity values for the RGB component opf the pixel */
Pixel randomColor()
{
	return pixel(rand() % (MAX_COLOR + 1),rand() % (MAX_COLOR + 1),rand() % (MAX_COLOR + 1));
}

/* Bresenham's line algorithm */
void linePlotting(Int2 p1, Int2 p2, Image res)
{

    int dx =  abs(p2.x-p1.x);
    int sx = (p1.x<p2.x) ? 1 : -1;
    int dy = -abs(p2.y-p1.y);
    int sy = (p1.y<p2.y) ? 1 : -1;
    int err = dx+dy;
	int e2;
    while (true) { 
        res[p1.x][p1.y] = randomColor();
        if (p1.x == p2.x && p1.y == p2.y) break;
        e2 = 2*err;
        if (e2 >= dy){ /* e_xy+e_x > 0 */
            err += dy;
            p1.x += sx;
        }
        if (e2 <= dx){ /* e_xy+e_y < 0 */
            err += dx;
            p1.y += sy;
        }
	}
}


/* Reduces the intensity of a RGB component by the int level */
int nivelate (int value, int level){
 	value = value + level/2;
    return value - (value%level);
}

/* Applies a certain scale to a cartesian plane simulation on the image */
double scale_value(int scale, int max_val,  int value)
{
	return (double)value/(double)scale;
}



/* MAIN FUNCTIONS */


Int2 imageCopy(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = img[i.x][i.y];
	}
	return n;
}

Int2 imagePaint(String color, Int2 n, Image res)
{
	Pixel paint;
    char *line = NULL;
	bool found = false;
	size_t len = 0;
	ssize_t read;
	unsigned long hex_code;

	/* File containing all the default colors the interpreter can identify by name */
	FILE *file = fopen(colorsFileName, "r");

	while(((read = getline(&line, &len, file)) != -1) && !found){
		char* code = strsep(&line, " ");
		char* name = strsep(&line, "\n");

		if(strcmp(name, color)==0){
			int r, g, b;
			sscanf(code, "%02x%02x%02x", &r, &g, &b); //Hex to RGB converter
			paint = pixel(r, g, b);
			found = true;
		}
    }
	fclose(file);

	/* If the passes color wasnt on the list,
	 it refers to all hex code, and can be applied directly */
	if(!found){
		int r, g, b;
		upperCase(color);
		sscanf(color, "%02x%02x%02x", &r, &g, &b);
		paint = pixel(r, g, b);
	}
		

	/* Filling each Pixel with the identified color */
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = paint;
	}
	return n;
}

Int2 imageNegative(Image img, Int2 n, Image res)
{
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		Pixel aux_pix = img[i.x][i.y];
		//Inverts the tonality of each RGB component
		res[i.x][i.y] = pixel(MAX_COLOR - aux_pix.red, MAX_COLOR - aux_pix.green, MAX_COLOR - aux_pix.blue);
	}
	return n;
}

Int2 imageDroplet(Int2 n, Image res)
{
	Int2 i;
	Image img;
	Int2 centre = int2Half(n);

		for(i.y = 0; i.y < n.y; i.y++)
		for(i.x = 0; i.x < n.x; i.x++) {

			
			Int2 pix_coord = int2(i.x,i.y);
			double dxy = int2Distance(centre, pix_coord);

			int colour = 0.7* MAX_COLOR + 0.3 * sin(dxy / 20) * MAX_COLOR;

			Pixel pixelColor = pixelGray(colour);


			res[i.x][i.y] = pixelColor;

		}


	return n;
}

Int2 circlePlotting(Int2 n, Image res)
{

	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = white;
	}
	int radius = 254;
	Int2 center;
	Int2 circle_p;
	Int2 opposite_p;
	circle_p.x = 0;
	center.x = (int)(n.x/2);
	center.y = (int)(n.y/2);

	/* Draws the circl;e with the lines pattern multiple times to achieve a web like image */
	while(radius > (int)radius/6.76){
		for(double t = 0; t <= 360; t += 0.1)
		{	
			/*  Drawing the circle */
			circle_p.x = radius * cos((t * M_PI )/ 180) + (int)(n.x/2);
			circle_p.y = radius * sin((t * M_PI)/180) + (int)(n.y/2);
			res[(int)circle_p.x][(int)circle_p.y] = randomColor();

			/* Each time the theta angle (int t) reaches a value divisible to 36, meaning 6 times,
			it draws multiple lines with the radius length, to points alongisde the circunference */
			if((int)t%36 == 0){
				opposite_p.x = radius * cos(((t-90) * M_PI )/ 180) + (int)(n.x/2);
				opposite_p.y = radius * sin(((t-90) * M_PI)/180) + (int)(n.y/2);
				linePlotting(opposite_p, circle_p, res);

				opposite_p.x = radius * cos(((t+90) * M_PI )/ 180) + (int)(n.x/2);
				opposite_p.y = radius * sin(((t+90) * M_PI)/180) + (int)(n.y/2);
				linePlotting(opposite_p, circle_p, res);

				opposite_p.x = radius * cos(((t+180) * M_PI )/ 180) + (int)(n.x/2);
				opposite_p.y = radius * sin(((t+180) * M_PI)/180) + (int)(n.y/2);
				linePlotting(opposite_p, circle_p, res);

				opposite_p.x = radius * cos(((t+360) * M_PI )/ 180) + (int)(n.x/2);
				opposite_p.y = radius * sin(((t+360) * M_PI)/180) + (int)(n.y/2);
				linePlotting(opposite_p, circle_p, res);
			}
		}
		radius = (int)radius/1.27; //reduction factor
	}

	return n;
}

/* Plotts two parametric curves, one mirroing the other. */
Int2 parametricCurve(Int2 n, Image res)
{

	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = white;
	}

	/* The x and y coordinates of both curves */
	long double lx,ly,zx,zy;
	/* Parameters of the polar coordinates */
	long double a = 140;
	long double b = 175;
	int limit = (n.y > n.x) ? n.x/2 : n.y/2;

	for(int t = 0; t < pow(limit, 2); t++) {
		lx = (a*(sin((175*t))));
		ly = b*sin((105*t));
		zx = (a*(cos((175*t))));
		zy = b*cos((105*t));;
		res[(int)lx + (int)(n.x/2)][(int)ly + (int)(n.y/2)] = randomColor();
		res[(int)zx + (int)(n.x/2)][(int)zy + (int)(n.y/2)] = randomColor();
	}
	return n;
}

Int2 hypocycloid(Int2 n, Image res)
{

	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {

		res[i.x][i.y] = white;

	}

	/* Hypocycloid angular parameter scaled to the pixel size (1-50) */
	long double a = 250;
	long double b = 150;
	long double lx,ly;
	int limit = (n.y > n.x) ? n.x/2 : n.y/2;
	
	for(int t = 0; t < pow(limit, 2); t++) {
		lx = ((a-b)*cos(t))+(b*cos(((a/b)-1)*t));
		ly=(((a-b)*sin(t))-(b*sin(((a/b)-1)*t)));
		res[(int)lx + (int)(n.x/2)][(int)ly + (int)(n.y/2)] = randomColor();
	}
	return n;
}

/* Convert each pixel to grayscale: ((r+g+b)/3) */
Int2 imageGrayscale(Image img, Int2 n, Image res)
{
	

	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {
        res[i.x][i.y] = pixelGray(pixelGrayAverage(img[i.x][i.y]));
    }
    return n;
}

Int2 voguelEquation(Int2 n, Image res)
{

	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {

		res[i.x][i.y] = white;

	}

	Int2 vogel;
	long double r,t;
	int limit = (n.y > n.x) ? n.x/2 : n.y/2;
	for(int v = 0; v < pow(limit, 2) - 100; v++)
	{

		r = sqrt(v);
		t = (2*M_PI)/((sqrt(5)+1)/2+1)*v;

		vogel.x= (int)(r*cos(t)) + (int)(n.x/2);
		vogel.y= (int)(-r*sin(t)) + (int)(n.x/2);

		res[vogel.x][vogel.y] = randomColor();
	}

	
	return n;
}

Int2 imageBlur(Image img, Int2 n, int nivel, Image res)
{

	Int2 i;

		/* iterates over the original image pixels */
		for(i.y = 0; i.y < n.y; i.y++) 
		for(i.x = 0; i.x < n.x; i.x++) {

			Pixel current = img[i.x][i.y];

			Int2 j;

			int sumOfRed = 0;
			int sumOfBlue = 0;
			int sumOfGreen = 0; 
			int count =0;

			for(j.x = i.x - nivel; j.x <= i.x + nivel; j.x++) //Iterate over the submatrix
			for(j.y = i.y - nivel; j.y<= i.y + nivel; j.y++){

				//Verifies that the submatrix is withing the image dimensions
				if(j.x >=0 && j.x<= n.x && j.y >= 0 && j.y<= n.y){ 


				Pixel aux = img[j.x][j.y];

				sumOfRed += aux.red;
				sumOfBlue += aux.blue;
				sumOfGreen += aux.green;

				count++;
			}


			}

			current.red = sumOfRed/count;
			current.blue = sumOfBlue/count;
			current.green = sumOfGreen/count;

			res[i.x][i.y] = current;

		}

	return n;
}

Int2 randDistribution(Int2 n, Image res)
{

	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {
		Pixel aux = res[i.x][i.y];
        /*Plotting the cartesian plan lines*/
		if(i.y == n.y/2 || i.x == n.x/2)
			res[i.x][i.y] = black;
		else
			res[i.x][i.y] = white;
    }

	/* Point used to plot a line, representing
	 the value given by the rand() function */
	Int2 aux_point;
	aux_point.y = (int)(n.y/2);
    for(i.x = 0; i.x < n.x; i.x++){
		aux_point.x = i.x;
		i.y = (int)(rand() % (n.y));
		linePlotting(aux_point, i, res);
	}
	return n;
}

Int2 imageRotation90(Image img, Int2 n, Image res)
{
	Int2 i;

	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.y][n.x -1-i.x] = img[i.x][i.y];
	}
		return int2(n.y,n.x);
}

Int2 imagePosterize(Image img, Int2 n, int factor, Image res)
{
	Int2 i;
    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {

		/* The tonality reduction factor, meaning the difference in
		possibilities for the RGB components of each pixel */
		int fac_diff = pow(2, (log2(MAX_COLOR) - factor));

		/* Nivelates the color component according to the calculated factor */ 
		int r = nivelate(img[i.x][i.y].red, fac_diff);
		int g = nivelate(img[i.x][i.y].green, fac_diff);
		int b = nivelate(img[i.x][i.y].blue, fac_diff);

        res[i.x][i.y] = pixel(r, g, b);
    }
    return n;
}

Int2 imageHalf(Image img, Int2 n, Image res)
{
	Int2 i;
    for(i.y = 0; i.y < n.y; i.y+=2)
    for(i.x = 0; i.x < n.x; i.x+=2) {
        res[i.x/2][i.y/2] = img[i.x][i.y];
    }
    return n;
}

Int2 imageOrderedDithering(Image img, Int2 n, Image res)
{
	#define INDEX_SIDE  8
	Byte indexMatrix[INDEX_SIDE][INDEX_SIDE] = {
					{ 0, 32,  8, 40,  2, 34, 10, 42}, 
					{48, 16, 56, 24, 50, 18, 58, 26},
					{12, 44,  4, 36, 14, 46,  6, 28},
					{60, 28, 52, 20, 62, 30, 54, 22},
					{ 3, 35, 11, 43,  1, 33,  9, 41},
					{51, 19, 59, 27, 49, 17, 57, 25},
					{15, 47,  7, 39, 13, 45,  5, 37},
					{63, 31, 55, 23, 61, 29, 53, 21}
			};

	Int2 i;
		for(i.y = 0; i.y < n.y; i.y++)
		for(i.x = 0; i.x < n.x; i.x++) {

			Pixel current = img[i.x][i.y];
			int associatedValue = indexMatrix[i.x%8][i.y%8];

			int pixelGrayA = pixelGrayAverage(current);

			double intensity = pixelGrayA/4.0;


			if(intensity > associatedValue){

				res[i.x][i.y] = white;
			}
			else{

				res[i.x][i.y] = black;
			}
		}




	return n;
}