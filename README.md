# libhal-starter

Before getting started, if you haven't used libhal before, follow the
[Getting Started](https://libhal.github.io/getting_started/) guide.

To build the project:

```bash
conan build . -pr <target_name> -pr <compiler>
```

For the `lpc4078`

```bash
conan build . -pr lpc4078 -pr arm-gcc-12.3
```

For the STM32F103 MicroMod V4:

```bash
conan build . -pr mod-stm32f1-v4 -pr arm-gcc-12.3
```

## Supported platforms

- lpc4078
- lpc4074
- stm32f103c8
- micromod
  - lpc4078 MicroMod V5
  - stm32f103c8 MicroMod V4
  - stm32f103c8 MicroMod V5

## Installing Platform Profiles

For ARM MCU profiles:

```bash
conan config install -sf conan/profiles/v1 -tf profiles https://github.com/libhal/libhal-arm-mcu.git
```

`micromod` profiles:

```bash
conan config install -sf conan/profiles/v1 -tf profiles https://github.com/libhal/libhal-micromod.git
```

## Description of Files

1. `main.cpp`: It contains the main loop of the application. In this
   example, it just prints "Hello, World" and toggles an LED on and off. This
   application will reset after 10 iterators just to demonstrate the capability.
2. `application.hpp`: This file defines the `resource_list` structure which
   holds optional pointers to the led, console, clock interfaces and reset
   callback. Modify this structure to change the set of required drivers,
   settings, and memory resources for the application. `initialize_platform`
   functions that must be implemented in the `platform/` file.
3. `CMakeLists.txt`: This is the build file for the project. It sets the minimum
   required version of CMake, names the project, and sets the platform library.
   It also defines the sources to compile and the libraries to link against.
   Finally, it sets up the post-build steps with `libhal_post_build`.
4. `platforms/lpc4078.cpp`: This file provides an implementation for
   `initialize_platform`, for the LPC4078 platform. In `initialize_platform`,
   it sets the clock speed, configures a uart peripheral for console output,
   and sets up an output pin for the led. Each driver is statically allocated.
   The returned `resource_list` has the pointers to each statically allocated
   peripheral and a reset callback that can resets the microcontroller.
5. `platforms/*.cpp`: Just like `platforms/lpc4078.cpp` but for any other
   platforms. It's important to note that the specifics of the
   `initialize_platform` function and the peripherals used will likely vary
   between different platforms but sometimes they don't. As an example the
   `lpc4074.cpp` file just includes directly the `lpc4078.cpp` because those
   are identical.
