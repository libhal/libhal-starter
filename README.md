# libhal-starter

Before getting started, if you haven't used libhal before, follow the
[Getting Started](https://libhal.github.io/latest/getting_started/) guide.

## 📥 Installing Platform Profiles

For ARM MCU profiles:

```bash
conan config install -sf conan/profiles/v1 -tf profiles https://github.com/libhal/libhal-arm-mcu.git
```

`micromod` profiles:

```bash
conan config install -sf conan/profiles/v1 -tf profiles https://github.com/libhal/libhal-micromod.git
```

## 🏗️ Building Application

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

## ✅ Supported platforms

- lpc4078
- lpc4074
- stm32f103c8
- micromod
  - lpc4078 MicroMod V5
  - stm32f103c8 MicroMod V4

## 🤔 Wondering how is this repo organized?

See our [`ARCHITECTURE.md`](./ARCHITECTURE.md) to learn about the important
files in the project, what they do, and quirks about how they work.
