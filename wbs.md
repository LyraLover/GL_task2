# WBS
## Libraries:
QT with Widget, XML and SyntaxHighlighter modules

## Line deletion
1. Get the buffer
	- QTextEdit::document(), QTextDocument::toPlainText()
2. Find current position
	- QTextEdit::textCursor()
3. Find first line break before current position or a start of the buffer
	- QTextDocument::find()
4. Find first line break after current position or the end of the buffer
	- QTextDocument::find()
5. Remove characters before two marked spots
	- std::remove, std::erase
6. If type is XML, markup the buffer
	- QSyntaxHighlighter::highlightBlock()

## Undo and redo
1. Get the buffer
	- QTextEdit::document()
2. Undo/redo changes of the buffer
	- QTextDocument::undo(), QTextDocument::redo()

## Opening and saving files, creating new ones
### New file
1. If the buffer has changed since last save, ask if current buffer needs to be saved. The action may be cancelled at this point. If the buffer has to be saved, continue with "Save" part of this section
2. Clear the buffer
	- QTextEdit::document(), QTextDocument::clear()

### Save
1. If there is no filename specified, prompt the filename otherwise. The action may be cancelled at this point
	- QFileDialog
2. Write buffer contents to the specified file
	- QFile

### Save as
1. Ask for a filename. The action may be cancelled at this point
	- QFileDialog
2. Write buffer contents to the specified file
	- std::ifstream

## UTF-8 support
UTF-8 support is achieved by using QString

## Search and replace with regex
### Search
1. Open a dialog with options:
	1. Regex
	2. Match case
2. Compile a regex
	- QRegExp
3. Find the first occurence in the buffer after the cursor
	- QTextDocument::find()

### Replace
1. Open a dialog with options:
	1. Regex
	2. Replacement
	3. Match case
	4. Replace one or all
2. Compile a regex
	- QRegExp
3. If only one must be replaced, replace one and move the cursor to the end of the inserted text. Replace all occurences and move the cursor to the end of the last replacement otherwise
	- QTextDocument::cursor(), QTextCursor()::insertText()

## Visual features
### Buffer statistics
Buffer statistics may be collected with QTextDocument methods:
	- characterCount
	- lineCount
Word statistics are collected with matching a text against a regex when the delimiter character is input

### Togglable line numbers
When line numbers need to be shown, labels with line numbers are shown to the left of the text field

## Working with XML
### Fold contents of XML nodes
Folds can be created by pressing ^/. Line folds work in two ways:
1. Folding XML nodes
	1. Get the oldest element on the line with cursor
	2. Find oldest element's end
	3. Add the lines between to a map of folds
	4. Update the input document accordingly
2. Folding selected lines of text
	1. Add selected lines to a map of folds
	2. Update the input document accordingly

### Syntax highlighting
QSyntaxHighlighter::highlightBlock()

### XML validation
QXmlSchema and QXmlSchemaValidator classes do all the work of validating XML
1. Save a file
2. Ask a user to select a file with schema or specify a URL
3. Load a schema and check if it is valid
	- QXmlSchema::load(), QXmlSchema::isValid()
4. Validate a current file against selected schema
	- QXmlSchemaValidator::validate(), QFile
