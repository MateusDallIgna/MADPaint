# 2D Paint Program

A 2D graphics painting application with shape drawing, transformation, and view controls.

## Features

- **Drawing Modes**: Rectangle, Triangle, Circle, Polygon
- **Transformations**: Rotate, Scale, Translate selected shapes
- **View Controls**: Zoom in/out, Pan around the canvas
- **Selection**: Single or multiple shape selection
- **Color Controls**: Change colors of selected shapes
- **Measurements**: Display area and perimeter of selected shapes

## Building

### Using Makefile (Recommended)

The easiest way to build is using the provided Makefile:

```bash
make          # Build the program
make run      # Build and run the program
make clean    # Remove build artifacts
make help     # Show help message
```

The executable will be created in the root directory as `Madpaint`.

### Manual Compilation

If you prefer to compile manually, you can use:

```bash
# From the root directory
g++ source/main.cpp source/Application.cpp source/Renderer.cpp source/Shader.cpp \
    source/VertexArray.cpp source/Layer.cpp source/IndexBuffer.cpp \
    source/Rectangle.cpp source/Triangle.cpp source/Circle.cpp source/Polygon.cpp \
    source/glad.c \
    -I./include \
    -lglfw -lGL -lGLU -ldl -lpthread -lwayland-client -lwayland-cursor -lwayland-egl -lX11 \
    -std=c++17 -o Madpaint
```

**Note**: When compiling manually, make sure to:
- Compile from the root directory (or adjust paths accordingly)
- Run the executable from the root directory so it can find the `shaders/` folder
- Include the `-std=c++17` flag for C++17 features

## Requirements

- g++ compiler (C++17 support)
- GLFW library
- OpenGL libraries
- Wayland libraries (for Wayland/Hyprland)
- X11 libraries (for X11 compatibility)

## Controls

When you start the program, all controls will be displayed in the terminal.

### Drawing Modes
- **0** - Select Mode
- **1** - Draw Rectangle
- **2** - Draw Triangle
- **3** - Draw Circle
- **4** - Draw Polygon (click to add points, right-click to finish)

### Selection & Transformation
- **Left Click** - Select shape (hold Ctrl for multiple selection)
- **Drag Mouse** - Move selected shape(s)
- **Q** - Rotate selected shape(s) counter-clockwise
- **E** - Rotate selected shape(s) clockwise
- **+ / =** - Scale up selected shape(s)
- **-** - Scale down selected shape(s)
- **A** - Display area and perimeter of selected shape(s)

### View Controls
- **Z** - Zoom in
- **X** - Zoom out
- **Middle Mouse Button** - Pan (drag to move view)
- **Space + Drag Mouse** - Pan (hold space and drag)

### Color Controls
- **R** - Change selected shape(s) to RED
- **G** - Change selected shape(s) to GREEN
- **B** - Change selected shape(s) to BLUE
- **K** - Change selected shape(s) to BLACK

### Other Controls
- **F** - Toggle fill mode (filled/outline)
- **Backspace** - Delete selected shape(s)

## Running

After building, run the executable from the root directory:

```bash
./Madpaint
```

The program will display all controls in the terminal when it starts.

## Project Structure

```
MADPaint/
├── include/          # Header files
├── source/           # Source files
├── shaders/          # Shader files
├── Makefile          # Build configuration
└── README.md         # This file
```

## Notes

- The program is designed to work with Wayland Hyprland but also supports X11
- Shader files must be in the `shaders/` directory relative to where the executable is run
- All transformations (rotate, scale, translate) are applied around the shape's center
- Area and perimeter calculations use the original shape geometry (before transformations)

