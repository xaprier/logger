<h1 align="center">
  🚀 logger
</h1>

<p align="center">
  <a href="https://github.com/xaprier/logger/blob/main/LICENSE" target="blank">
    <img src="https://img.shields.io/github/license/xaprier/logger" alt="license" />
  </a>
</p>

<b>logger</b> is an thread-safe interface to outputting with colors and saving outputs to log files with C++ if you enable logging to file.

### 🛠️ Build Instructions for Linux/UNIX

Clone the repository to your directory

```sh
git clone https://github.com/xaprier/logger.git
```

Enter the direcotry of the source code

```sh
cd logger/
```

Build the makefile with CMake, if you don't have cmake you have to install it.

```sh
cmake -G "Unix Makefiles" -B "build"
```

Than our Makefile is ready for build the source code. We have to access the directory of makefile and run make.

```sh
cd build && make -j$(nproc)
```

Than our compiled library is ready for run in the directory of the makefile with the name "Logger_simple_app"
You can run the sample program with this command

```sh
./Logger_TEST
```

### ⛓ How to use in your project

You can use this project as an library to link your project with CMake.

- Add the repository to any place where you put libs as an git submodule.

```sh
git submodule add https://github.com/xaprier/logger.git lib/logger # if you have lib directory in your project for an example
```

- Add the project directory to your CMake project

```cmake
add_subdirectory(lib/logger)
```

- Link the library to your target

```cmake
target_link_libraries(myproject PRIVATE Logger) # Logger is the project name in my CMake project.
```

- Everything is complete, now you can use this project as a library in your project.

### 📚 Dependencies

- CMake(Building project)
- Git(Just for cloning repository)
