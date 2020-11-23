#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
	delete ui;
}
