//
//	Application:	Voice Logger - Standard
//
//	File:			recorder.h
//	Purpose:		logger gui and status
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#ifndef RECORDER_H
#define RECORDER_H

// QT Specific
#include <QApplication>
#include <QWidget>
#include <QMessageBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QFrame>
#include <QTimer>
#include <QStatusBar>
#include <QSettings>
#include <QDateTime>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QShortcut>

// App specific
#include "fmod\fmod.h"
#include "vumeter.h"
#include "config.h"
#include "playback.h"

// sha1 hashing
#include "sha1\sha1.h"


//class VUMeter;

class Recorder : public QWidget
{
	Q_OBJECT

	public:
		Recorder(QWidget *parent = 0);

	public slots:
		void MakeRecordName();		// create the record file name from the settings
		void CreateComment();		// create the comment for the recording (activate in settings)
		void GetSettings();			// get settings from registry
		void SaveComment();			// saves comment data to file

		// hidden menu
		void ShowHiddenMenu();

		// activation
		void DoActivate();

		// playback
		void DoPlayback();

		// audio config
		void DoFMODConfig();

		// voicelog config
		void DoVoiceLogConfig();

	signals:
		void RemakeRecordName();	// recreate the filename for each recording
		void GetSettingsSignal();	// re-get the settings if changed
		void SavedComment();		// comment saved

		void passwordMatches();

	private slots:
		void CheckPassMatches();


	protected:

	private:

		// passwords
		void EnterUserPassword();
		bool userPassed;

		QLineEdit *userPassButton;

		// buttons
		QPushButton *audioButton;
		QPushButton *loggerButton;
	    QPushButton *playButton;
		QPushButton *activateButton;
		QPushButton *quitButton;

		VUMeter *vuMeter;
		FMod *fmodSound;
		PlayBack *playBack;

		// settings values

		// recording settings
		QString qstrRecPath;
		QString qstrRecName;
		int intThreshTimer;

		// final version of our name after replacing strings
		QString qstrRecNameFinal;

		// user settings
		QString qstrCompany;
		QString qstrDept;
		QString qstrUser;

		// general settings
		int intAutoStart;
		int intAllowConfig;
		int intAllowDeactivate;
		int intAllowPlayback;
		int intGoStealth;

		int intComments;

		// fmod settings
		QString qstrFMODDeviceDriver;
		int intFMODRecDevice, intFMODPlayDevice;

		// voice logger
		Config *configLogger;

		// comments
		QString qstrLastDateTime;		// time and date of start of last recording

		QLabel *labelFileName1;
		QLabel *labelFileLen1;
		QLabel *labelFileHash1;
		QLabel *labelDateTime1;
		QLabel *labelType1;
		QLineEdit *editContact;
		QLineEdit *editContactNum;
		QTextEdit *textComment;
};

#endif


