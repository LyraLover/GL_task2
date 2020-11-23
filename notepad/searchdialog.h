#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QWidget>

namespace Ui {
	class SearchDialog;
}

class SearchDialog : public QWidget
{
		Q_OBJECT

	public:
		explicit SearchDialog(QWidget *parent = nullptr);
		~SearchDialog();
		Ui::SearchDialog *ui;

	public:
		void search(QRegExp regexp, int start, int end);
		void replace(QRegExp regexp, const QString &replacement, int start, int end);
};

#endif // SEARCHDIALOG_H
