//
//	Application:	Voice Logger - Standard
//
//	File:			config.cpp
//	Purpose:		voice logger configuration
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#include "config.h"

Config::Config(QWidget *parent)
	: QWidget(parent)
{
}

Config::~Config()
{
}


// ***********************************************************************************************
// Recording Tab layout
// ***********************************************************************************************

RecordingTab::RecordingTab(QWidget *parent)
        : QWidget(parent)
{
	// Recording path
	QLabel *labelRecPath = new QLabel;
	labelRecPath->setText("Recording &Path");

	editRecPath = new QLineEdit();
	editRecPath->setFixedWidth(200);

	labelRecPath->setBuddy(editRecPath);

	QPushButton *browseButton = new QPushButton(tr("&Browse"));
	browseButton->setDefault(false);
	connect(browseButton, SIGNAL(clicked()), this, SLOT(FindFile()));

	// Recording name
	QLabel *labelRecName = new QLabel;
	labelRecName->setText("Recording &Name");

	editRecName = new QLineEdit(tr("%C %D %U %T"));
	editRecName->setFixedWidth(200);

	labelRecName->setBuddy(editRecName);

	// our threshold timeout value
	QLabel *labelThreshTimer = new QLabel;
	labelThreshTimer->setText("&Threshold Timeout (sec)");

	spinThreshTimer = new QSpinBox;
	spinThreshTimer->setRange(1, 180);
	spinThreshTimer->setValue(10);
	spinThreshTimer->setFixedWidth(50);

	labelThreshTimer->setBuddy(spinThreshTimer);

	// layout for options
	QGridLayout *gBoxRec = new QGridLayout;
	gBoxRec->addWidget(labelRecPath, 0, 0);
	gBoxRec->addWidget(editRecPath, 0, 1);
	gBoxRec->addWidget(browseButton, 0, 2);

	gBoxRec->addWidget(labelRecName, 1, 0);
	gBoxRec->addWidget(editRecName, 1, 1);

	gBoxRec->addWidget(labelThreshTimer, 2, 0);
	gBoxRec->addWidget(spinThreshTimer, 2, 1);

	setLayout(gBoxRec);
}

// ***********************************************************************************************
// Get the recording directory
// ***********************************************************************************************

void RecordingTab::FindFile()
{
	QDir *dir = new QDir;

	QString currentPath = dir->currentPath() + "/recordings";

	QFileDialog *fileDialog = new QFileDialog(this, "Select a path to save recordings", currentPath);
	fileDialog->setFileMode(QFileDialog::DirectoryOnly);
	fileDialog->setViewMode(QFileDialog::List);
	fileDialog->setFixedSize(500, 300);

    QStringList fileNames;

    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
		editRecPath->setText(QString(fileNames[0]));
	}

}

// ***********************************************************************************************
// User Tab layout
// ***********************************************************************************************

UserTab::UserTab(QWidget *parent)
        : QWidget(parent)
{
	// Company
	QLabel *labelCompany = new QLabel;
	labelCompany->setText("&Company");

	editCompany = new QLineEdit();
	editCompany->setFixedWidth(315);

	labelCompany->setBuddy(editCompany);

	// Department
	QLabel *labelDept = new QLabel;
	labelDept->setText("&Department");

	editDept = new QLineEdit();
	editDept->setFixedWidth(315);

	labelDept->setBuddy(editDept);

	// Username
	QLabel *labelUser = new QLabel;
	labelUser->setText("User &Name");

	editUser = new QLineEdit();
	editUser->setFixedWidth(200);

	labelUser->setBuddy(editUser);

	// passwords
	QPushButton *userPassButton = new QPushButton(tr("Set Use&r Password"));
	userPassButton->setDefault(false);

	connect(userPassButton, SIGNAL(clicked()), this, SLOT(ChangeUserPassword()));

	// layout for options
	QGridLayout *gBoxRec = new QGridLayout;
	gBoxRec->addWidget(labelCompany, 0, 0);
	gBoxRec->addWidget(editCompany, 0, 1, 1, 0);

	gBoxRec->addWidget(labelDept, 1, 0);
	gBoxRec->addWidget(editDept, 1, 1, 1, 0);

	gBoxRec->addWidget(labelUser, 2, 0);
	gBoxRec->addWidget(editUser, 2, 1);
	gBoxRec->addWidget(userPassButton, 2, 2);


	setLayout(gBoxRec);

}

