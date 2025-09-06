# NanoJVM
NanoJVM is a simple Proof of Concept (PoC) Java Virtual Machine (JVM) implemented in pure C, making it as portable and
lightweight as possible. It is designed to run on freestanding environments, where standard libraries may not be available.

## Contributing

We welcome any contributions or suggestions to improve NanoJVM. Feel free to open issues or submit pull requests.

### Source Code Layout

The source code is organized into multiple CMake submodules. Each submodule has its own `CMakeLists.txt` file, which is imported to
the main `CMakeLists.txt` file located in the root directory.

| Module            | Purpose                                                                                   | Requires libc | Executable |
|-------------------|-------------------------------------------------------------------------------------------|---------------|------------|
| nanojvm-classfile | Freestanding library for reading & parsing Java class files. See README for more details. |       ❌       |      ❌     |
| nanojvm-common    | Common utilities used by all other modules.                                               |       ❌       |      ❌     |
| nanojvm-tests     | Tests for all other modules(uses Greatest as testing library).                            |       ✅       |      ✅     |
| nanojvm-core      | Core NanoJVM library, containg interpreter, heap allocator and more.                      |       ❌       |      ❌     |

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.