## build

<kbd>draw\_planar</kbd> gets build for Qt5 by executiong <kbd>build5</kbd>.

<kbd>draw\_planar</kbd> gets build for Qt6 by executiong <kbd>build6</kbd>.

[draw_planar.cpp](draw_planar.cpp) does assert all points z-coordinates to be 0.  
Before displaying it sets z-coordinate of first point to -1.  
Only that way the colored faces of planar embedding can be shown.  
build5 and build6 patch copies of CGAL files to start with colors and inverse_normal true.  
```
$ ./draw_planar ../10.off 
Using OpenGL context 4.6 GL
```

![../../res/demo.10.off.png](../../res/demo.10.off.png)

## cleanup

For Qt5 by executiong <kbd>really_clean5</kbd>.

For Qt5 by executiong <kbd>really_clean6</kbd>.


