/*
Pixelate0 module body

The detailed manual of use for all the commands on this interpreter can be found on the README file.
Note that each command has a specific set of arguments necessary for it to work.
*/

#include "Pixelate.h"


/*** TYPE Int2 ***/

Int2 int2Zero = {0, 0};
Int2 int2Error = {-1, -1};

Int2 int2(int x, int y)
{
	Int2 t = {x, y};
	return t;
}

/* Basic functions for the Int2 type */

bool int2Equals(Int2 a, Int2 b)
{
	return a.x == b.x && a.y == b.y;
}

bool int2LessThan(Int2 a, Int2 b)
{
	return a.x < b.x && a.y < b.y;
}

bool int2LessThanOrEqual(Int2 a, Int2 b)
{
	return a.x <= b.x && a.y <= b.y;
}

bool int2IsError(Int2 n)
{
	return int2Equals(n, int2Error);
}

bool int2Between(Int2 a, Int2 i, Int2 b)
{
	return int2LessThanOrEqual(a, i) && int2LessThan(i, b);
}

bool int2Between2(Int2 a, Int2 i, Int2 b)
{
	return int2LessThanOrEqual(a, i) && int2LessThanOrEqual(i, b);
}

Int2 int2Min(Int2 a, Int2 b)
{
	return int2(a.x<b.x?a.x:b.x, a.y<b.y?a.y:b.y);
}

Int2 int2Max(Int2 a, Int2 b)
{
	return int2(a.x<b.x?b.x:a.x, a.y<b.y?b.y:a.y);
}

Int2 int2Add(Int2 a, Int2 b)
{
	return int2(a.x + b.x, a.y + b.y);
}

Int2 int2Half(Int2 a)
{
	return int2(a.x / 2, a.y / 2);
}

double int2Distance(Int2 a, Int2 b)
{
	double x_dist = abs(a.x - b.x);
	double y_dist = abs(a.y - b.y);
	return sqrt(x_dist * x_dist + y_dist * y_dist);
}


/*** TYPE Pixel ***/

Pixel white = {255, 255, 255};
Pixel red = {255, 0, 0};
Pixel green = {0, 255, 0};
Pixel gray = {127, 127, 127};
Pixel blue = {0, 0, 255};
Pixel black = {0, 0, 0};

static int colorNormalize(int colorComponent)
{
	if( colorComponent < 0 )
		return 0;
	else if( colorComponent > MAX_COLOR )
		return MAX_COLOR;
	else
		return colorComponent;
}

Pixel pixel(int red, int green, int blue)
{
	Pixel p = {
			colorNormalize(red),
			colorNormalize(green),
			colorNormalize(blue)
	};
	return p;
}

Pixel pixelGray(int b)
{
	return pixel(b, b, b);
}


bool pixelEquals(Pixel a, Pixel b)
{
	return a.red == b.red
			&& a.green == b.green
			&& a.blue == b.blue;
}

/* Calculates the average between the red, green and blue shades, resulting in the gray color */
int pixelGrayAverage(Pixel p)
{
	return (p.red + p.green + p.blue)/3;
}



/*** TYPE Image ***/

Int2 imageLoad(String ficheiro, Image res)
{
	Byte *mem;
	unsigned int w, h;
	int error = lodepng_decode24_file(&mem, &w, &h, ficheiro);
	Int2 n = int2(w, h);
	Pixel *p = (Pixel *)mem;
	if( error != 0 )
		return int2Error;
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		res[i.x][i.y] = *p++;
	}
	free(mem);
	return n;
}

/* Converts the Pixel matrix into a PNG file, saved on the img directory. */
bool imageStore(String file, Image img, Int2 n)
{
	Byte *mem = malloc(MAX_X * MAX_Y * sizeof(Pixel));
	Pixel *p = (Pixel *)mem;
	Int2 i;
	for(i.y = 0; i.y < n.y; i.y++)
	for(i.x = 0; i.x < n.x; i.x++) {
		*p++ = img[i.x][i.y];
	}
	if( lodepng_encode24_file(file, mem, n.x, n.y) != 0 )	
		return false;
	free(mem);
	return true;
}


/* ERROR MESSAGES */

static void message(String mens, String arg)
{
	if( strcmp(arg, "") == 0 )
		printf("%s.\n", mens);
	else
		printf("%s \"%s\".\n", mens, arg);
}

static bool error(String err)
{
	printf("ERRO: %s!\n", err);
	return false;
}


/* COMMAND LINE */

/* Split input line into various Strings
according to the expected commands arguments */
int stringSplit(String s, String parts[], int maxParts)
{
	int i, j=0, p=0;
	bool insideQuotes = false;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		if( insideQuotes ) {
			if( s[i] == '"' || s[i] == '\n' ) {
				insideQuotes = false;
				if( p != 0 ) {
					parts[j][p] = '\0';
					j++;
					p=0;
				}
			}
			else {
				parts[j][p] = s[i];
				p++;
			}
		}
		else {
			if( s[i] == '"' )
				insideQuotes = true;
			else if( isspace(s[i]) ) {
				if( p != 0 ) {
					parts[j][p] = '\0';
					j++;
					p=0;
				}
			}
			else {
				parts[j][p] = s[i];
				p++;
			}
		}
	}
	return j;
}

