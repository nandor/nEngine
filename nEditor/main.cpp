#include "stdafx.h"
#include "neditor.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	nEditor w;
	w.show();
	return a.exec();
}
