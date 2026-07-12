# Raytracing in C

A simple 2D raytracing simulation written in C using SDL3. The program draws a
light source, emits rays in every direction, and stops each ray when it hits a
moving circular obstacle or leaves the window.


![Raytracing simulation demo](demo_gif.gif)
its lagging in the gif only :-)

## How It Works

- The window size is fixed at `800x600`.
- A main circle acts as the light source.
- `RAY_COUNT` controls how many rays are emitted. Currently, the simulation
  casts `200` rays around the full circle.
- Each ray starts from the light source and moves forward one pixel at a time
  using `cos(angle)` and `sin(angle)`.
- While moving, the ray checks whether its current point is inside any obstacle.
  If it touches an obstacle, the ray stops.
- Obstacles are also circles. They move every frame using their velocity values.
- When an obstacle reaches a window boundary, its velocity is reversed, making it
  bounce back into the scene.
- Holding the left mouse button and moving the mouse drags the light source to a
  new position.

## Main Functions

- `FillCircle()` draws a filled circle on the SDL surface.
- `dist()` calculates the distance between two points.
- `isColiding()` checks whether a ray point is inside any obstacle.
- `drawRay()` traces a single ray until it collides or exits the screen.
- `generate_rays()` creates rays around the light source in all directions.
- `moveObstacle()` updates obstacle positions and handles wall bouncing.

## Running

For a direct run on Windows, keep these files in the same folder:

- `Run RayTracing.bat`
- `RayTracing.exe`
- `SDL3.dll`

Then double-click:

```text
Run RayTracing.bat
```

The launcher opens `RayTracing.exe` from the project folder and checks that the
SDL3 runtime DLL is present.

## Building

If SDL3 is already configured for your compiler, build with a command similar to:

```bash
gcc main.c -o RayTracing.exe -lSDL3 -lm
```

Then run:

```bash
./RayTracing.exe
```
