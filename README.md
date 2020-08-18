# Computer-graphics-lab1
## Generate transformations on basic shapes
## File Organization
 All the files are divided into two folders, One is images, which contains all the images. The other
is files, where you can see txt files stored separately
## Additional Shapes
 We add hexagon, octagon, circle, and one additional shape Star, the pictures are shown below.
For every shape, we define two constructors. One is a given center point and a reasonable length.
The center defaults to (0, 0), and the distance from each vertex to the center of the graph is set
by the given length. Another constructor is set the required vertices of a given graph. This
constructor can allow irregular polygons to appear and can also allow we to draw a new graph by
mouse manipulation. We will take the additional shape Star for example.
## Matrix definition
 For overloaded matrix addition(operator +) and multiplication(operator *), we implement arbitrary
matrix calculations of the same size. For inverse operations(operator !), we assume that all that
is needed to be calculated are 3 * 3 matrices, which will make the algorithm more concise.
## Simple operators
 All the data (coordinates) can be seen in SimpleOps.txt, here we show the final result graphs.
Fractals
 For each fractal we construct, we will give two pictures, one is showing the transformation we
apply in Geometry Viewer, and the other one is showing the final result in IFSViewer. Details
can be seen in BasicFractals.txt and pictures can be seen in file images.