void stringToUpperCase(String s)
{
	int i;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		s[i] = toupper(s[i]);
	}
}

#define INVALID_INT		-1

int stringToInt(String s)
{
	int i, n = 0;
	for( i = 0 ; s[i] != '\0' ; i++ ) {
		if( isdigit(s[i]) ) {
			n = n * 10 + (s[i] - '0');
		}
		else
			return INVALID_INT;
	}
	return n;
}


/* INTERPRETER */

#define INT_INFINITE	9999999
#define MAX_POSTER		8
#define MAX_PARTS   	6

static Image ia, ib, ic;
static Int2 na, nb, nc;
static String prefix, postfix;
static String a_file = "Empty";
static String b_file = "Empty";
String colorsFileName;

static bool validateCommand(String parts[], int nParts,
			String expectedCommand, int expectedNParts)
{
	if( strcmp(parts[0], expectedCommand) != 0 )
		return error("Invalid command");
	else if( nParts != expectedNParts )
		return error("Invalid number of arguments!");
	else
		return true;
}

static bool validateImage(char command) {
	if( command == 'A' && int2IsError(na) )
		return error("A image is empty. Load one to perform this action.");
	else if( command == 'B' && int2IsError(nb) )
		return error("B image is empty. Load one to perform this action.");
	else
		return true;
}

static bool validateIntX(String s) {
	int i = stringToInt(s);
	if( 0 < i && i <= MAX_X )
		return true;
	else
		return error("Valid integer expected for X.");
}

static bool validateIntY(String s) {
	int i = stringToInt(s);
	if( 0 < i && i <= MAX_Y )
		return true;
	else
		return error("Valid integer expected for Y.");
}

static bool validateInt(String s, int max) {
	int i = stringToInt(s);
	if( 0 <= i && i <= max )
		return true;
	else
		return error("Valid integer expected.");
}

