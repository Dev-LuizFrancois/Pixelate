# Pixelate 

Developed by Luiz François.

The objective of this project is to implement a set of image transformations in *C* through the encoder/decoder written by Lode Vandevenne. The application writes and reads *PNG* images as a matrix of the 'Pixel' structure. 

This structure consists of the registration of the hue of three colors, red, green and blue, the well-known *RGB*. Through the manipulation of these tones, pixels and also matrix operations, Pixelate transforms the *PNG* images loaded in the system or even draws new *PNGs*.

This application was implemented by Luiz François as the project for the class of Languages and Programming Environments at the New University of Lisbon. It makes use of the Lode Vandevenne decoder (LodePNG version 20201017, Copyright (c) 2005-2020 Lode Vandevenne), as authorized by the author.

---

 ## 1. Compiling

The directory for the application is:

imageomatic/
|- lodepng.h
|- lodepng.c
|- Pixelate0.c
|- Pixelate.h
|- Pixelate.c
|- img/
|  |- example.png
|  |- colors.txt

Note that all the images you intend toi use in the application need to be stored inside the /img/ directory, and need to be of the *PNG* type. All the outuput images will also be stored in the /img/ directory, with the name of your choosing.

The compiling command is:
```
cc -std=c11 -o Pixelate PixelateMain.c Pixelate.c lodepng.c -lm
```

---

## 2. The Registers

The commande interpreter has two iamge registers, A and B. Every command cycle, the interpreter prints the name and the dimensions of the images saved in the registers. If A or B is empty, it will be printed "?" as the dimension. 

Example with 'example.png' stored in A and an empty B:

```
A: "img/example.png" = (width = 512, height = 512) B = (width = ?, height =?)
```

In the case of commands that synthesize an image instead of doing a transformation, there is no need for A or B to be loaded with a file. The result of all the commands is always stored on the A register. To writte the result as a *PNG*, use the = 'filename' command, as shown in the next session.

There are other commands that transform an image into another image. In these cases, only the A register is used: The image is fetched from A, the transformation is applied and stored back in A.

Finally, there are commands that combine two images to generate a new image. In this case the images of A and B are used, and the result is in A, as always. 

---

## 3. Commands Manual

The structure of arguments and call of commands to be used is as follows:

* `A arg`:

The `A` command loads the file `arg` into the A register. Note that only the name of the file should be passed in `arg`, and the file must be located in the /img/ directory.


* `B arg`:

The `B` command loads the file `arg` into the A register. Note that only the name of the file should be passed in `arg`, and the file must be located in the /img/ directory.


* `= arg`:

The `=` command creates or overwrites the file `arg`.png inside the /img/ directory. The `arg` value is the desired name for the file, and must not include the '.png' termination.



* `Q`:

Stops the execution and quits the application. Any image stored in A or B that was not saved is lost.


* `C`:

Copys the Pixel matrix of the A register and loads it into the B register.\

* `Z arg1 arg2`

The Z command plotts draws a circle by defining the x and y coordinates of each pixel through the `sin` and `cos` functions. It alsos uses the Bresenham's line algorithm to plot lines using the pixels, crossing the circle in varied patterns. The end effect looks like a web, and has a lot of complexity to it.

The arguments `arg1` and `arg2` are used to pass the desired dimensions (width and height) of the resulting image, that is loaded into the A register.

* `Y arg1 arg2`

This command uses the  parametric equation for a hypocycloid curve to plott a pixel image: each pixel coordinate is traced by a point P on a circle of radius b which rolls round a fixed circle of radius a. In this program, the parameters a and b are fixed at 250 and 150, forming the hypocycloid at the middle of the image. To visualize the pattern, its encouraged to zoom in the result image.

The arguments `arg1` and `arg2` are used to pass the desired dimensions (width and height) of the resulting image, that is loaded into the A register.


* `V arg1 arg2`

This command uses the  Helmut Vogel (1979) model to draw the sunflower pattern with pixels. Vogel defined the positions of the seeds, using polar coordinates (r, θ), and here the polar coordinates are used to determine the coordinate of each pixel. The command also utilizes a random generator of colors, so each pixel has a random *RGB*. To visualize the pattern, its encouraged to zoom in the result image.

The arguments `arg1` and `arg2` are used to pass the desired dimensions (width and height) of the resulting image, that is loaded into the A register.

* `X arg1 arg2 arg3`:

This command uses the `sin` and `cos` functions on `math.c` library to plott two parametric curves, making a beatiful image. The result is loaded into the A register. The arguments `arg1` and `arg2` are used to pass the desired dimensions (width and height) of the resulting image, that is loaded into the A register.

* `O arg1`:

The 'O' command applies a 'Poster' effect on the image of register A by degrading the max level of color tonalities of each pixel of its matrix.

The current possible tonalities are a product of 256 shades for each of the RGB contents, meaning `256 * 256 * 256`, or 'level 8' tonalities. The `arg1` is the nem level of tonalities possibilities you desire to the image, at can be from 0 to 8.

For instance, if 0 is passed, thhere can only be one tonality for each component, and the result will be a blank image. If 8 is passed, the image maintains its current level, and is not altered. If 6 is passed, there can be 64 different values of each component, so the resolution will be `64 * 64 * 64`.

* `U arg1`:

This command applies a Blur effect on the image loaded into the A register. The intensity of the filter is passed as argument `arg1`. Each pixel of the blurred image is obtained by averaging the square submatrix on the side 2 * level + 1, centered on the original pixel. The resulting image is loaded on the A register.


* `D arg1 arg2`:

This command plots a cartesian plan with center on the actual center of the image. Then, for each value of x, ranging from the beggining to the end width of the image, normalized to match the value on the cartesian plan, is calculated a random y value using `rand()`, and a line is drawn from the x axis to the point of y. Through this, we are able to visualize the distribution of the `rand()` function on C. 

The arguments `arg1` and `arg2` are used to pass the desired dimensions (width and height) of the resulting image, that is loaded into the A register.


* `W arg1 arg2`:

Through the use of a *sin* function, creates an image with gray levels simulating a drop of water. The gray level oscillates from the center of the image to the border of the image according to the following formula

The new image dimension is passed as argument, `arg1` being the width and `arg2` the height.

The `arg1` and `arg2` arguments represent the desired dimensions for the result image, and the `arg3` argument is the desired scale for plotting. For instance, if `arg3 = 100`, 100 pixels on the image will represent the value of 1.

* `T`:

This command applies the ordered dithering algorithm: an algorithm used to display a continuous image on a display of smaller color depth, characterized by noticeable crosshatch patterns in the result. In the case of Pixelate, the dithering converts the image on register A to a black and white scale.

The result is loaded into the A register.

* `N`:

Applies the 'negative' filter on the image on register A. The resulting image is loaded on the A register

The negative effect is achieved by inverting each color component of every pixel relative to the maximum color intensity `MAX_COLOR`.

* `G`:

Applies the 'grayscale' filter on the image on register A, meaning it takes each pixel on the matrix and averages the intensity of red, green and blue, creating a gray tonality. The result is loaded in A.

* `H`:

 Reduces the origianal *PNG* dimension, loaded in A, to half its size, while maintaining the image integrity. The result is loaded in A.

* `P arg1 arg2 arg3`:

Generates a new image consisting of a singular color. The color is passed as `arg1`, and can be either an HTML color hex code or one of the names defined on the 'colors.txt' file.

 The `arg2` and `arg3` represents the desired dimensions (width and heigth) of the new image.

The result is stored in the A register.


* `R`:

Rotates the image on the A register on 90 degrees. Can be applied multiple times for a greater rotation.


