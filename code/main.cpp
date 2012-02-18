//
//	Application:	Voice Logger - Standard
//
//	File:			main.c
//	Purpose:		app initialization
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#include <QApplication>
#include <QPushButton>

#include "recorder.h"

//	Our main entry point
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(voice_std);

	QCoreApplication::setOrganizationName("Coldshift Technologies");
    QCoreApplication::setOrganizationDomain("coldshift.com");
    QCoreApplication::setApplicationName("VoiceLog1");

	Recorder *recordGUI = new Recorder;

	recordGUI->setWindowFlags(Qt::WindowMinimizeButtonHint);
	recordGUI->show();

	return app.exec();
}
