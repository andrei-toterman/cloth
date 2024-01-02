## cloth simulation
reimplementation of [this article](https://viscomp.alexandra.dk/index2fa7.html), using the programmable OpenGL pipeline

### dependencies
- OpenGL
- glfw
- glew

### compile and run
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build -G Ninja
cmake --build build
./build/cloth
```

### input
- W: move ball forward
- A: move ball left
- S: move ball backward
- D: move ball right
- Q: move ball down
- E: move ball up
- space: apply wind to cloth
