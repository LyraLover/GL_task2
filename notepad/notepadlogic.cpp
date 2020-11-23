#include "notepadlogic.h"

NotepadLogic::NotepadLogic():
	filename("")
  , changedSinceSave(false)
  , document(nullptr)
{
}

void NotepadLogic::setDocument(QTextDocument *document) {
	if (this->document)
		disconnect(this->document, &QTextDocument::contentsChanged, this, &NotepadLogic::markChanges);
	connect(document, &QTextDocument::contentsChanged, this, &NotepadLogic::markChanges);
	this->document = document;
}

QTextDocument *NotepadLogic::getDocument() {
	return document;
}

void NotepadLogic::clear() {
	document->clear();
	changedSinceSave = false;
}

bool NotepadLogic::open(const QString &filename) {
	clear();
	auto cursor = QTextCursor(document);
	QFile file(filename);
	if (!file.open(QFile::ReadOnly))
		return false;
	while (!file.atEnd()) {
		QString line = file.readLine();
		cursor.insertText(line);
	}
	file.close();

	this->filename = filename;
	changedSinceSave = false;
	return true;
}

bool NotepadLogic::save(const QString &filename) {
	QTextDocumentWriter writer(filename, "plaintext");
	if (writer.write(document)) {
		changedSinceSave = false;
		this->filename = filename;
		return true;
	}
	return false;
}

void NotepadLogic::markChanges() {
	changedSinceSave = true;
}

QString NotepadLogic::syntaxCheck() const {
	QString xml = document->toPlainText();
	QXmlStreamReader reader(xml);
	while (!reader.atEnd()) {
		reader.readNext();
	}
	if (reader.hasError()) {
		return reader.errorString();
	}
	else
		return "";
}

QXmlSchema *NotepadLogic::schemaCheck(const QString &schemaFilename) {
	QFile schemaFile(schemaFilename);
	if (!schemaFile.open(QFile::ReadOnly)) {
		return nullptr;
	}

	QXmlSchema *schema = new QXmlSchema();
	schema->load(&schemaFile);
	if (schema->isValid())
		return schema;
	else
		return nullptr;
}

bool NotepadLogic::validateXML(QXmlSchema *schema) {
	QXmlSchemaValidator validator(*schema);
	QString xml = document->toPlainText();
	return validator.validate(xml);
}