static void run_command(String parts[], int nParts)
{
	String s;
	stringToUpperCase(parts[0]);
	char c0 = parts[0][0];
	char c1 = parts[0][1];
	switch( c0 ) {

		/* Writes the image in register A to a file with name passed as argument */
		case '=':
			if( validateCommand(parts, nParts, "=", 2)
			&& validateImage('A') ) {
				sprintf(s, "%s%s%s", prefix, parts[1], postfix);
				message("The image A was written to file ", s);
				strcpy(a_file, "Empty");
				if( !imageStore(s, ia, na) )
					error("The image could not be written to the file. Try again.");
			}
			break;

		/* Loads a png file inside de /img directory into the A register.
		The file name is passed as argument, without the '.png' extension. */
		case 'A':
			if( validateCommand(parts, nParts, "A", 2) ) {
				sprintf(s, "%s%s%s", prefix, parts[1], postfix);
				message("File uploaded to A: ", s);
				strcpy(a_file, s);
				na = imageLoad(s, ia);
				if( int2IsError(na) )
					error("File could not be reached");
			}
			break;

		/* Loads a png file inside de /img directory into the B register.
		The file name is passed as argument, without the '.png' extension. */
		case 'B':
			if( validateCommand(parts, nParts, "B", 2) ) {
				sprintf(s, "%s%s%s", prefix, parts[1], postfix);
				message("File uploaded to B: ", s);
				strcpy(b_file, s);
				nb = imageLoad(s, ib);
				if( int2IsError(nb) )
					error("File could not be reached");
			}
			break;

		/* Makes an exact copy of the pixels in A to the register B. */
		case 'C':
			if( validateCommand(parts, nParts, "C", 1)
			&& validateImage('B') ) {
				message(" Copied B -> A", "");
				strcpy(b_file, a_file);
				na = imageCopy(ib, nb, ia);
			}
			break;

		case 'Q':
			if( validateCommand(parts, nParts, "Q", 1) ) {
				exit(0);
			}
			break;

		/* Applies the 'Grayscale' filter to the image on register A. */
		case 'G':
			if( validateCommand(parts, nParts, "G", 1)
			&& validateImage('A') ) {
				message("Grayscale Filter ->", "");
				nc = imageGrayscale(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		/* Applies the 'Negative' filter to the image on register A. */	
		case 'N':
			if( validateCommand(parts, nParts, "N", 1)
			&& validateImage('A') ) {
				message("Negatice Filter -> A", "");
				nc = imageNegative(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		/* Reduces the origianal PNG dimension for its half size, while maintaining the image integrity. */	
		case 'H':
			if( validateCommand(parts, nParts, "H", 1)
			&& validateImage('A') ) {
				message("Cut in Half -> A", "");
				nc = imageHalf(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		/* Generates a new image consisting of a singular color, passed as an argument.
		The color can be passed either as a HTML color hex code or as one of the names
		defined on the 'colors.txt' file. */
		case 'P':
			if( validateCommand(parts, nParts, "P", 4)
			&& validateIntX(parts[2])
			&& validateIntY(parts[3]) ) {
				Int2 n = int2(stringToInt(parts[2]), stringToInt(parts[3]));
				message("Color Fill -> A", parts[1]);
				strcpy(a_file, "Color Fill");
				na = imagePaint(parts[1], n, ia);
			}
			break;

		/* Rotates the image on the A register on 90 degrees. Can be applied multiple times for a greater rotation. */	
		case 'R':
			if( validateCommand(parts, nParts, "R", 1)
			&& validateImage('A') ) {
				message("Rotate 90 -> A", "");
				nc = imageRotation90(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		/* Applies the 'Poster' filter to the image on register A. */
		case 'O':
			if( validateCommand(parts, nParts, "O", 2)
			&& validateInt(parts[1], MAX_POSTER)
			&& validateImage('A') ) {
				message("Poster Filter -> A", parts[1]);
				nc = imagePosterize(ia, na, stringToInt(parts[1]), ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		/* Applies the 'Dropplet' effect to the image on register A. */
		case 'W':
			if(validateCommand(parts, nParts, "W", 3)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2]) ) {
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("Dropplet Effect -> A", "");
				na = imageDroplet(n, ia);
			}
			break;

		/* Applies the 'Posterize' effect to the image on register A. Intensity of the filter is passed as argument*/
		case 'U':
			if( validateCommand(parts, nParts, "U", 2)
			&& validateImage('A')
			&& validateInt(parts[1], INT_INFINITE)
			 ) {
				message("Blur Filter -> A", parts[1]);
				nc = imageBlur(ia, na, stringToInt(parts[1]), ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		/* Draws the geometric form of a Hypocycloid */
		case 'Y':
			if( validateCommand(parts, nParts, "Y", 3)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2])){
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("Plotted a Hypocycloid", "");
				strcpy(a_file, "Result");
				na = hypocycloid(n, ia);
			}
			break;
		/* Draws two mirroed parametric curves */
		case 'X':
			if( validateCommand(parts, nParts, "X", 3)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2])){
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("Plotted a Parametric Curve", "");
				strcpy(a_file, "Result");
				na = parametricCurve(n, ia);
			}
			break;

		/* Applies a algorithm that simulates a web effect */
		case 'Z':
			if(validateCommand(parts, nParts, "Z", 3)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2])){
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("Web Pattern", "");
				strcpy(a_file, "Result");
				na = circlePlotting(n, ia);
			}
			break;

		/* Draws the Vogel Model */
		case 'V':
			if(validateCommand(parts, nParts, "V", 3)
			&& validateIntX(parts[1])
			&& validateIntY(parts[2])){
				Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
				message("The Voguel Model Simulation. Zoom in for cool visualization!", "");
				strcpy(a_file, "Result");
				na = voguelEquation(n, ia);
			}
			break;
		/* Plotts a graph with the distribution of the C rand() function */
		case 'D':
		if(validateCommand(parts, nParts, "D", 3)
		&& validateIntX(parts[1])
		&& validateIntY(parts[2])){
			Int2 n = int2(stringToInt(parts[1]), stringToInt(parts[2]));
			message("Plotted the rand() Distribution", "");
			strcpy(a_file, "Result");
			na = randDistribution(n, ia);
		}
		break;

		/* Applies the 'Dithering' effect to the image on register A. The intensity is passed as argument*/
		case 'T':
			if( validateCommand(parts, nParts, "T", 1)
			&& validateImage('A') ) {
				message("Dithering Effect -> A", parts[1]);
				nc = imageOrderedDithering(ia, na, ic);
				na = imageCopy(ic, nc, ia);
			}
			break;

		default:
			validateCommand(parts, nParts, "", 0);
			break;
	}
}

static void initialization(void)
{
	na = nb = int2Error;
	
	/* Paths for the images and colors directory, inside the main application directory */
	strcpy(prefix, "img/");
	strcpy(postfix, ".png");
	strcpy(colorsFileName, prefix);
	strcat(colorsFileName, "colors.txt");
}

/* 
Each cycle of commands, the system prints the current state of the A and B image registers. If any image is loaded onto the registers
the respective size will be printed out.
 */
static void img_regis_state(void)
{
	if( int2IsError(na) )
		printf("A: %s  = (width=?, heigth=?)", a_file);
	else
		printf("A: %s  = (width=%d, heigth=%d)",a_file, na.x, na.y);
	printf("   ");
	if( int2IsError(nb) )
		printf("B: %s  = (width=?, heigth=?)", b_file);
	else
		printf("B: %s  = (width=%d, heigth=%d)",b_file, nb.x, nb.y);
}

static void interpreter(void)
{
	String parts[MAX_PARTS];
	int nParts;
	String line;
	for(;;) {
		img_regis_state();
		printf("\n> ");
		if( fgets(line, MAX_STRING, stdin) == NULL ) {
			strcpy(line, "Z\n");
			printf("%s", line);
		}
		nParts = stringSplit(line, parts, MAX_PARTS);
#if 0	
		for( int i = 0 ; i < nParts ; i++ )
			printf("%s\n", parts[i]);
#endif
		if( nParts > 0 )
			run_command(parts, nParts);
	}
}

int main(void)
{
	initialization();
	interpreter();
	return 0;
}






