# Cook Programming Language Extension

This extension provides syntax highlighting, snippets, and IntelliSense for the Cook programming language.

## Features

- Syntax highlighting for Cook language files (.cook)
- Code snippets for common Cook language constructs
- Language configuration for comments, brackets, and auto-closing pairs
- IntelliSense suggestions for ingredients (variables) and recipe parameters

## Snippets

The following snippets are available:

- `ingredient`: Create a new ingredient (variable) declaration
- `recipe`: Create a new recipe (function) declaration
- `taste`: Create a taste statement (print)
- `cook`: Create a cook call (function call)
- `comment`: Create a comment

## Example

```cook
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

## IntelliSense

The extension provides intelligent code completion for:

- **Ingredients (Variables)**: All ingredients defined in the current file will appear in the suggestions list
- **Recipe Parameters**: Parameters defined in recipes will be suggested
- **Keywords**: All Cook language keywords are included in suggestions

The suggestions will appear automatically as you type, or you can trigger them manually with `Ctrl+Space`.

## Development

### Building the Extension

To build and test the extension:

1. Open the extension folder in VS Code
2. Press F5 (or select "Run" > "Start Debugging")
3. This will open a new window with the extension loaded
4. Create a new file with a `.cook` extension
5. Verify that syntax highlighting works
6. Try out the snippets and IntelliSense features

## License

This extension is licensed under the MIT License.
