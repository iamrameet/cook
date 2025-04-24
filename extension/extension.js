// The module 'vscode' contains the VS Code extensibility API
const vscode = require('vscode');

/**
 * @param {vscode.ExtensionContext} context
 */
function activate(context) {
    console.log('Cook Language Extension is now active!');

    // Register the completion provider for Cook language
    const provider = vscode.languages.registerCompletionItemProvider('cook', {
        provideCompletionItems(document, position) {
            // Get all text up to the current position
            const text = document.getText(new vscode.Range(0, 0, position.line, position.character));

            // Find all ingredient declarations
            const ingredientRegex = /ingredient\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=/g;
            const ingredients = [];
            let match;

            while ((match = ingredientRegex.exec(text)) !== null) {
                ingredients.push(match[1]);
            }

            // Find all recipe parameters
            const recipeParamRegex = /recipe\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)/g;
            const recipeParams = [];

            while ((match = recipeParamRegex.exec(text)) !== null) {
                const params = match[2].split(',').map(param => param.trim());
                recipeParams.push(...params);
            }

            // Create completion items for ingredients
            const completionItems = [];

            // Add ingredients (variables)
            for (const ingredient of ingredients) {
                const item = new vscode.CompletionItem(ingredient, vscode.CompletionItemKind.Variable);
                item.detail = 'Ingredient (variable)';
                item.documentation = `Ingredient "${ingredient}" defined in this file`;
                completionItems.push(item);
            }

            // Add recipe parameters
            for (const param of recipeParams) {
                if (param && !ingredients.includes(param)) {
                    const item = new vscode.CompletionItem(param, vscode.CompletionItemKind.Variable);
                    item.detail = 'Recipe parameter';
                    item.documentation = `Parameter "${param}" defined in a recipe`;
                    completionItems.push(item);
                }
            }

            // Add Cook language keywords
            const keywords = ['ingredient', 'recipe', 'cookbook', 'cook', 'taste'];
            for (const keyword of keywords) {
                const item = new vscode.CompletionItem(keyword, vscode.CompletionItemKind.Keyword);
                item.detail = 'Cook language keyword';
                completionItems.push(item);
            }

            return completionItems;
        }
    });

    context.subscriptions.push(provider);
}

function deactivate() {}

module.exports = {
    activate,
    deactivate
};
