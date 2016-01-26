<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_1.jpg" width="800px" align="middle"/>


In this assignment you will implement a simple rasterizer, including features like supersampling, hierarchical transforms, and texture mapping with antialiasing. At the end, you'll have a functional vector graphics renderer that can take in modified SVG (Scalable Vector Graphics) files, which are widely used on the internet.

## Announcements
* Assignment 1 is due Wednesday February 10th at 11:59pm. Assignments which are turned in after 11:59pm are a full day late -- there are no late minutes or late hours.
* Note: You will write a webpage to present your results.  We will add more details about requirements for this write-up, and provide an *html* template for you to use.


## Getting set up

You can either [download](https://github.com/CS184-sp16/asst1_rasterizester/archive/master.zip) the zipped assignment straight to your computer or clone it from [GitHub](https://github.com/CS184-sp16/asst1_rasterizester) using the command 

    $ git clone https://github.com/CS184-sp16/asst1_rasterizester.git

Please consult this article on [how to build and submit assignments for CS 184](classarticle:4).


## Using the GUI

You can run the executable with the command

    ./rasterizester ../svg/basic/test1.svg

A flower should show up on your screen. After finishing Part 4, you will be able to change the viewpoint by dragging your mouse to pan around or scrolling to zoom in and out. Here are all the keyboard shortcuts available (some depend on you implementing various parts of the assignment):

|Key | Action|
|:-----:|------|
|`' '`  | return to original viewpoint|
|`'-'`  | decrease sample rate|
|`'='` | increase sample rate|
|`'Z'` | toggle the pixel inspector|
|`'P'` | switch between texture filtering methods on pixels|
|`'L'` | switch between texture filtering methods on mipmap levels|
|`'S'` | save a *png* screenshot in the current directory|
| `'1'-'9'`  | switch between svg files in the loaded directory|
| `'ESC'` | quit|

The argument passed to `rasterizester` can either be a single file or a directory containing multiple *svg* files, as in
    
    ./rasterizester ../svg/basic/

If you load a directory with up to 9 files, you can switch between them using the number keys 1-9 on your keyboard.

## Assignment structure

The assignment has 8 parts and 100 possible points. Some require only a few lines of code, while others are more substantial. 

1. Rasterizing lines (5 pts)
2. Rasterizing single-color triangles (10 pts)
3. Antialiasing triangles (20 pts)
4. Transforms (10 pts)
5. Barycentric coordinates (5 pts)
6. "Pixel sampling" for texture mapping (15 pts)
7. "Level sampling" with mipmaps for texture mapping (25 pts)
8. Draw something interesting! (10++ pts)

For each part, the potentially relevant locations in the code are marked with a C++ comment that looks like
    
    // Part 1: ...

There is a fair amount of code in the CGL library, which we will be using for future assignments. The relevant header files for this assignment are *vector2D.h*, *matrix3x3.h*, *color.h*, and *renderer.h*. In the discussion sections on Jan 27 and 28, we will give a tour of the starter code to help you get started with the assignment.

Here is a very brief sketch of what happens when you launch `rasterizester`: An `SVGParser` (in *svgparser.\**) reads in the input *svg* file(s), launches a OpenGL `Viewer` containing a `DrawRend` renderer, which enters an infinite loop and waits for input from the mouse and keyboard. DrawRend (*drawrend.\**) contains various callback functions hooked up to these events, but its main job happens inside the `DrawRend::redraw()` function. The high-level drawing work is done by the various `SVGElement` child classes (*svg.\**), which then pass their low-level point, line, and triangle rasterization data back to the three `DrawRend` rasterization functions.

### What you will turn in

You will submit your entire project directory in a *zip* file. This should include a *website* directory containing a web-ready assignment writeup in a file *index.html*. Most parts of the assignment have **Deliverables** specified, which will be *png* and *svg* files along with various textual descriptions. You should accumulate these deliverables into sections in your webpage writeup as you go through the assignment. There are a few open-ended deliverables, and the quality of your explanations is just as important as your output images, especially when you are trying for extra credit! We want you to demonstrate a lucid understanding of what you have implemented.

*Note: Do not squander all your hard work on this assignment by converting your *png* files into *jpg*s or any other format!* Leave the screenshots as they are saved by the `'S'` key in the GUI, otherwise you will introduce artifacts that will ruin your rasterization efforts. You can see these effects in the *jpg* images on this writeup page (which means, as a result, you should not use these for pixel-to-pixel comparisons!).


<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_7.jpg" width="800px" align="middle"/>

## Act I: In which you implement the bare bones

### Part 1 (warmup): Rasterizing lines (5 pts)
**Relevant lecture: 2**

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_8.jpg" width="800px" align="middle"/>


Part 1 is intended to be a simple warmup problem: fill in the `DrawRend::rasterize_line(...)` function in *drawrend.cpp*. The given coordinates `(x0,y0)` and `(x1,y1)` define the screen-space endpoints of a line of color `color`. Screen space coordinates range from `(0,0)` at the top left to `(width,height)` at the bottom right of the viewing window. Assume that screen sample position are centered at half-integer coordinates in this space. 

You may search the web for line-drawing algorithms and implement any you find (though write the code yourself). One option is to use [Bresenham's algorithm](http://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html). It is fine for you to rely on the existing implementation of `DrawRend::rasterize_point(...)` to write colors to the buffer. Make sure your algorithm only performs work proportional to the length of the line -- do not check every sample in the bounding box!

**Deliverables:** 

* Save a *png* of *svg/basic/test2.svg* with the default viewing parameters and with the pixel inspector centered on an interesting part of the scene.


### Part 2: Rasterizing single-color triangles (10 pts)
**Relevant lecture: 2**

Triangle rasterization is a core function in the graphics pipeline to convert input triangles into framebuffer pixel values. In Part 2, you will implement triangle rasterization using the methods discussed in lecture 2 to fill in the `DrawRend::rasterize_triangle(...)` function in *drawrend.cpp*. 

Notes: 

  * For now, ignore the `Triangle *tri` input argument to the function. We will come back to this in part 5.
  * You are encouraged but not required to implement the edge rules for samples lying exactly on an edge. 
  * Make sure the performance of your algorithm is no worse than one that checks each sample within the bounding box of the triangle.

When finished, you should be able to render many more test files, including those with rectangles and polygons, since we have provided the code to break these up into triangles for you. In particular, *basic/test3.svg*, *basic/test4.svg*, and *basic/test5.svg* should all render correctly. 

**Deliverables:** 

* Save a *png* of *svg/basic/test4.svg* with the default viewing parameters and with the pixel inspector centered on an interesting part of the scene.
* **Extra Credit:** Make your triangle rasterizer super fast (e.g., by factoring redundant arithmetic operations out of loops, minimizing memory access, and not checking every sample in the bounding box). Write about the optimizations you used. Use `clock()` to get timing comparisons between your naive and speedy implementations.



### Part 3: Antialiasing triangles (20 pts)
**Relevant lecture: 2**

Use supersampling to get rid of the jaggies, to render nicely antialiased edges on your triangles. The `sample_rate` parameter in `DrawRend` (adjusted using the `-` and `=` keys) tells you how many samples to use per pixel. 

You do not have to antialias points or lines.

You have some latitude to implement this part in whatever way you please. One piece of advice: to do this correctly, you will almost certainly need to keep track of `width * height * sample_rate` accumulated sample colors. 

Take care to make sure your antialiasing interfaces correctly with alpha blending (think about how two full-opacity triangles each half-covering some portion of a pixel could differ from one half-opacity triangle covering the whole pixel...). One sign that you may have broken this is if cracks start to appear along the edges of previously watertight triangles, and good file to test this on is *svg/basic/test4.svg*.

Your triangle edges should be noticeably smoother when using > 1 sample per pixel! You can examine the differences closely using the pixel inspector.

**Deliverables:** 

* Save a comparison *png* files of *svg/basic/test4.svg* with the default viewing parameters and sample rates 1, 4, and 16. Position the pixel inspector over an area that showcases the effect dramatically; for example, a very skinny triangle corner.
* Describe the new structs and functions you might have added to *drawrend.\** to implement antialiasing (there are multiple correct approaches).
* **Extra Credit:** Explore alternative antialiasing methods, such as jittered or low-discrepancy sampling. Create comparison images showing the differences between grid supersampling and your new pattern. Try making a scene that contains aliasing artifacts when rendered using grid supersampling but not when using your pattern.



### Part 4: Transforms (10 pts)
**Relevant lecture: 3**

Implement the three transforms in the *transforms.cpp* file according to the [SVG spec](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform). The matrices are 3x3 because they operate in homogeneous coordinates -- you can see how they will be used on instances of `Vector2D` by looking at the way the `*` operator is overloaded in the same file.

Additionally, implement `DrawRend::move_view(...)` in *drawrend.cpp*. This will allow you to pan and scroll using your cursor. Make sure you understand the matrix stack that transitions first from SVG to normalized device coordinates, then from NDC to screen space coordinates.

**Deliverables:** 

* Create a new *svg* file using geometric primitives and a hierarchical transform stack (at least two matrices deep) involving your new rotation, translation, and scaling matrices. [Here](http://tutorials.jenkov.com/svg/g-element.html) is one example of how the SVG `Group` element is used to make a transform stack. Create four copies of your *svg* file where you modify a transform to illustrate your grouping hierarchy.  Save your *svg* files in the *svg/my_examples/* directory and add *png* screenshots of your rendered drawing to the writeup.
* **Extra Credit:** Add an extra feature to the GUI. For example, you could make two unused keys to rotate the viewport. Save an example image to demonstrate your feature, and write about how you modified the SVG to NDC and NDC to screen-space matrix stack to implement it. 

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_6.jpg" width="800px" align="middle"/>

## Act II: In which you become a sampling guru

### Part 5 (warmup): Barycentric coordinates (5 pts)
**Relevant lecture: 4**

Familiarize yourself with the `ColorTri` struct in *svg.h*. Modify your implementation of `DrawRend::rasterize_triangle(...)` so that if a non-NULL `Triangle *tri` pointer is passed in, it computes barycentric coordinates of each sample hit and passes them to `tri->color(...)` to request the appropriate color. 

Implement the `ColorTri::color(...)` function in *svg.cpp* so that it returns this color. This function is very simple: it does not need to make use of any arguments besides `Vector2D xy` (the remaining arguments are for the texture mapped triangles). Note that this `color()` function plays the role of a very primitive shader. 

**Deliverables:** 

* Save a *png* of *svg/basic/test7.svg* with the default viewing parameters and sample rate 1.


### Part 6: "Pixel sampling" for texture mapping (15 pts)
**Relevant lecture: 4**

Familiarize yourself with the `TexTri` struct in *svg.h*. This is the primitive that implements texture mapping. For each vertex, you are given corresponding *uv* coordinates that index into the `Texture` pointed to by `*tex`. 

To implement texture mapping, `DrawRend::rasterize_triangle` should fill in the `psm` and `lsm` members of a `SampleData` struct and pass it to `tri->color(...)`. Then `TexTri::color(...)` should fill in the correct *uv* coordinates in the `SampleData` struct, and pass it on to `tex->sample(...)`. Then `Texture::sample(...)` should examine the `SampleData` to determine the correct sampling scheme.

The GUI toggles `DrawRend`'s `PixelSampleMethod` variable `psm` using the `'P'` key. When `psm == P_NEAREST`, you should use nearest-pixel sampling, and when `psm == P_LINEAR`, you should use bilinear sampling.

You can pass in dummy `Vector2D(0,0)` values for the `dx` and `dy` arguments to `tri->color`

For this part, just pass `0` for the `level` parameter of the `sample_nearest` and `sample_bilinear` functions.

For convenience, here is a list of functions you will need to modify:

1. `DrawRend::rasterize_triangle`
2. `TexTri::color`
3. `Texture::sample`
4. `Texture::sample_nearest`
5. `Texture::sample_bilinear`

**Deliverables:** 

* Test the *svg* files in the *svg/texmap/* directory. Use the pixel inspector to find a good example of where bilinear sampling clearly defeats nearest sampling. Hint: you want the texture to be magnified in the rendered image.  Save four screenshots to show comparisons between nearest and bilinear at 1 sample per pixel and at 16 samples per pixel. Comment on the relative differences.



### Part 7: "Level sampling" with mipmaps for texture mapping (25 pts)
**Relevant lecture: 4**

Finally, you will add support for sampling different `MipMap` levels. The GUI toggles `DrawRend`'s `LevelSampleMethod` variable `lsm` using the `'L'` key. 

* When `lsm == L_ZERO`, you should sample from the zero-th `MipMap`, as in Part 6. 
* When `lsm == L_NEAREST`, you should compute the nearest appropriate `MipMap` level using the one-pixel difference vectors `du` and `dv` and pass that level as a parameter to the nearest or bilinear sample function. 
* When `lsm == L_LINEAR`, you should find the appropriate `MipMap` level and do full trilinear sampling by getting two bilinear samples from adjacent levels and computing a weighted sum.

Implement `Texture::get_level` as a helper function. This is the trickiest math in the whole assignment -- make sure to read the relevant slides in Lecture 4 carefully.

For convenience, here is a list of functions you will need to modify:

1. `DrawRend::rasterize_triangle`
2. `TexTri::color`
3. `Texture::sample`
4. `Texture::get_level`

**Deliverables:** 

* There are large number of sampling schemes available to you now: you can adjust pixel sampling, level sampling, and samples per pixel all independent of one another! Pull some *png* images from the internet and create your own *svg* files to demonstrate the strengths and weaknesses of various techniques at different zoom levels. You can take existing files in *svg/texmap/* and replace the `texture` filename to try out new images. A good starting place for this is *svg/texmap/test7.png*. Show at least one example (using a *png* file you find yourself) comparing all four combinations of one of `L_ZERO` and `L_NEAREST` with one of `P_NEAREST` and `P_BILINEAR` at a zoomed out viewpoint.
* **Extra Credit:** Implement [anisotropic filtering](https://en.wikipedia.org/wiki/Anisotropic_filtering) or [summed area tables](https://en.wikipedia.org/wiki/Summed_area_table). Show comparisons of your method to nearest, bilinear, and trilinear sampling. Use `clock()` to measure the relative performance of the methods.


### Part 8: Draw something interesting! (10++ pts)

Use your newfound powers to render something fun and attractive. You can look up the svg specifications online for matrix transforms and for Point, Line, Polyline, Rect, Polygon, and Group classes. The ColorTri and TexTri are our own inventions, so you can intuit their parameters by looking at the *svgparser.cpp* file. Some ideas:

* Try to draw something "by hand" on graph paper, and manually transfer it to coordinates in the *svg* file.
* Write a program to procedurally generate some geometric patterns.  For example, we wrote some simple programs to generate the texture mapped *svg* files in the *svg/texmap/* directory as well as the color wheel in *svg/basic/test7.svg*. 
* Write a program that thresholds an input photo and generates a triangle mesh from it.

We will consider aesthetics, so it's worthwhile to consider factors like composition, color, etc.

**Deliverables:** 

* Give us your best *svg* file and a *png* screenshot of it! Also include a description of what you were trying to achieve, and how you created your *svg* file. 
* **Extra Credit:** Flex your right or left brain -- either show us your artistic side, or generate awesome procedural patterns with code. This could involve a lot of programming either inside or outside of the codebase! If you write a script to generate procedural *svg* files, include it in your submission and briefly explain how it works. 



## Tips

  * Start early! 
  * Start assembling your webpage early to make sure you have a handle on how to edit the html code to insert images and format sections.
  * The earlier you finish the basic requirements of the assignment, the more time you'll have to choose your favorite parts and implement some extra credit extensions!
