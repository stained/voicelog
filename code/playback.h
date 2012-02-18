//
//	Application:	Voice Logger - Standard
//
//	File:			playback.h
//	Purpose:		playback recordings
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#ifndef PLAYBACK_H
#define PLAYBACK_H

// QT specific
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
#include <QTabWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QSlider>
#include <QFont>
#include <QProgressBar>
#include <QApplication>
#include <QEventLoop>

//app sepcific
#include "fmod\fmod.h"
#include "vumeter.h"

// sha1 hashing
#include "sha1\sha1.h"

class PlayBack : public QWidget
{
	Q_OBJECT

	public:
		PlayBack(QWidget *parent = 0);
		~PlayBack();

	public slots:
		void ShowPlayBack();				// show the playback dialog

		void SetCell(int row, int col);		// sets the current row and cell on click
		void DeleteFile();					// deletes recording
		void GetProperties();				// Gets the properties of a recording
		void PlayFile();					// Does tha actual file playback

	signals:

	private slots:
		// playback funtions
		void FModDoPlay();
		void FModDoPause();
		void FModDoStop();
		void FModDoSpeedDn();
		void FModDoSpeedUp();

		void FModDoPos(int pos);
		void SetSliderLen(int len);

		void KillWindow();

	protected:

	private:
		// buttons
		QPushButton *playButton;
		QPushButton *propButton;
		QPushButton *delButton;

		// playback controls
		QPushButton *playControl;
		QPushButton *pauseControl;
		QPushButton *stopControl;
		QPushButton *speedDnControl;
		QPushButton *speedUpControl;


		void loadRecordings();					// loads the recording files
		void parseFileData(QString fileData);	// parses the properties file for each recording

		QTableWidget *qtwFileList;

		// settings
		QString qstrRecPath;

		// fmod settings
		QString qstrFMODDeviceDriver;
		int intFMODRecDevice, intFMODPlayDevice;

		// current cell and row
		int currentRow, currentCol;

		// required for actual playback
		VUMeter *vuMeter;
		FMod *fmodSound;

		// our slider :)
		QSlider *vuSlider;
};

#endif