// ***********************************************************************************************
// Show dialog to change user password
// ***********************************************************************************************

void UserTab::ChangeUserPassword()
{
	// create dialog
	QDialog *passwordDialog = new QDialog;

	// Old password
	QLabel *labelUserOldPass = new QLabel;
	labelUserOldPass->setText("&Old Password");


	editUserOldPass = new QLineEdit();
	editUserOldPass->setFixedWidth(180);
	editUserOldPass->setEchoMode(QLineEdit::Password);

	labelUserOldPass->setBuddy(editUserOldPass);

	// New password
	QLabel *labelUserNewPass = new QLabel;
	labelUserNewPass->setText("&New Password");

	editUserNewPass = new QLineEdit();
	editUserNewPass->setFixedWidth(180);
	editUserNewPass->setEchoMode(QLineEdit::Password);

	labelUserNewPass->setBuddy(editUserNewPass);

	// New password confirm
	QLabel *labelUserNewPassRep = new QLabel;
	labelUserNewPassRep->setText("&Confirm Password");

	editUserNewPassRep = new QLineEdit();
	editUserNewPassRep->setFixedWidth(180);
	editUserNewPassRep->setEchoMode(QLineEdit::Password);

	labelUserNewPassRep->setBuddy(editUserNewPassRep);

	// Buttons
	QHBoxLayout *hButtonBox = new QHBoxLayout;
	hButtonBox->addStretch(1);

	// create ok button
	QPushButton *okButton = new QPushButton(tr("&Ok"));
	okButton->setDefault(true);

	// set ok button to default and check if passwords match
	connect(okButton, SIGNAL(clicked()), this, SLOT(CheckPassMatches()));
	connect(this, SIGNAL(userPassMatches()), passwordDialog, SLOT(accept()));

	// create cancel button
	QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
	connect(cancelButton, SIGNAL(clicked()), passwordDialog, SLOT(reject()));
	cancelButton->setDefault(false);

	hButtonBox->addWidget(okButton);
	hButtonBox->addWidget(cancelButton);

	// layout for options
	QGridLayout *gBoxPass = new QGridLayout;
	gBoxPass->addWidget(labelUserOldPass, 0, 0);
	gBoxPass->addWidget(editUserOldPass, 0, 1);

	gBoxPass->addWidget(labelUserNewPass, 1, 0);
	gBoxPass->addWidget(editUserNewPass, 1, 1);

	gBoxPass->addWidget(labelUserNewPassRep, 2, 0);
	gBoxPass->addWidget(editUserNewPassRep, 2, 1);

	QVBoxLayout *vBox = new QVBoxLayout(passwordDialog);
    vBox->addLayout(gBoxPass);			// password stuff
    vBox->addLayout(hButtonBox);		// ok and cancel buttons

	passwordDialog->setLayout(vBox);

	passwordDialog->setGeometry(200,200,200,40);

	passwordDialog->setWindowTitle("Change User Password");
	passwordDialog->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	passwordDialog->exec();
}

// ***********************************************************************************************
// Checks if user passwords match
// ***********************************************************************************************

