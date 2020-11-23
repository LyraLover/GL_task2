#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QResizeEvent>
#include <QRect>
#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>

#include <highlighters.h>

class CodeEditor : public QPlainTextEdit {
		Q_OBJECT
	public:
		CodeEditor(QWidget *parent=nullptr);
		QSyntaxHighlighter *setHighlighter(QSyntaxHighlighter *highlighter);

		void lineNumberAreaPaintEvent(QPaintEvent *event);
		int lineNumberAreaWidth();

		void setLineNumbersEnabled(bool);
		bool lineNumbersEnabled() const;

		void fold();
		void unfoldAll();

	protected:
		void resizeEvent(QResizeEvent *event) override;

	private:
		void updateLineNumberAreaWidth(int newBlockCount);
		void updateLineNumberAreaWidth_noargs();
		void updateLineNumberArea(const QRect &rect, int dy);
		void updateWholeDocument();

		QFont font;
		QSyntaxHighlighter *highlighter;
		QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget {
	public:
		LineNumberArea(CodeEditor *editor):
			QWidget(editor)
		  , editor(editor)
		{}

		QSize sizeHint() const override {
			return QSize(editor->lineNumberAreaWidth(), 0);
		}

	protected:
		void paintEvent(QPaintEvent *event) override {
			editor->lineNumberAreaPaintEvent(event);
		}

	private:
		CodeEditor *editor;
};

#endif // CODEEDITOR_H
