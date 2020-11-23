#include "notepad.h"
#include "ui_notepad.h"
#include "ui_searchdialog.h"

Notepad::Notepad(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Notepad)
{
	ui->setupUi(this);
	connect(ui->editor->document(), &QTextDocument::contentsChanged, this, &Notepad::markChanges);

	logic.setDocument(ui->editor->document());

	plainHighlighter = new PlainHighlighter(nullptr);
	xmlHighlighter = new XMLHighlighter(nullptr);

	// File menu
	connect(ui->actionNew, &QAction::triggered, this, &Notepad::actionNew);
	connect(ui->actionOpen, &QAction::triggered, this, &Notepad::actionOpen);
	connect(ui->actionSave, &QAction::triggered, this, &Notepad::actionSave);
	connect(ui->actionSaveAs, &QAction::triggered, this, &Notepad::actionSaveAs);

	// Edit menu
	connect(ui->actionFindReplace, &QAction::triggered, this, &Notepad::actionSearch);
	connect(ui->actionUndo, &QAction::triggered, this, &Notepad::actionUndo);
	connect(ui->actionRedo, &QAction::triggered, this, &Notepad::actionRedo);

	// View menu
	connect(ui->actionLineNumbers, &QAction::triggered, this, &Notepad::actionLineNumbers);
//	connect(ui->actionFold, &QAction::triggered, this, &Notepad::actionFold);
//	connect(ui->actionUnfoldAll, &QAction::triggered, this, &Notepad::actionUnfoldAll);

	// XML menu
	connect(ui->actionSyntaxHighlight, &QAction::triggered, this, &Notepad::actionSyntaxHighlight);
	connect(ui->actionXMLSyntax, &QAction::triggered, this, &Notepad::actionSyntaxCheck);
	connect(ui->actionXMLSchema, &QAction::triggered, this, &Notepad::actionSchemaCheck);

	// Find and replace buttons
//	connect(searchDialog.ui->buttonFind, &QPushButton::clicked, this, &Notepad::buttonFind);
//	connect(searchDialog.ui->buttonReplace, &QPushButton::clicked, this, &Notepad::buttonReplace);

	// Default mode is txt mode
	ui->actionLineNumbers->setChecked(true);
	ui->actionSyntaxHighlight->setChecked(false);
	ui->menuXML->setEnabled(false);
}

Notepad::~Notepad()
{
	delete ui;
	delete plainHighlighter;
	delete xmlHighlighter;
}

bool Notepad::actionNew() {
	if (logic.changed()) {
		int result = askSaveDialog();
		switch (result) {
			case QMessageBox::Save:
				if (!actionSaveAs())
					return false;
			break;
			case QMessageBox::Discard:
			break;
			case QMessageBox::Cancel:
				return false;
		}
	}
	logic.clear();
	setWindowTitle("Notepad");
	return true;
}

bool Notepad::actionSave() {
	QString filename = logic.file();
	if (filename.isEmpty()) {
		bool result = actionSaveAs();
		return result;
	}
	return logic.save(filename);
}

bool Notepad::actionSaveAs() {
	QString filename = QFileDialog::getSaveFileName(
				this,
				"Save file",
				"",
				"Text files (*.txt);;XML files (*.xml)"
	);
	if (filename.isEmpty()) {
		return false;
	}
	if (logic.save(filename)) {
		setWindowTitle("Notepad: " + filename);
		if (filename.endsWith(".xml")) {
			ui->menuXML->setEnabled(true);
			ui->editor->setHighlighter(xmlHighlighter);
			ui->editor->setLineNumbersEnabled(true);
		}
		else {
			ui->menuXML->setEnabled(false);
			ui->editor->setHighlighter(plainHighlighter);
			ui->editor->setLineNumbersEnabled(false);
		}
		return true;
	}

	return false;
}

