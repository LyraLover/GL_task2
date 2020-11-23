#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>

#include "highlighters.h"
#include "notepadlogic.h"
#include "searchdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
		Q_OBJECT

	public:
		Notepad(QWidget *parent = nullptr);
		~Notepad();

	protected:
		void markChanges();

		bool actionNew();
		bool actionOpen();
		bool actionSave();
		bool actionSaveAs();

		void actionSearch();
		void actionUndo();
		void actionRedo();

		void actionLineNumbers();
		void actionFold();
		void actionUnfoldAll();

		void actionSyntaxHighlight();
		void actionSyntaxCheck();
		void actionSchemaCheck();

		void buttonFind();
		void buttonReplace();

		int askSaveDialog();
	private:
		Ui::Notepad *ui;
		NotepadLogic logic;
		SearchDialog searchDialog;
		QSyntaxHighlighter *xmlHighlighter, *plainHighlighter;
};
#endif // NOTEPAD_H
