//
//	Application:	Voice Logger - Standard
//
//	File:			config.h
//	Purpose:		voicelogger configuration
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#ifndef CONFIG_H
#define CONFIG_H

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

//app sepcific

// sha1 hashing
#include "sha1\sha1.h"

class GeneralTab : public QWidget
{
    Q_OBJECT

	public:
        GeneralTab(QWidget *parent = 0);

		QCheckBox *checkStart;
		QCheckBox *checkConfig;
		QCheckBox *checkDeactivate;
		QCheckBox *checkPlayback;
		QCheckBox *checkHidden;
		QCheckBox *checkComments;
};

class RecordingTab : public QWidget
{
    Q_OBJECT

	public:
        RecordingTab(QWidget *parent = 0);

        // stuff we need to access from the save section
		QLineEdit *editRecPath;
		QLineEdit *editRecName;
		QSpinBox *spinThreshTimer;

	private slots:
		void FindFile();

	private:


};

class UserTab : public QWidget
{
    Q_OBJECT

	public:
        UserTab(QWidget *parent = 0);

		QLineEdit *editDept;
		QLineEdit *editCompany;
		QLineEdit *editUser;


	private slots:
		void ChangeUserPassword();
	//	void ChangeAdminPassword();
		void CheckPassMatches();

	signals:
		void userPassMatches();

	private:
		char* userPasswordMD5;
		char* adminPasswordMD5;

		// password edit boxes
		QLineEdit *editUserOldPass;
		QLineEdit *editUserNewPass;
		QLineEdit *editUserNewPassRep;

};


class Config : public QWidget
{
	Q_OBJECT

	public:
		Config(QWidget *parent = 0);
		~Config();

	public slots:
		// our configuration page;
		void ShowConfig();
		void SaveSettings();

	signals:
		void SavedSettings();

	private slots:

	protected:
		RecordingTab *recordTab;
		UserTab *userTab;
		GeneralTab *generalTab;

	private:
		QTabWidget *tabWidget;
};

#endif


