# RayCaster

This is a simple raycaster that I hacked together as a learning project with C & SDL. It's nothing I'm particularly proud of—it's just a learning project to better understand ray casting.

![raycaster](raycaster.png)

## Building
It comes with an Xcode project ready to build. The SDL2 and SDL2_image frameworks are distributed with it. The code itself should all be portable to other platforms without changes.

## Running
Use the arrows keys to turn and step forward/backward. Press the 'm' key to see the overhead map. Press it again to hide it. You can change the "level design" by editing the array laid out as a square at the top of game.c in initialize().

## Learning Resources
There were two articles that were particularly helpful:
- [A first-person engine in 265 lines](http://www.playfuljs.com/a-first-person-engine-in-265-lines/)
- [Raycasting](https://lodev.org/cgtutor/raycasting.html)


## License
Apache
