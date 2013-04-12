#include "touchdemo.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>

#define RELEASE 0

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#if RELEASE
    QApplication::setOverrideCursor(Qt::BlankCursor);
#endif

	QTextCodec *codec = QTextCodec::codecForName("UTF8");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);
	QFont font("宋体", 10, QFont::Normal, FALSE);
	a.setFont(font);

	TouchDemo w;
	w.show();

	return a.exec();
}
