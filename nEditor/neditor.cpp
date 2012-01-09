#include "stdafx.h"
#include "neditor.h"

nEditor::nEditor(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

nEditor::~nEditor()
{

}