void UserTab::CheckPassMatches()
{
	// get sha1 hash
	SHA1* sha1 = new SHA1();

	// get sha1 hash of old password
	QByteArray oldPassHash = editUserOldPass->text().toAscii();

	sha1->addBytes(oldPassHash.data(), strlen(oldPassHash.data()));
	QString oldPassDigest = sha1->getDigest();
	oldPassDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

	delete sha1;

	// get password from registry
	QSettings configSettings;

	QString regPassDigest = configSettings.value("config/user/Password").toString();				// get password
	regPassDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

	// check if new passwords match
	if(editUserNewPass->text() == editUserNewPassRep->text())
	{
		// check if new and old pass match

		if(oldPassDigest.left(20) == regPassDigest.left(20))
		{
			// get sha1 hash
			SHA1* sha1 = new SHA1();

			// get sha1 hash of new password
			QByteArray newPassHash = editUserNewPass->text().toAscii();

			sha1->addBytes(newPassHash.data(), strlen(newPassHash.data()));
			QString newPassDigest = sha1->getDigest();
			newPassDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

			delete sha1;

			// update registry
			configSettings.setValue("config/user/Password", newPassDigest);				// set password

			QMessageBox::information(this, "Change User Password", QString("Passwords succesfully changed..."));

			// passwords match so send signal
			emit(userPassMatches());
		}
		else
		{
			QMessageBox::warning(this, "Change User Password", QString("Old Passwords do not match..."));
		}
	}
	else
	{
		QMessageBox::warning(this, "Change User Password", QString("New Passwords do not match..."));
	}

}


// ***********************************************************************************************
// General Tab layout
// ***********************************************************************************************

GeneralTab::GeneralTab(QWidget *parent)
        : QWidget(parent)
{
	checkStart = new QCheckBox(tr("&Start Automatically"));
	checkStart->setCheckState(Qt::Checked);

	checkConfig = new QCheckBox(tr("Allow &Configuration"));
	checkConfig->setCheckState(Qt::Unchecked);

	checkDeactivate = new QCheckBox(tr("Allow &Deactivation"));
	checkDeactivate->setCheckState(Qt::Unchecked);

	checkPlayback = new QCheckBox(tr("Allow &Playback"));
	checkPlayback->setCheckState(Qt::Checked);

	checkHidden = new QCheckBox(tr("Stealth &Mode"));
	checkHidden->setCheckState(Qt::Unchecked);

	checkComments = new QCheckBox(tr("&Commenting"));
	checkComments->setCheckState(Qt::Checked);

	// layout for options
	QGridLayout *gBoxRec = new QGridLayout;
	gBoxRec->addWidget(checkStart, 0, 0);
	gBoxRec->addWidget(checkConfig, 1, 0);
	gBoxRec->addWidget(checkDeactivate, 2, 0);
	gBoxRec->addWidget(checkPlayback, 0, 1);
	gBoxRec->addWidget(checkHidden, 1, 1);
	gBoxRec->addWidget(checkComments, 2, 1);

	setLayout(gBoxRec);
}

// ***********************************************************************************************
// Show Logger configuration
// ***********************************************************************************************

void Config::SaveSettings()
{
	// create our settings object
	QSettings voiceLogSettings;

	// save the recording settings
	voiceLogSettings.setValue("config/recording/Path", recordTab->editRecPath->text());			// the path of the recordings - usually app directory
	voiceLogSettings.setValue("config/recording/Name", recordTab->editRecName->text());			// record file name
	voiceLogSettings.setValue("config/recording/Timer", recordTab->spinThreshTimer->value());	// under threshold timer

	// save the user settings
	voiceLogSettings.setValue("config/user/Company", userTab->editCompany->text());				// company name
	voiceLogSettings.setValue("config/user/Department", userTab->editDept->text());				// department
	voiceLogSettings.setValue("config/user/Username", userTab->editUser->text());				// username

	// save general settings
	voiceLogSettings.setValue("config/general/AutoStart", generalTab->checkStart->checkState());				// auto start
	voiceLogSettings.setValue("config/general/AllowConfig", generalTab->checkConfig->checkState());				// allow access to configuration
	voiceLogSettings.setValue("config/general/AllowDeactivate", generalTab->checkDeactivate->checkState());		// allow access to deactivation
	voiceLogSettings.setValue("config/general/AllowPlayback", generalTab->checkPlayback->checkState());			// allow playback of recordings
	voiceLogSettings.setValue("config/general/GoStealth", generalTab->checkHidden->checkState());				// go into stealth mode - set process id different and no gui
	voiceLogSettings.setValue("config/general/Comments", generalTab->checkComments->checkState());				// create comments

	emit(SavedSettings());

//	QMessageBox::information(this, "test", QString("%1") .arg(recordTab->spinThreshTimer->value()));

}

