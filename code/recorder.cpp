//
//	Application:	Voice Logger - Standard
//
//	File:			recorder.c
//	Purpose:		recording and record status subsystem
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//


#include "recorder.h"

// ***********************************************************************************************
// Init our recorder GUI and connect all objects
// ***********************************************************************************************

Recorder::Recorder(QWidget *parent)
	: QWidget(parent)
{
	GetSettings();

	userPassed = false;

	// get settings
	//connect(this, SIGNAL(GetSettingsSignal()), this, SLOT(GetSettings()));

	// create gui

	// create logo
	QPixmap *pixLogo = new QPixmap;

	// load logo image from file
	pixLogo->load(":/images/cst_logo.png");

	// show logo file on label
	QLabel *labelCSTLogo = new QLabel;
	labelCSTLogo->setPixmap(*pixLogo);
	labelCSTLogo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	labelCSTLogo->setLineWidth(2);

	// create our FMod sound object
	fmodSound = new FMod;

	// set fmod driver if available
	if(intFMODRecDevice != -1)
	{
		QByteArray qbyteTemp(qstrFMODDeviceDriver.toLatin1());
		char *charFMODDeviceDriver = qbyteTemp.data();

		// set driver
		fmodSound->setDriver(charFMODDeviceDriver, intFMODRecDevice, intFMODPlayDevice);
	}

	// create controls
	QGroupBox *groupBoxControls = new QGroupBox(tr("Controls"));

	// create layout for controls
	QVBoxLayout *vBoxControls = new QVBoxLayout;

	// our audio configuration tool in fmod
	audioButton = new QPushButton(tr("&Audio Settings"));
	connect(fmodSound, SIGNAL(SavedSettings()), this, SLOT(GetSettings()));		// re-get setting
	connect(audioButton, SIGNAL(clicked()), this, SLOT(DoFMODConfig()));

	// create our configuration
	configLogger = new Config;

	loggerButton = new QPushButton(tr("&VoiceLog Settings"));
	connect(configLogger, SIGNAL(SavedSettings()), this, SLOT(GetSettings()));		// re-get setting

	connect(loggerButton, SIGNAL(clicked()), this, SLOT(DoVoiceLogConfig()));

	playBack = new PlayBack;

	playButton = new QPushButton(tr("&Playback"));
	connect(playButton, SIGNAL(clicked()), this, SLOT(DoPlayback()));

	activateButton = new QPushButton(tr("A&ctivate"));
	activateButton->setCheckable(true);
	activateButton->setDefault(true);

	// check if we need to activate or deactivate button
	connect(activateButton, SIGNAL(clicked()), this, SLOT(DoActivate()));

	quitButton = new QPushButton(tr("&Quit"));
	connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	vBoxControls->addWidget(audioButton);
	vBoxControls->addWidget(loggerButton);
    vBoxControls->addWidget(playButton);
	vBoxControls->addWidget(activateButton);
	vBoxControls->addWidget(quitButton);

    groupBoxControls->setLayout(vBoxControls);

	// create layout for VU
	QGroupBox *groupBoxVU = new QGroupBox(tr("Sound Level Monitor"));

	QVBoxLayout *vBoxVU = new QVBoxLayout;

	// create VU Meter
	vuMeter = new VUMeter(this);
	vuMeter->setFixedSize(280,144);

	// connect to our sound recorder
	connect(vuMeter, SIGNAL(overThreshold()), this, SLOT(MakeRecordName()));

	connect(vuMeter, SIGNAL(overThreshold()), fmodSound, SLOT(startRecordToFile()));
	connect(vuMeter, SIGNAL(underThreshold()), fmodSound, SLOT(stopRecordToFile()));

	// connect our vu meters threshold to vu meter's recording light
	connect(vuMeter, SIGNAL(underThreshold()), vuMeter, SLOT(hideRecord()));
	connect(vuMeter, SIGNAL(overThreshold()), vuMeter, SLOT(showRecord()));

	// save a comment if required
	if(intComments == 2)
	{
		connect(vuMeter, SIGNAL(underThreshold()), this, SLOT(CreateComment()));
	}

	//connect our vu meter to the sound buffer
	connect(fmodSound, SIGNAL(waveChanged(int)), vuMeter, SLOT(setVU(int)));

	// check freq
	connect(fmodSound, SIGNAL(freqChanged(float)), vuMeter, SLOT(setFreq(float)));

	// check sound length
	connect(fmodSound, SIGNAL(lenChanged(int)), vuMeter, SLOT(setLen(int)));

	vBoxVU->addWidget(vuMeter);

	groupBoxVU->setLayout(vBoxVU);


	QGridLayout *gBoxGUI = new QGridLayout;

	gBoxGUI->addWidget(labelCSTLogo, 0, 0, 1, 0);
	gBoxGUI->addWidget(groupBoxControls, 1, 0);
	gBoxGUI->addWidget(groupBoxVU, 1, 1);

	setGeometry(200,200,100,100);

	setWindowTitle("Coldshift Technologies - VoiceLog1 (single user)");
	setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	// create our shortcut
	QShortcut *shortKey = new QShortcut(QKeySequence(tr("CTRL+ALT+SHIFT+C, CTRL+ALT+SHIFT+O, CTRL+ALT+SHIFT+L, CTRL+ALT+SHIFT+D")), this);
	connect(shortKey, SIGNAL(activated()), this, SLOT(ShowHiddenMenu()));

	setLayout(gBoxGUI);

}

