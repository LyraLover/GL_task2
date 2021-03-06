#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent):
	QPlainTextEdit(parent)
  , font("monospace", 11)
  , lineNumberArea(new LineNumberArea(this))
  , highlighter(nullptr)
{
	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(document(), &QTextDocument::documentLayoutChanged, this, &CodeEditor::updateLineNumberAreaWidth_noargs);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);

	updateLineNumberAreaWidth(0);
	document()->setDefaultFont(font);
}

QSyntaxHighlighter *CodeEditor::setHighlighter(QSyntaxHighlighter *highlighter) {
	// Unregister an old highlighter if it is present
	if (this->highlighter)
		this->highlighter->setDocument(nullptr);

	// Register a new highlighter
	highlighter->setDocument(document());

	// Remember a new highlighter and return an old one
	QSyntaxHighlighter *oldHighlighter = this->highlighter;
	this->highlighter = highlighter;

	return oldHighlighter;
}

int CodeEditor::lineNumberAreaWidth() {
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * qMax(digits, 3);

	return space;
}

void CodeEditor::updateLineNumberAreaWidth(int) {
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberAreaWidth_noargs() {
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
	QPainter painter(lineNumberArea);
	painter.setFont(font);
	painter.fillRect(event->rect(), Qt::lightGray);
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + qRound(blockBoundingRect(block).height());
	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
							 Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + qRound(blockBoundingRect(block).height());
		++blockNumber;
	}
}

void CodeEditor::setLineNumbersEnabled(bool enabled) {
	lineNumberArea->setVisible(enabled);
}

bool CodeEditor::lineNumbersEnabled() const {
	return lineNumberArea->isVisible();
}

void CodeEditor::fold() {
}

void CodeEditor::unfoldAll() {}
