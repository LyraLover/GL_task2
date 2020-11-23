#include "highlighters.h"

PlainHighlighter::PlainHighlighter(QTextDocument *document):
	QSyntaxHighlighter(document)
{}

void PlainHighlighter::highlightBlock(const QString &text) {
	QTextCharFormat format;
	setFormat(0, text.size(), format);
}

XMLHighlighter::XMLHighlighter(QTextDocument *document): QSyntaxHighlighter(document) {
	elementFormat.setForeground(QColor("blue"));
	rules.emplace_back(
				QRegExp("\\b[\\w\\d_]+(?=[\\s/?])?"),
				elementFormat
	);

	attributeFormat.setFontItalic(true);
	attributeFormat.setForeground(QColor("green"));
	rules.emplace_back(
				QRegExp("\\b[\\w\\d_]+(?=\\=)"),
				attributeFormat
	);
	rules.emplace_back(
				QRegExp("="),
				attributeFormat
	);

	valueFormat.setForeground(QColor("orange"));
	valueStart = QRegExp("\"");
	valueEnd = QRegExp("\"(?=[\\s></])");

	singleLineCommentFormat.setForeground(QColor("grey"));
	rules.emplace_back(
				QRegExp("<!--[^\n]*-->"),
				singleLineCommentFormat
	);

	textFormat.setForeground(QColor("black"));
	rules.emplace_back(
				QRegExp(">(.+)(?=</?)"),
				textFormat
	);

	keywordFormat.setForeground(QColor("blue"));
	keywordFormat.setFontWeight(QFont::Bold);
	for (const auto pattern : {"\\b?xml\\b", "/>", ">", "<", "</"}) {
		rules.emplace_back(
				QRegExp(pattern), keywordFormat
		);
	}
}

void XMLHighlighter::highlightBlock(const QString &text) {
	for (auto &[pattern, format] : rules) {
		auto expression = QRegExp(pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);
	int startIndex = 0;
	if (previousBlockState() != 1) {
		startIndex = valueStart.indexIn(text);
	}
	while (startIndex >= 0) {
		int endIndex = valueEnd.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.size() - startIndex;
		}
		else {
			commentLength = endIndex - startIndex + valueEnd.matchedLength();
		}
		setFormat(startIndex, commentLength, valueFormat);
		startIndex = valueStart.indexIn(text, startIndex + commentLength);
	}
}