// ***********************************************************************************************
// show hidden menu
// ***********************************************************************************************

void Recorder::ShowHiddenMenu()
{
	QMessageBox::information(this, "test", "hidden menu");

}

// ***********************************************************************************************
// test and activate recording
// ***********************************************************************************************

void Recorder::DoActivate()
{
	// not running
	if(activateButton->isChecked() == 0)
	{
		// allowed
		if(intAllowDeactivate > 0 || userPassed == true)
		{
			activateButton->setChecked(false);

			activateButton->setText(tr("A&ctivate"));
			fmodSound->stopMonitor();

			// activate buttons
			audioButton->setEnabled(true);;
			loggerButton->setEnabled(true);;
		    playButton->setEnabled(true);;
			activateButton->setEnabled(true);;
			quitButton->setEnabled(true);

			// if recording, deactivate
//			fmodSound->stopRecordToFile();
			vuMeter->hideRecord();

		}
		else if(intAllowDeactivate == 0 && userPassed == false)
		{
			activateButton->setChecked(true);
			// not allowed to
			EnterUserPassword();
		}

	}

	else
	{
		activateButton->setText(tr("Dea&ctivate"));
		fmodSound->startMonitor();

		activateButton->setChecked(true);

		// deactivate buttons
		audioButton->setEnabled(false);;
		loggerButton->setEnabled(false);;
	    playButton->setEnabled(true);;
		activateButton->setEnabled(true);;
		quitButton->setEnabled(true);
	}
}

// ***********************************************************************************************
// test and activate playback
// ***********************************************************************************************

void Recorder::DoPlayback()
{
	// allowed
	if(intAllowPlayback > 0 || userPassed == true)
	{
		playBack->ShowPlayBack();
	}
	else if(intAllowPlayback == 0 && userPassed == false)
	{
		// not allowed to
		EnterUserPassword();
	}

}


// ***********************************************************************************************
// test and activate audio config
// ***********************************************************************************************

void Recorder::DoFMODConfig()
{
	// allowed
	if(intAllowConfig > 0 || userPassed == true)
	{
//		fmodSound->stopMonitor();
		fmodSound->showConfig();
	}
	else if(intAllowConfig == 0 && userPassed == false)
	{
		// not allowed to
		EnterUserPassword();
	}

}

// ***********************************************************************************************
// test and activate audio config
// ***********************************************************************************************

void Recorder::DoVoiceLogConfig()
{
	// allowed
	if(intAllowConfig > 0 || userPassed == true)
	{
		configLogger->ShowConfig();
	}
	else if(intAllowConfig == 0 && userPassed == false)
	{
		// not allowed to
		EnterUserPassword();
	}

}


// ***********************************************************************************************
// create a comment if required
// ***********************************************************************************************

