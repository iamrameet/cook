# Installing the Cook Language Extension

## Method 1: Using VS Code Extension Development Features

1. Open the extension folder in VS Code
2. Press F5 (or select "Run" > "Start Debugging")
3. This will open a new VS Code window with the extension loaded
4. Open a .cook file in the new window
5. You should see syntax highlighting and be able to use the snippets

## Method 2: Install as a Regular Extension

### Prerequisites
- Node.js and npm installed
- VS Code Extension Manager (vsce) installed (`npm install -g @vscode/vsce`)

### Steps to Package and Install

1. From the extension directory, run:
   ```
   vsce package
   ```

2. This will create a .vsix file in the extension directory

3. In VS Code:
   - Open the Extensions view (Ctrl+Shift+X)
   - Click on the "..." menu (top-right of the Extensions view)
   - Select "Install from VSIX..."
   - Navigate to and select the .vsix file

4. The extension should now be installed and active for all .cook files
