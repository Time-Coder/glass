# GLASS -- Use easiest way to render 3D scene

# Introduction

## Getting Start

**GLASS** is an open source, cross-platform, object-oriented 3D rendering engine for C++. **GLASS** is built totally based on **OpenGL 4.6**. By using **GLASS**, you can build your VR scene in a very intuitive way. How intuitive? Just see following example:
```c++
#include "glass/Scene3D"
#include "glass/utils/geometry.h"

int main()
{
	Scene3D scene;
	// Create Scene3D object

	scene.models["floor"] = glass::floor();
	// Add floor model

	scene.models["floor"].setTexture("floor.png");
	// Set floor texture as "floor.png"

	scene.models["duck"].load("duck/duck.dae");
	// Add duck model

	scene.models["duck"].zoom(0.01);
	// Zoom duck size to scale 0.01

	scene.dir_lights["light"].setDirection(-1, -1, -1);
	// Add a direction light and set direction as (-1, -1, -1)

	scene.dir_lights["light"].useShadow();
	// Turn on shadow for this light,
	// otherwise all models won't have shadow.

	scene.show();
	// Show the scene
}
```
Run the code then you will see following window.

<img src="Figures/duck.png" width="70%"/>

You can use mouse and key to navigate in the scene:

* Mouse move to see around and up/down;
* <kbd>W</kbd> to move forward;
* <kbd>S</kbd> to move back;
* <kbd>A</kbd> to move left;
* <kbd>D</kbd> to move right;
* <kbd>E</kbd> to move up;
* <kbd>C</kbd> to move down;
* <kbd>ESC</kbd> to exit navigation mode and show the cursor;
* <kbd>Enter</kbd> to enter navigation mode again.

Click [demos] to see more demos.

Above example is using **GLASS** high level interface. **GLASS** library also provide a lot of interfaces for OpenGL core programming directly. See more details at [low level programming using GLASS].

If you want to learn systematically about **GLASS**, [Developer Guide] will teach you step-by-step to build your more and more complicated VR world. And [API Reference] will be a good helper for your development.

## Why GLASS?

The reason is simple, because **GLASS** is simple. Simple and intuitive is the core philosophy of **GLASS**. As you see the code above, every line of code just work as it's literal meaning. That's very friendly for 3D developement beginer. And more, here lists all the advantages of **GLASS**:

* **User friendly interface**: Compared to other 3D rendering engines, **GLASS** has the easiest interface and the most intuitive programming style.
* **Multi abstraction levels**: Beginer can use high level interface. For higher controllable developement, you can introduce more 3D rendering objects to build your scene. And for more degree of freedom developement, you can also introduce OpenGL objects. And even more, you can use **GLASS** just for OpenGL core mode programming: pass data to GPU, tell GPU to do something... in **GLASS** way.
* **Open source**: **GLASS** is released under [MIT Licence]. You can use **GLASS** in your commercial software freely.
* **Cross-platform**: **GLASS** is based on OpenGL. Because OpenGL is cross-platform, you can use **GLASS** to all devices that support OpenGL: desktop application(Windows/Linux/Mac), embedded firmware(Android/IOS/Other Embedded OS) and after the API for JavaScript is ready, web application will also be OK.
* **Cross-language**: It will be. But not now. **GLASS** will support pure C, Python, Java, JavaScript. Because **GLASS** is not just an API for one specific language, but also an architecture idea for 3D programming. This idea will be implement in different language for different developers. But now it's only support C++. Your voice and donation will accelerate the applying process.

And for 3D rendering details, it has following advantages:

* **Provide multi kinds of light**: You can use Point Light, Direction Light, Spot Light in your scene. And can easily control the parameter of lights.
* **Provid multi functions to generate primary shape**: 
* **Use CSM shadow**
* **Use defered shadering**
* ****