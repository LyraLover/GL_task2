# Text editor with support for txt and xml
## Features
`^(key)` means `ctrl+(key)`
- Text editing (~2-3 hours)
	- Delete line with ^L
	- Undo/redo
		- Undo: ^Z
		- Redo: ^(Shift)Z
- Opening and saving files, creating new ones (~3-5 hours due to file chooser dialogs)
	- Shortcuts
		- New: ^N
		- Open: ^O
		- Save: ^S
		- Save As: ^(Shift)S
- UTF-8 support (<1 hour, since AFAIK QString handles UTF-8)
- Search and replace with regex (~2 hours)
	- Shortcuts
		- Find: ^F
		- Replace: ^R
- Visual features (~2 hours)
	- Buffer statistics
		- Number of symbols, words, lines
	- Togglable line numbers
- Working with XML: (~10 hours)
	- Fold contents of XML nodes
	- Syntax highlighting
	- XML validation
		- Well-formed
		- XSD

WBS is available [here](/wbs.md)
UML is available [here](/uml.svg)