void Recorder::CreateComment()
{

	// create comment dialog
	QDialog *window = new QDialog;

	// info about the recording
	// filename
	QLabel *labelFileName = new QLabel;
	labelFileName->setText("File Name:");

	labelFileName1 = new QLabel;
	labelFileName1->setText(qstrRecNameFinal + ".mp3");

	// file length
	QLabel *labelFileLen = new QLabel;
	labelFileLen->setText("File Length:");


	// determine the actual file length (length of wav)
	QDateTime qdtFileLen = QDateTime::fromString(QString("00"), QString("ss"));
	QDateTime qdtFileLen2 = qdtFileLen.addSecs((int)fmodSound->getLastDataLen());

	QString qstrFileLen = qdtFileLen2.toString(QString("hh:mm:ss"));

	labelFileLen1 = new QLabel;
	labelFileLen1->setText(qstrFileLen);

	// get the file has (to determine if theres been tampering)
	QLabel *labelFileHash = new QLabel;
	labelFileHash->setText("SHA1 Hash");

	labelFileHash1 = new QLabel;

	QString qstrFileHash = fmodSound->getFileHash();
	qstrFileHash.replace(QString(" "), QString(""), Qt::CaseInsensitive);

	labelFileHash1->setText(qstrFileHash);

	// Date and Time
	QLabel *labelDateTime = new QLabel;
	labelDateTime->setText("Date / Time:");

	labelDateTime1 = new QLabel;
	labelDateTime1->setText(qstrLastDateTime);

	// call type
	QLabel *labelType = new QLabel;
	labelType->setText("Call Type:");

	// check if theres a tel number saved
	QString qstrLastTelNum = fmodSound->getTelNum();

	editContactNum = new QLineEdit();
	editContactNum->setFixedWidth(200);

	if(qstrLastTelNum != "")
	{
		labelType1 = new QLabel;
		labelType1->setText("Outgoing");
		editContactNum->setText(qstrLastTelNum);
	}
	else
	{
		labelType1 = new QLabel;
		labelType1->setText("Incoming");
	}

	// Call to / from
	QLabel *labelContact = new QLabel;
	labelContact->setText("&Contact Person:");

	editContact = new QLineEdit();
	editContact->setFixedWidth(200);

	labelContact->setBuddy(editContact);

	// Contact Number
	QLabel *labelContactNum = new QLabel;
	labelContactNum->setText("Contact &Number:");

	labelContactNum->setBuddy(editContactNum);

	// comment box
	QLabel *labelComment = new QLabel;
	labelComment->setText("Co&mment:");

	textComment = new QTextEdit;
	textComment->setFixedWidth(200);

	labelComment->setBuddy(textComment);

	// Create Layout
	QGridLayout *gBoxData = new QGridLayout;

	gBoxData->addWidget(labelFileName, 0, 0);
	gBoxData->addWidget(labelFileName1, 0, 1);

	gBoxData->addWidget(labelFileLen, 1, 0);
	gBoxData->addWidget(labelFileLen1, 1, 1);

	gBoxData->addWidget(labelDateTime, 2, 0);
	gBoxData->addWidget(labelDateTime1, 2, 1);

	gBoxData->addWidget(labelType, 3, 0, Qt::AlignTop);
	gBoxData->addWidget(labelType1, 3, 1, Qt::AlignTop);

	gBoxData->setRowMinimumHeight(3, 20);

	gBoxData->addWidget(labelContact, 4, 0);
	gBoxData->addWidget(editContact, 4, 1);
	gBoxData->addWidget(labelContactNum, 5, 0);
	gBoxData->addWidget(editContactNum, 5, 1);
	gBoxData->addWidget(labelComment, 6, 0, Qt::AlignTop);
	gBoxData->addWidget(textComment, 6, 1);

	// Buttons
	QHBoxLayout *hButtonBox = new QHBoxLayout;
	hButtonBox->addStretch(1);

	// create ok button
	QPushButton *okButton = new QPushButton(tr("&Save"));

	// set ok button to default and accept
	connect(okButton, SIGNAL(clicked()), this, SLOT(SaveComment()));
	connect(this, SIGNAL(SavedComment()), window, SLOT(accept()));

	// create cancel button
	QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
	connect(cancelButton, SIGNAL(clicked()), window, SLOT(reject()));
	cancelButton->setDefault(false);

	hButtonBox->addWidget(okButton);
	hButtonBox->addWidget(cancelButton);

	// everything
	QVBoxLayout *vBox = new QVBoxLayout(window);
    vBox->addLayout(gBoxData);		// configuration tabs
    vBox->addLayout(hButtonBox);		// ok and cancel buttons

	window->setLayout(vBox);
	okButton->setDefault(true);

	window->setGeometry(200,200,200,50);
	window->setWindowTitle("Add Comment");
	window->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	// create a timer to close the comment window
	QTimer *saveCommentTimer = new QTimer(this);
	connect(saveCommentTimer, SIGNAL(timeout()), okButton, SLOT(click()));
	saveCommentTimer->start(10000);

	window->exec();

}

