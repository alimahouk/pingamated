# Pingamated

The server component of a [chat app](https://github.com/alimahouk/pingamate-ios).

## Project Structure

```text
pingamated/
├── src/
│   ├── crypto/    - Cryptography-related implementations
│   ├── fs/        - File system operations
│   ├── include/   - Header files
│   ├── init/      - Initialization and setup
│   ├── net/       - Networking components
│   ├── proto/     - Protocol implementations
│   └── util/      - Utility functions and helpers
└── pingamated.xcodeproj/ - Xcode project files
```

## Building the Project

This project uses Xcode as the build system. Open `pingamated.xcodeproj` and build + run.

## Requirements

- macOS (Darwin)
- Xcode
- C/C++ compiler (clang)

## Development

The project is organized into several key components:

- **Networking (net/)**: Core networking functionality
- **Cryptography (crypto/)**: Security and encryption features
- **File System (fs/)**: File handling and I/O operations
- **Protocol (proto/)**: Network protocol implementations
- **Utilities (util/)**: Helper functions and common utilities

## License

This project is licensed under the terms specified in the LICENSE file.

## Author

Created by Ali Mahouk.
