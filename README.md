# NanoJVM

**NanoJVM** is an experimental Java bytecode interpreter, written entirely in C. This project designs and implements the [JVM specifications](https://docs.oracle.com/javase/specs/jvms/se21/html/index.html) for learning.

> [!NOTE]
> ⚡️ This project is a work in progress and primarily intended for experimentation and learning. Be aware that bugs, crashes and memory leaks are highly possible.

---

## 💡 Overview

NanoJVM aims to:

- Learn more about JVM internals.
- Interpret Java bytecode directly.
- Be highly portable thanks to its pure C design.
- Serve as a base for experimenting with Java virtual machine internals, memory management, and class loading.

---

## 🧑‍💻 Features

- 🧠 **Java Class Parsing** — powered by [classparse](https://github.com/koblizekXD/classparse) - my C-based Java Classfile parser.
- 💾 **Minimal Dependencies** — builds cleanly with `gcc` or `clang`.
- 🐧 **Cross-Platform Builds** — supports Linux, Windows (MinGW), and WebAssembly targets.

---

## ⚙️ Build Instructions

NanoJVM uses a `Makefile` for building and managing targets.

### Requirements

- GCC / Clang or MinGW (for Windows cross-compilation).
- GNU Make.
- [Optional] Emscripten for WebAssembly builds.

---

### Build Commands

```bash
# Development build for your target platform
make

# Clean build files
make clean

# Rebuild from scratch
make rebuild

# Specify build mode
make BUILD_MODE=prod
```

---

### Cross Compilation

#### Windows (MinGW)

```bash
make win32
```

#### WebAssembly (Using `emcc`)

```bash
make wasm
```

---

## 🔬 Project Structure

```
.
├── src/               # Source code for the NanoJVM core
├── lib/               # External libraries (e.g., miniz, classparse)
├── build/             # Object files and intermediate build outputs
├── bin/               # Final binaries
└── Makefile           # Build system
```

---

## 📄 Usage

For information about VM arguments, see:
```bash
./bin/nanojvm --help
```

```bash
./bin/nanojvm path/to/MyClass.class
```

---

## 📝 Roadmap

- [x] Basic bytecode execution.
- [x] Stack and heap simulation.
- [x] Class loading and constant pool resolution.
- [x] Native method stubs.
- [x] Exception handling.
- [ ] Garbage collection.
- [ ] JIT (maybe?).
- [ ] Assertions(a lot of code doesn't assert types stack requires etc.)

---

## 🙏 Acknowledgments

- `miniz` — a minimal zlib replacement for compressed `.class` file support and JAR/JMod reading. Licensed under MIT license.

---

## ⚠️ Disclaimer

This is an educational project and not intended for production use. Behavior may differ from the Java Virtual Machine Specification, especially under edge cases.