// ***********************************************************************************************
// if ok clicked, save comment
// ***********************************************************************************************

void Recorder::SaveComment()
{
	QString qstrComment;

	// save filename
	qstrComment = "<file>" + labelFileName1->text() + "</file>\n";
	qstrComment = qstrComment + "<hash>" + labelFileHash1->text() + "</hash>\n";
	qstrComment = qstrComment + "<size>" + labelFileLen1->text() + "</size>\n";
	qstrComment = qstrComment + "<date>" + labelDateTime1->text() + "</date>\n";
	qstrComment = qstrComment + "<type>" + labelType1->text() + "</type>\n";
	qstrComment = qstrComment + "<contact_person>" + editContact->text() + "</contact_person>\n";
	qstrComment = qstrComment + "<contact_number>" + editContactNum->text() + "</contact_number>\n";
	qstrComment = qstrComment + "<comment>\n" + textComment->toPlainText() + "\n</comment>\n";

	// get an integrity hash for our comment file
	QByteArray qbyteCommentToHash(qstrComment.toLatin1());

	SHA1* sha1 = new SHA1();

	sha1->addBytes(qbyteCommentToHash.data(), qbyteCommentToHash.size());
	QString shaDigest = sha1->getDigest();

	delete sha1;

	// remove all whitespaces
	shaDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

	qstrComment = qstrComment + "<integrity>" + shaDigest + "</integrity> \r\n";

	QString qstrFileName = labelFileName1->text();
	qstrFileName = qstrFileName.left(qstrFileName.length() - 4);

	if(qstrRecPath.right(1) == "/")
	{
		qstrFileName = qstrRecPath  + qstrFileName + ".txt";
	}
	else
	{
		qstrFileName = qstrRecPath + "/" + qstrFileName + ".txt";
	}

	#ifdef WIN32
	{
		// replace \ with /
		qstrFileName.replace("/", "\\", Qt::CaseInsensitive);
	}
	#endif

	QFile file(qstrFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       return;

    QTextStream out(&file);
    out << qstrComment;

	file.close();

	emit(SavedComment());
}

// ***********************************************************************************************
// get the app settings from registry / ini
// ***********************************************************************************************

void Recorder::GetSettings()
{
	// get settings

	// create our settings object
	QSettings voiceLogSettings;

	// get the recording settings
	qstrRecPath = voiceLogSettings.value("config/recording/Path").toString();			// the path of the recordings - usually app directory
	qstrRecName = voiceLogSettings.value("config/recording/Name").toString();			// record file name

	intThreshTimer = voiceLogSettings.value("config/recording/Timer").toInt();	// under threshold timer

	// get the user settings
	qstrCompany = voiceLogSettings.value("config/user/Company").toString();				// company name
	qstrDept = voiceLogSettings.value("config/user/Department").toString();										// department
	qstrUser = voiceLogSettings.value("config/user/Username").toString();				// username

	// get general settings
	intAutoStart = voiceLogSettings.value("config/general/AutoStart").toInt();				// auto start
	intAllowConfig = voiceLogSettings.value("config/general/AllowConfig").toInt();				// allow access to configuration
	intAllowDeactivate = voiceLogSettings.value("config/general/AllowDeactivate").toInt();		// allow access to deactivation
	intAllowPlayback = voiceLogSettings.value("config/general/AllowPlayback").toInt();			// allow playback of recordings
	intGoStealth = voiceLogSettings.value("config/general/GoStealth").toInt();				// go into stealth mode - set process id different and no gui

	intComments = voiceLogSettings.value("config/general/Comments").toInt();

	// get fmod settings
	// reset device to -1
	intFMODRecDevice = -1;

	// get settings
	qstrFMODDeviceDriver = voiceLogSettings.value("fmod/device").toString();					// the device driver
	intFMODRecDevice = voiceLogSettings.value("fmod/record").toInt();						// record driver
	intFMODPlayDevice = voiceLogSettings.value("fmod/playback").toInt();					// playback driver

}

// ***********************************************************************************************
// create the filename for every iteration of recording
// ***********************************************************************************************

void Recorder::MakeRecordName()
{
	// create our settings object
	QSettings voiceLogSettings;

	// get the recording settings
	qstrRecPath = voiceLogSettings.value("config/recording/Path").toString();			// the path of the recordings - usually app directory
	qstrRecName = voiceLogSettings.value("config/recording/Name").toString();			// record file name

	// get the user settings
	qstrCompany = voiceLogSettings.value("config/user/Company").toString();				// company name
	qstrDept = voiceLogSettings.value("config/user/Department").toString();										// department
	qstrUser = voiceLogSettings.value("config/user/Username").toString();				// username

	qstrRecNameFinal = qstrRecName;

	// get date / time
	QString qstrDateTime = QDateTime::currentDateTime().toString(QString("ddMMyyyyhhmmss"));
	qstrLastDateTime = QDateTime::currentDateTime().toString(QString("dd MMMM yyyy hh:mm:ss AP"));

	// replace date if required
	qstrRecNameFinal.replace(QString("%T"), qstrDateTime, Qt::CaseInsensitive);

	// replace company if required
	qstrRecNameFinal.replace(QString("%C"), qstrCompany, Qt::CaseInsensitive);

	// replace department if required
	qstrRecNameFinal.replace(QString("%D"), qstrDept, Qt::CaseInsensitive);

	// replace username if required
	qstrRecNameFinal.replace(QString("%U"), qstrUser, Qt::CaseInsensitive);

	QString qstrRecPathFinal;

	if(qstrRecPath.right(1) == "/")
	{
		qstrRecPathFinal = qstrRecPath + qstrRecNameFinal;
	}
	else
	{
		qstrRecPathFinal = qstrRecPath + "/" + qstrRecNameFinal;
	}

	#ifdef WIN32
	{
		// replace \ with /
		qstrRecPathFinal.replace("/", "\\", Qt::CaseInsensitive);
	}
	#endif

	QByteArray qbyteTemp(qstrRecPathFinal.toLatin1());
	char *charRecPath = qbyteTemp.data();

	fmodSound->setRecordFile(charRecPath);

	// save the date and time data for the comments
}

// ***********************************************************************************************
// Show dialog to enter user password
// ***********************************************************************************************

void Recorder::EnterUserPassword()
{
	// create dialog
	QDialog *passwordDialog = new QDialog;

	userPassButton = new QLineEdit();
	userPassButton->setFixedWidth(180);
	userPassButton->setEchoMode(QLineEdit::Password);

	// Buttons
	QHBoxLayout *hButtonBox = new QHBoxLayout;

	// create ok button
	QPushButton *okButton = new QPushButton(tr("&Ok"));
	okButton->setDefault(true);

	// set ok button to default and check if passwords match
	connect(okButton, SIGNAL(clicked()), this, SLOT(CheckPassMatches()));
	connect(this, SIGNAL(passwordMatches()), passwordDialog, SLOT(accept()));

	hButtonBox->addWidget(okButton, 1, Qt::AlignHCenter);

	// layout for options
	QGridLayout *gBoxPass = new QGridLayout;
	gBoxPass->addWidget(userPassButton, 0, 0);

	QVBoxLayout *vBox = new QVBoxLayout(passwordDialog);
    vBox->addLayout(gBoxPass);			// password stuff
    vBox->addLayout(hButtonBox);		// ok and cancel buttons

	passwordDialog->setLayout(vBox);

	passwordDialog->setGeometry(200,200,200,40);

	passwordDialog->setWindowTitle("Enter Password");
	passwordDialog->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	passwordDialog->exec();
}

// ***********************************************************************************************
// Checks if user passwords match
// ***********************************************************************************************

void Recorder::CheckPassMatches()
{

	// check if passwords match
	if(userPassButton->text() > "")
	{
		// get sha1 hash
		SHA1* sha1 = new SHA1();

		// get sha1 hash of old password
		QByteArray newPassHash = userPassButton->text().toAscii();

		sha1->addBytes(newPassHash.data(), strlen(newPassHash.data()));
		QString shaDigest = sha1->getDigest();
		shaDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

		delete sha1;

		// get password from registry
		QSettings voiceLogSettings;

		QString oldPassDigest = voiceLogSettings.value("config/user/Password").toString();				// get password
	//		voiceLogSettings.setValue("config/user/Password", shaDigest);				// set password

		// check if passwords match
		if(shaDigest.left(20) == oldPassDigest.left(20))
		{
			userPassed = true;
			emit(passwordMatches());
			setWindowIcon(QIcon(QPixmap(":/images/icons/appIconSecure.png")));
			return;
		}
		else
		{
			userPassed = false;
			emit(passwordMatches());
			setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));
		}
	}
	else
	{
		emit(passwordMatches());
	}
}

