#ifndef HIGHLIGHTERS_H
#define HIGHLIGHTERS_H

#include <QSyntaxHighlighter>
#include <QString>
#include <vector>

class PlainHighlighter: public QSyntaxHighlighter {
	public:
		PlainHighlighter(QTextDocument *document);
		virtual void highlightBlock(const QString &) override;
};

class XMLHighlighter : public QSyntaxHighlighter {
	public:
		XMLHighlighter(QTextDocument *document);
		virtual void highlightBlock(const QString &) override;

	private:
		QTextCharFormat elementFormat, attributeFormat, valueFormat, singleLineCommentFormat, textFormat, keywordFormat;
		QRegExp valueStart, valueEnd;
		std::vector <std::pair <QRegExp, QTextCharFormat>> rules;

};

#endif // HIGHLIGHTERS_H
