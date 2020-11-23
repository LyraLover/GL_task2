#ifndef NOTEPADLOGIC_H
#define NOTEPADLOGIC_H

#include <QTextDocumentWriter>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QTextCursor>

#include "codeeditor.h"

class NotepadLogic : public QObject {
		Q_OBJECT
	public:
		NotepadLogic();
		~NotepadLogic() {}
		QTextDocument *getDocument();
		void setDocument(QTextDocument *document);
		void markChanges();

		void clear();
		bool open(const QString &filename);
		bool save(const QString &filename);

		QTextCursor find(int start, int end, const QRegExp &regexp) const;
		QTextCursor replace(int start, int end, const QRegExp &regexp, const QString &text);
		void undo();
		void redo();

		QString syntaxCheck() const;
		QXmlSchema *schemaCheck(const QString &schemaFilename);
		bool validateXML(QXmlSchema *schema);

		bool changed() const { return changedSinceSave; }
		const QString &file() const { return filename; }

	private:

		QString filename;
		bool changedSinceSave;
		QTextDocument *document;
};

#endif // NOTEPADLOGIC_H
