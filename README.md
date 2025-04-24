# Cook Programming Language

Cook is a programming language inspired by cooking recipes, where programming concepts are expressed through culinary metaphors.

## Philosophy

Cook aims to make programming more intuitive and approachable by leveraging familiar cooking concepts:
- Variables are "ingredients"
- Functions are "recipes"
- Libraries are "cookbooks"
- Execution is "cooking" or "baking"
- Debugging is "tasting"
- Comments are "notes"

## Features

- Simple, readable syntax inspired by cooking terminology
- Support for variables, functions, and basic control flow
- String and numeric data types
- Basic arithmetic operations
- Function calls with parameters

## Building from Source

### Prerequisites
- C++ compiler (MinGW-w64 recommended)
- CMake 3.10 or higher

### Build Instructions
```bash
# Create a build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```

## Example

```
// Hello World in Cook language
ingredient name = "World";
taste "Hello, " + name + "!";

// Define a recipe (function)
recipe greet(person) {
    taste "Greetings, " + person + "!";
}

// Cook (call) the recipe
cook greet(name);
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
