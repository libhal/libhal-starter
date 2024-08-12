# libhal starter architecture

## `src/main.cpp`

It contains the main loop of the application. In this example, it just prints
"Hello, World" and toggles an LED on and off. This application will reset after
10 iterators just to demonstrate the capability.

## `include/application.hpp`

This file defines the `resource_list` structure which holds optional pointers
to the led, console, clock interfaces and reset callback. Modify this structure
to change the set of required drivers, settings, and memory resources for the
application. `initialize_platform` functions that must be implemented in the
`platform/` file.

## `CMakeLists.txt`

This is the build file for the project. It sets the minimum required version of
CMake, names the project, and sets the platform library. It also defines the
sources to compile and the libraries to link against. Finally, it sets up the
post-build steps with `libhal_post_build`.

### Profile Environment Variables

At the start of the cmake file you will encounter these lines:

```cmake
# Always check that the $ENV{LIBHAL_PLATFORM_LIBRARY} & $ENV{LIBHAL_PLATFORM}
# environment variables are set by the profile.
if("$ENV{LIBHAL_PLATFORM_LIBRARY}" STREQUAL "")
    message(FATAL_ERROR
        "Build environment variable LIBHAL_PLATFORM_LIBRARY is required for " "this project.")
endif()
if("$ENV{LIBHAL_PLATFORM}" STREQUAL "")
    message(FATAL_ERROR
        "Build environment variable LIBHAL_PLATFORM is required for "
        "this project.")
endif()
```

libhal conan profiles utilize environment variables as a generic means to
communicate to build systems the name of the libhal binary package,
`$ENV{LIBHAL_PLATFORM_LIBRARY}` and the name of the target platform
`$ENV{LIBHAL_PLATFORM}`. If the developer wishes to use a different build
system compatible with conan, they will need to use their build system's means
to acquire environment variables in order link against the correct library and
select the correct platform source file.

## `platforms/lpc4078.cpp`

This file provides an implementation for `initialize_platform`, for the
`LPC4078` platform. In `initialize_platform`, it sets the clock speed,
configures a uart peripheral for console output, and sets up an output pin for
the led. Each driver is statically allocated. The returned `resource_list` has
the pointers to each statically allocated peripheral and a reset callback that
can resets the microcontroller.

### `platforms/*.cpp`

Just like `platforms/lpc4078.cpp` but for any other platforms. It's important
to note that the specifics of the `initialize_platform` function and the
peripherals used will likely vary between different platforms but sometimes
they don't. As an example the `lpc4074.cpp` file just includes directly the
`lpc4078.cpp` because those are identical.