// ***********************************************************************************************
// Show Logger configuration
// ***********************************************************************************************

void Config::ShowConfig()
{

	// create dialog
	QDialog *window = new QDialog;

	// Config Logo header
	QPixmap *pixLogo = new QPixmap;

	// load logo image from file
	pixLogo->load(":/images/cst_config_logo.png");

	// show logo file on label
	QLabel *labelConfigLogo = new QLabel;
	labelConfigLogo->setPixmap(*pixLogo);
	labelConfigLogo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	labelConfigLogo->setLineWidth(2);

	tabWidget = new QTabWidget;

	recordTab = new RecordingTab;
	userTab = new UserTab;
	generalTab = new GeneralTab;

    tabWidget->addTab(recordTab, tr("&Recording"));
    tabWidget->addTab(userTab, tr("&User"));
    tabWidget->addTab(generalTab, tr("&General"));

	// Buttons
	QHBoxLayout *hButtonBox = new QHBoxLayout;
	hButtonBox->addStretch(1);

	// create ok button
	QPushButton *okButton = new QPushButton(tr("&Ok"));

	// set ok button to default and accept
	connect(okButton, SIGNAL(clicked()), this, SLOT(SaveSettings()));
	connect(this, SIGNAL(SavedSettings()), window, SLOT(accept()));

	// create cancel button
	QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
	connect(cancelButton, SIGNAL(clicked()), window, SLOT(reject()));
	cancelButton->setDefault(false);

	hButtonBox->addWidget(okButton);
	hButtonBox->addWidget(cancelButton);

	// everything
	QVBoxLayout *vBox = new QVBoxLayout(window);
	vBox->addWidget(labelConfigLogo);
    vBox->addWidget(tabWidget);				// configuration tabs
    vBox->addLayout(hButtonBox);		// ok and cancel buttons

	window->setLayout(vBox);
	okButton->setDefault(true);

	window->setGeometry(200,200,400,50);
	window->setWindowTitle("VoiceLog1 Configuration");
	window->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	// create our settings object
	QSettings voiceLogSettings;

	// get the recording settings
	recordTab->editRecPath->setText(voiceLogSettings.value("config/recording/Path").toString());			// the path of the recordings - usually app directory
	recordTab->editRecName->setText(voiceLogSettings.value("config/recording/Name").toString());			// record file name
	recordTab->spinThreshTimer->setValue(voiceLogSettings.value("config/recording/Timer").toInt());	// under threshold timer

	// get the user settings
	userTab->editCompany->setText(voiceLogSettings.value("config/user/Company").toString());				// company name
	userTab->editDept->setText(voiceLogSettings.value("config/user/Department").toString());										// department
	userTab->editUser->setText(voiceLogSettings.value("config/user/Username").toString());				// username

	// get general settings
	generalTab->checkStart->setCheckState(Qt::CheckState(voiceLogSettings.value("config/general/AutoStart").toInt()));				// auto start
	generalTab->checkConfig->setCheckState(Qt::CheckState(voiceLogSettings.value("config/general/AllowConfig").toInt()));				// allow access to configuration
	generalTab->checkDeactivate->setCheckState(Qt::CheckState(voiceLogSettings.value("config/general/AllowDeactivate").toInt()));		// allow access to deactivation
	generalTab->checkPlayback->setCheckState(Qt::CheckState(voiceLogSettings.value("config/general/AllowPlayback").toInt()));			// allow playback of recordings
	generalTab->checkHidden->setCheckState(Qt::CheckState(voiceLogSettings.value("config/general/GoStealth").toInt()));				// go into stealth mode - set process id different and no gui
	generalTab->checkComments->setCheckState(Qt::CheckState(voiceLogSettings.value("config/general/Comments").toInt()));				// go into stealth mode - set process id different and no gui

	window->exec();
}

