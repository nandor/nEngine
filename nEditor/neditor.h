#ifndef NEDITOR_H
#define NEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_neditor.h"

class nEditor : public QMainWindow
{
	Q_OBJECT

public:
	nEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~nEditor();

private:
	Ui::nEditorClass ui;
};

#endif // NEDITOR_H
