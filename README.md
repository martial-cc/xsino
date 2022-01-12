# xsino - X cursor signaller

The xsino utility facilitates the projection of signals through X, by way
of cursor movements.

Installation instructions
-------------------------

xsino is written in c99, and depends on X/Xlib.
Though the program is developed in OpenBSD, any X oriented
POSIX/Unix-like system should be compatible.

To install the program:

	$ sudo make install

Documentation
-------------

End user documentation can be found in the [`xsino(1)` manual page](xsino.1).

Involvement
-----------

All communication can be directed to xsino at martial dot cc

Usage examples
--------

```sh
# Use the X cursor to illustrate "shaking of the head" motions:

xsino -n

# Use the X cursor to illustrate "nodding of the head" motions:

xsino -y
```
