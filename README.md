# NanoJVM

This is an experimental JVM interpreter.

## Contributing

Contributions and ideas for improvement are always welcome! Feel free to report them in the issues or create a pull request.

### Coding Guidelines

Generally following what you see around codebase should be enough for you to figure it out, but here are some basics:

- Typedeffed structs - `typedef struct {...} CamelCase;`
- Public functions - `void CamelCase(void)`
- Static/private functions or functions not meant to be used globally - `_snake_case(void)` or `snake_case(void)`
- Nested structs - `struct { ... } camel_case;`

Please document the code with Doxygen-like comments.

## License

Based on the [MIT License](LICENSE).