bool Notepad::actionOpen() {
	if (logic.changed()) {
		int result = askSaveDialog();
		switch (result) {
			case QMessageBox::Save:
				if (!actionSaveAs())
					return false;
			break;
			case QMessageBox::Discard:
			break;
			case QMessageBox::Cancel:
				return false;
		}
	}

	// Get file name
	QString filename = QFileDialog::getOpenFileName(
				this,
				"Open file",
				"",
				"Text files (*.txt);;XML files (*.xml)"
	);
	if (filename.isEmpty()) {
		return false;
	}

	if (logic.open(filename)) {
		setWindowTitle("Notepad: " + filename);
		if (filename.endsWith(".xml")) {
			ui->menuXML->setEnabled(true);
			ui->editor->setHighlighter(xmlHighlighter);
			ui->actionSyntaxHighlight->setChecked(true);
			ui->actionLineNumbers->setChecked(true);
		}
		else {
			ui->menuXML->setEnabled(false);
			ui->editor->setHighlighter(plainHighlighter);
			ui->editor->setLineNumbersEnabled(false);
		}
		return true;
	}

	return false;
}

void Notepad::markChanges() {
	if (!logic.changed()) {
		setWindowTitle("*" + windowTitle());
	}
	logic.markChanges();
}

int Notepad::askSaveDialog() {
	QMessageBox messageBox;
	messageBox.setText("The document has been modified");
	messageBox.setInformativeText("Do you want to save your changes?");
	messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	messageBox.setDefaultButton(QMessageBox::Save);
	return messageBox.exec();
}

void Notepad::actionSearch() {
	searchDialog.setVisible(!searchDialog.isVisible());
}

void Notepad::actionUndo() {
	ui->editor->undo();
}

void Notepad::actionRedo() {
	ui->editor->redo();
}

void Notepad::actionLineNumbers() {
	ui->editor->setLineNumbersEnabled(ui->actionLineNumbers->isChecked());
}

void Notepad::actionFold() {
	ui->editor->fold();
}

void Notepad::actionUnfoldAll() {
	ui->editor->unfoldAll();
}

void Notepad::actionSyntaxHighlight() {
	if (ui->actionSyntaxHighlight->isChecked()) {
		ui->editor->setHighlighter(xmlHighlighter);
	}
	else {
		ui->editor->setHighlighter(plainHighlighter);
	}
}

void Notepad::actionSyntaxCheck() {
	QString errorString = logic.syntaxCheck();

	QMessageBox messageBox;
	if (errorString.size()) {
		messageBox.setText("XML syntax is invalid");
		messageBox.setInformativeText(errorString);
	}
	else {
		messageBox.setText("XML is well-formed");
	}
	messageBox.addButton(QMessageBox::Ok);
	messageBox.setDefaultButton(QMessageBox::Ok);
	messageBox.exec();
}

void Notepad::actionSchemaCheck() {
	QString schemaFilename = QFileDialog::getOpenFileName(
				this,
				"Select schema file",
				"",
				"XML Schema Definitions (*.XSD)"
	);
	if (schemaFilename.isEmpty()) {
		return;
	}

	QMessageBox messageBox;
	messageBox.addButton(QMessageBox::Ok);
	messageBox.setDefaultButton(QMessageBox::Ok);

	QXmlSchema *schema = logic.schemaCheck(schemaFilename);
	if (!schema) {
		messageBox.setText("XML schema is invalid");
	}
	else {
		if (logic.validateXML(schema)) {
			messageBox.setText("XML is invalid");
		}
		else {
			messageBox.setText("XML is valid");
		}
	}
	messageBox.exec();

}

//void Notepad::buttonFind() {
//	auto cursor = ui->editor->textCursor();
//	auto start = (searchDialog.ui->fromBeginning->isChecked())? 0: cursor.selectionStart();
//	auto end = (searchDialog.ui->inSelection->isChecked())? -1: cursor.selectionEnd();
//	QRegExp regexp(
//				searchDialog.ui->regexpText->text(),
//				(searchDialog.ui->caseSensitive->isChecked())? Qt::CaseSensitive: Qt::CaseInsensitive
//	);
//	ui->editor->setTextCursor(logic.find(start, end, regexp));
//}

//void Notepad::buttonReplace() {
//	auto cursor = ui->editor->textCursor();
//	auto start = (searchDialog.ui->fromBeginning->isChecked())? 0: cursor.selectionStart();
//	auto end = (searchDialog.ui->inSelection->isChecked())? -1: cursor.selectionEnd();
//	QRegExp regexp(
//				searchDialog.ui->regexpText->text(),
//				(searchDialog.ui->caseSensitive->isChecked())? Qt::CaseSensitive: Qt::CaseInsensitive
//	);
//	auto replacement = searchDialog.ui->replaceText->text();
//	ui->editor->setTextCursor(logic.replace(start, end, regexp, replacement));
//}
