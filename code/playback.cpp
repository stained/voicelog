//
//	Application:	Voice Logger - Standard
//
//	File:			playback.cpp
//	Purpose:		playback recordings
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#include "playback.h"

PlayBack::PlayBack(QWidget *parent)
	: QWidget(parent)
{
}

PlayBack::~PlayBack()
{
}

// ***********************************************************************************************
// Show Playback window
// ***********************************************************************************************

void PlayBack::ShowPlayBack()
{

	// create dialog
	QDialog *window = new QDialog;

	// PlayBack Logo header
	QPixmap *pixLogo = new QPixmap;

	// load logo image from file
	pixLogo->load(":/images/cst_playback_logo.png");

	// show logo file on label
	QLabel *labelPlayBackLogo = new QLabel;
	labelPlayBackLogo->setPixmap(*pixLogo);
	labelPlayBackLogo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	labelPlayBackLogo->setLineWidth(2);

	// get recording directory
	QSettings playBackSettings;

	// get the recording settings
	qstrRecPath = playBackSettings.value("config/recording/Path").toString();			// the path of the recordings - usually app directory

	// get fmod settings
	// reset device to -1
	intFMODRecDevice = -1;

	// get device settings
	qstrFMODDeviceDriver = playBackSettings.value("fmod/device").toString();					// the device driver
	intFMODRecDevice = playBackSettings.value("fmod/record").toInt();						// record driver
	intFMODPlayDevice = playBackSettings.value("fmod/playback").toInt();					// playback driver

	// init file list
	qtwFileList = new QTableWidget(6, 8, this);

    QStringList labels;
    labels << tr("I") << tr("Date") << tr("Recording") << tr("Length") << tr("Type") << tr("Contact") << tr("Number") << tr("Comment");
    qtwFileList->setHorizontalHeaderLabels(labels);

    qtwFileList->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    qtwFileList->horizontalHeader()->setClickable(true);
    qtwFileList->horizontalHeader()->setMovable(true);

	// resize sections
    qtwFileList->horizontalHeader()->resizeSection(0, 22);
    qtwFileList->horizontalHeader()->resizeSection(1, 140);
    qtwFileList->horizontalHeader()->resizeSection(2, 145);
    qtwFileList->horizontalHeader()->resizeSection(3, 55);
    qtwFileList->horizontalHeader()->resizeSection(4, 55);
    qtwFileList->horizontalHeader()->resizeSection(5, 60);
    qtwFileList->horizontalHeader()->resizeSection(6, 60);
    qtwFileList->horizontalHeader()->resizeSection(7, 100);

    qtwFileList->verticalHeader()->hide();
    qtwFileList->setShowGrid(true);
	qtwFileList->setSortingEnabled (true);

	connect(qtwFileList, SIGNAL(cellClicked(int, int)), this, SLOT(SetCell(int, int)));
	connect(qtwFileList, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(PlayFile()));

	// get file listing
	loadRecordings();

	// clear empty row
    qtwFileList->removeRow(qtwFileList->rowCount() - 1);

	// Buttons
	QHBoxLayout *hButtonBox = new QHBoxLayout;
	hButtonBox->addStretch(1);

	// create play button
	playButton = new QPushButton(tr("&Play"));
	playButton->setEnabled(false);

	// set ok button to default and accept
	connect(playButton, SIGNAL(clicked()), this, SLOT(PlayFile()));

	// create info button
	propButton = new QPushButton(tr("P&roperties"));
	connect(propButton, SIGNAL(clicked()), this, SLOT(GetProperties()));
	propButton->setDefault(false);
	propButton->setEnabled(false);

	// create delete button
	delButton = new QPushButton(tr("&Delete"));
	connect(delButton, SIGNAL(clicked()), this, SLOT(DeleteFile()));
	delButton->setDefault(false);
	delButton->setEnabled(false);

	// create ok button
	QPushButton *okButton = new QPushButton(tr("&Ok"));
	connect(okButton, SIGNAL(clicked()), window, SLOT(reject()));
	okButton->setDefault(false);

	hButtonBox->addWidget(playButton);
	hButtonBox->addWidget(propButton);
	hButtonBox->addWidget(delButton);
	hButtonBox->addWidget(okButton);

	// everything
	QVBoxLayout *vBox = new QVBoxLayout(window);
	vBox->addWidget(labelPlayBackLogo);
	vBox->addWidget(qtwFileList);
    vBox->addLayout(hButtonBox);		// ok and cancel buttons

	window->setLayout(vBox);
	playButton->setDefault(true);

	window->setGeometry(200,200,400,300);
	window->setWindowTitle("VoiceLog1 Recording PlayBack");
	window->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	window->exec();
}

// ***********************************************************************************************
// Load the recording file data
// ***********************************************************************************************

void PlayBack::loadRecordings()
{
	// init our table to 0 rows
    qtwFileList->setRowCount(1);

	if(qstrRecPath.right(1) != "/")
	{
		qstrRecPath = qstrRecPath + "/";
	}

	#ifdef WIN32
	{
		// replace \ with /
		qstrRecPath.replace("/", "\\", Qt::CaseInsensitive);
	}
	#endif

	// set directory
	QDir qdirRecPath = QDir(qstrRecPath, "*.txt");

	QStringList fileList = qdirRecPath.entryList();
	QString fileName;

	// create a progress bar for loading
	QDialog *progress = new QDialog(this);

	// label of num files
	QLabel *numFiles = new QLabel(tr(""));

	QProgressBar *progressBar = new QProgressBar();
	progressBar->setRange(0, fileList.size() - 1);

	// everything
	QVBoxLayout *vBox = new QVBoxLayout(progress);
	vBox->addWidget(numFiles);
	vBox->addWidget(progressBar);

	progress->setLayout(vBox);

	progress->setGeometry(400,400,300,50);
	progress->setWindowTitle("Loading Playback...");
	progress->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

    progress->show();
    progress->raise();
    progress->activateWindow();

	// load file data
    for (int i = 0; i < fileList.size(); ++i)
    {
		numFiles->setText("File: " + QString("%1").arg(i) + " of " +  QString("%1").arg(fileList.size() - 1)  + " - " + fileList.at(i));

		progressBar->setValue(i);
		qApp->processEvents();

		fileName = qstrRecPath + fileList.at(i).toLocal8Bit().constData();

		// open txt file for data
		QFile file(fileName);

	    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		// get data
	    QByteArray fileData = file.readAll();

		// parse data
		parseFileData(fileData);

		// close file
		file.close();
	}
    progress->hide();

}

// ***********************************************************************************************
// Parse the tags from our file
// ***********************************************************************************************

void PlayBack::parseFileData(QString fileData)
{
	// get filename
	QStringList qstrFileName;

	if(fileData.contains("<file>") == true)
	{
		qstrFileName = fileData.split("<file>");
		qstrFileName = qstrFileName.at(1).split("</file>");
	}
	else
	{
		qstrFileName << tr("");
	}

	// get hash
	QStringList qstrFileHash;
	if(fileData.contains("<hash>") == true)
	{
		qstrFileHash = fileData.split("<hash>");
		qstrFileHash = qstrFileHash.at(1).split("</hash>");
	}
	else
	{
		qstrFileHash << tr("");
	}

	// get length
	QStringList qstrFileLen;
	if(fileData.contains("<size>") == true)
	{
		qstrFileLen = fileData.split("<size>");
		qstrFileLen = qstrFileLen.at(1).split("</size>");
	}
	else
	{
		qstrFileLen << tr("");
	}

	// get date
	QStringList qstrFileDate;
	if(fileData.contains("<date>") == true)
	{
		qstrFileDate = fileData.split("<date>");
		qstrFileDate = qstrFileDate.at(1).split("</date>");
	}
	else
	{
		qstrFileDate << tr("");
	}

	// get type
	QStringList qstrFileType;
	if(fileData.contains("<type>") == true)
	{
		qstrFileType = fileData.split("<type>");
		qstrFileType = qstrFileType.at(1).split("</type>");
	}
	else
	{
		qstrFileType << tr("");
	}

	// get contact person
	QStringList qstrContactPerson;
	if(fileData.contains("<contact_person>") == true)
	{
		qstrContactPerson = fileData.split("<contact_person>");
		qstrContactPerson = qstrContactPerson.at(1).split("</contact_person>");
	}
	else
	{
		qstrContactPerson << tr("");
	}

	// get contact number
	QStringList qstrContactNumber;
	if(fileData.contains("<comment>") == true)
	{
		qstrContactNumber = fileData.split("<contact_number>");
		qstrContactNumber = qstrContactNumber.at(1).split("</contact_number>");
	}
	else
	{
		qstrContactNumber << tr("");
	}

	// get comment
	QStringList qstrComment;
	if(fileData.contains("<comment>") == true)
	{
		qstrComment = fileData.split("<comment>\n");
		qstrComment = qstrComment.at(1).split("</comment>");
	}
	else
	{
		qstrComment << tr("");
	}

	// get integrity
	QStringList qstrIntegrity;
	if(fileData.contains("<integrity>") == true)
	{
		qstrIntegrity = fileData.split("<integrity>");
		qstrIntegrity = qstrIntegrity.at(1).split("</integrity>");
	}
	else
	{
		qstrIntegrity << tr("");
	}

	// set integrity
	int fileIntegrity = 1;

	// if we have a file hash and hash matches
	if(qstrFileHash.at(0) != "")
	{
		// check recording to see if hash matches

		// get file hash
		QFile file(qstrRecPath + qstrFileName.at(0));

	    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

	    QByteArray fileToHash = file.readAll();

		file.close();

		SHA1* shaRec = new SHA1();

		shaRec->addBytes(fileToHash.data(), fileToHash.size());
		QString shaRecDigest = shaRec->getDigest();

		delete shaRec;
		shaRecDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

		// hash match :)
		if(qstrFileHash.at(0).left(20) == shaRecDigest.left(20))
		{
			fileIntegrity = 2;
		}
	}

	// if integrity hash matches
	if(qstrIntegrity.at(0) != "" && fileIntegrity == 2)
	{
		QStringList qstrIntegrityTest = fileData.split("<integrity>");

		QByteArray qbyteIntegrityHash(qstrIntegrityTest.at(0).toLatin1());

		SHA1* shaInteg = new SHA1();

		shaInteg->addBytes(qbyteIntegrityHash.data(), qbyteIntegrityHash.size());
		QString shaIntegDigest = shaInteg->getDigest();

		delete shaInteg;
		shaIntegDigest.replace(QString(" "), QString(""), Qt::CaseInsensitive);

		// if equal, then hash matches
		if(qstrIntegrity.at(0).left(20) == shaIntegDigest.left(20))
		{
			fileIntegrity = 3;
		}
	}

	// create file table data
	int row = qtwFileList->rowCount();
	qtwFileList->insertRow(row);

	// integrity
    QTableWidgetItem *wiIntegrity = new QTableWidgetItem(tr(""));

	QString integrityFile = QString(":/images/icons/integrity%1.png").arg(fileIntegrity);
	wiIntegrity->setIcon(QIcon(QPixmap(integrityFile)));
    wiIntegrity->setFlags(Qt::ItemIsEnabled);

	// date
	QDateTime qdtFileDate = QDateTime::fromString(qstrFileDate.at(0), QString("dd MMMM yyyy hh:mm:ss AP"));
	QString qstrFileDate2 = qdtFileDate.toString(QString("dd/MM/yyyy hh:mm:ss AP"));

   	QTableWidgetItem *wiDate = new QTableWidgetItem(qstrFileDate2);
    wiDate->setFlags(Qt::ItemIsEnabled);

	// filename
   	QTableWidgetItem *wiFileName = new QTableWidgetItem(qstrFileName.at(0));
    wiFileName->setFlags(Qt::ItemIsEnabled);

	// file len
   	QTableWidgetItem *wiFileLen = new QTableWidgetItem(qstrFileLen.at(0));
    wiFileLen->setFlags(Qt::ItemIsEnabled);

	// file type
   	QTableWidgetItem *wiFileType = new QTableWidgetItem(qstrFileType.at(0));
    wiFileType->setFlags(Qt::ItemIsEnabled);

	// contact person
   	QTableWidgetItem *wiContactPerson = new QTableWidgetItem(qstrContactPerson.at(0));
    wiContactPerson->setFlags(Qt::ItemIsEnabled);

	// contact number
   	QTableWidgetItem *wiContactNumber = new QTableWidgetItem(qstrContactNumber.at(0));
    wiContactNumber->setFlags(Qt::ItemIsEnabled);

	// Comment
   	QTableWidgetItem *wiComment = new QTableWidgetItem(qstrComment.at(0));
    wiComment->setFlags(Qt::ItemIsEnabled);
	wiComment->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);

	// show all
    qtwFileList->setItem(row - 1, 0, wiIntegrity);
    qtwFileList->setItem(row - 1, 1, wiDate);
    qtwFileList->setItem(row - 1, 2, wiFileName);
    qtwFileList->setItem(row - 1, 3, wiFileLen);
    qtwFileList->setItem(row - 1, 4, wiFileType);
    qtwFileList->setItem(row - 1, 5, wiContactPerson);
    qtwFileList->setItem(row - 1, 6, wiContactNumber);
    qtwFileList->setItem(row - 1, 7, wiComment);
}

// ***********************************************************************************************
// Get all file data from the table
// ***********************************************************************************************

void PlayBack::GetProperties()
{
	QString qstrFileData;
	QStringList fileHeaders;

	fileHeaders << tr("<b>Date:</b>") << tr("<b>File Name:</b>") << tr("<b>Length:</b>")<< tr("<b>Type:</b>")<< tr("<b>Contact:</b>")<< tr("<b>Number:</b>")<< tr("&nbsp;</td></tr><tr><td><b>Comment:</b>");

	qstrFileData = "<table>";

	// create the file data
	for(int i = 1; i < 8; i++)
	{
		QTableWidgetItem *item = qtwFileList->item(currentRow, i);

		if(item->text() != "")
		{
			qstrFileData = qstrFileData + "<tr><td>" + fileHeaders.at(i - 1) + "</td><td>&nbsp;</td><td>" + item->text() + "</td></tr>";
		}
	}

	qstrFileData = qstrFileData + "</table>";

	qstrFileData.replace(QString("\n"), QString("<br />"), Qt::CaseInsensitive);


	QMessageBox *information = new QMessageBox;

	information->setText(qstrFileData);
	information->setWindowTitle("Recording Properties");
	information->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));
	information->setFixedWidth(300);
	information->setIcon(QMessageBox::Information);
	information->show();
}

// ***********************************************************************************************
// enable buttons and set the current cell index
// ***********************************************************************************************

void PlayBack::SetCell(int row, int col)
{
	if(qtwFileList->rowCount() > 0)
	{

		currentRow = row;
		currentCol = col;

		playButton->setEnabled(true);
		propButton->setEnabled(true);
		delButton->setEnabled(true);
	}
}

// ***********************************************************************************************
// delete recording
// ***********************************************************************************************

void PlayBack::DeleteFile()
{
	if(qtwFileList->rowCount() > 0)
	{
		// get filename
		QTableWidgetItem *item = qtwFileList->item(currentRow, 2);
		QString qstrFileName = item->text();
		QString qstrFileNameProp = qstrFileName.left(qstrFileName.size() - 3) + "txt";

	    QMessageBox mb("Delete Recording",
	    	"Are you sure you want to delete " + qstrFileName + "?",
	    	 QMessageBox::Question,
	         QMessageBox::Yes,
	         QMessageBox::No | QMessageBox::Escape | QMessageBox::Default,
	         QMessageBox::NoButton);

		mb.setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

	    if (mb.exec() == QMessageBox::Yes)
	    {
			QDir directory;

			// delete properties
			directory.remove(qstrRecPath + qstrFileNameProp);
			// delete recording
			bool fileDeleted = directory.remove(qstrRecPath + qstrFileName);

			if(fileDeleted == true)
			{
				qtwFileList->removeRow(currentRow);

				// no files, so disable our buttons
				if(qtwFileList->rowCount() == 0)
				{
					playButton->setEnabled(false);
					propButton->setEnabled(false);
					delButton->setEnabled(false);
				}
			}
			else
			{
				QMessageBox::warning(this, "Delete Recording", "The recording: " + qstrFileName + " cannot be deleted...");
			}

		}

	}
}

// ***********************************************************************************************
// playback recording
// ***********************************************************************************************

void PlayBack::PlayFile()
{
	// if we have a valid file to load
	if(qtwFileList->rowCount() > 0)
	{
		// create dialog
		QDialog *playWindow = new QDialog();
		playWindow->setGeometry(300,300,200,150);

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

		// vu meter
		// create layout for VU
		QVBoxLayout *vBoxVU = new QVBoxLayout;

		// create VU Meter
		vuMeter = new VUMeter(this);
		vuMeter->setFixedSize(playWindow->width(),120);

		// add our playback slider
		vuSlider = new QSlider(Qt::Horizontal);
		connect(vuSlider, SIGNAL(valueChanged(int)), this, SLOT(FModDoPos(int)));

		//connect our vu meter to the sound buffer
		connect(fmodSound, SIGNAL(waveChanged(int)), vuMeter, SLOT(setVU(int)));

		// check sound length
		connect(fmodSound, SIGNAL(lenChanged(int)), vuMeter, SLOT(setLen(int)));
		connect(fmodSound, SIGNAL(lenChanged(int)), vuSlider, SLOT(setValue(int)));

		// check file sound length
		connect(fmodSound, SIGNAL(lenFileChanged(int)), vuMeter, SLOT(setFileLen(int)));
		connect(fmodSound, SIGNAL(lenFileChanged(int)), this, SLOT(SetSliderLen(int)));

		// get file to play
		QTableWidgetItem *item = qtwFileList->item(currentRow, 2);
		QString qstrFileName = qstrRecPath + item->text();

		QByteArray qbyteTemp(qstrFileName.toLatin1());
		char *charPlayPath = qbyteTemp.data();

		// init playback
		fmodSound->initPlayback(charPlayPath);

		vBoxVU->addWidget(vuMeter);
		vBoxVU->addWidget(vuSlider);

		// playback controls
		QHBoxLayout *hControlBox = new QHBoxLayout;
		hControlBox->addStretch(1);

		// create play button
		playControl = new QPushButton(QIcon(QPixmap(":/images/controls/play.png")), tr(""));
		connect(playControl, SIGNAL(clicked()), vuMeter, SLOT(showPlayback()));
		connect(playControl, SIGNAL(clicked()), this, SLOT(FModDoPlay()));
		playControl->setDefault(true);
		playControl->setFixedSize(28, 28);

		// create pause button
		pauseControl = new QPushButton(QIcon(QPixmap(":/images/controls/pause.png")), tr(""));
		connect(pauseControl, SIGNAL(clicked()), this, SLOT(FModDoPause()));
		pauseControl->setDefault(false);
		pauseControl->setFixedSize(28, 28);

		// create Stop button
		stopControl = new QPushButton(QIcon(QPixmap(":/images/controls/stop.png")), tr(""));
		connect(stopControl, SIGNAL(clicked()), this, SLOT(FModDoStop()));
		stopControl->setDefault(false);
		stopControl->setFixedSize(28, 28);

		// create speed down button
		speedDnControl = new QPushButton(QIcon(QPixmap(":/images/controls/speeddn.png")), tr(""));
		connect(speedDnControl, SIGNAL(clicked()), this, SLOT(FModDoSpeedDn()));
		speedDnControl->setDefault(false);
		speedDnControl->setFixedSize(28, 28);

		// create speed up button
		speedUpControl = new QPushButton(QIcon(QPixmap(":/images/controls/speedup.png")), tr(""));
		connect(speedUpControl, SIGNAL(clicked()), this, SLOT(FModDoSpeedUp()));
		speedUpControl->setDefault(false);
		speedUpControl->setFixedSize(28, 28);

		// create eject button
		QPushButton *ejectControl = new QPushButton(QIcon(QPixmap(":/images/controls/eject.png")), tr(""));
		connect(ejectControl, SIGNAL(clicked()), this, SLOT(FModDoStop()));
		connect(ejectControl, SIGNAL(clicked()), playWindow, SLOT(reject()));
		ejectControl->setDefault(false);
		ejectControl->setFixedSize(28, 28);

		// disable all buttons to start
		playControl->setEnabled(true);
		pauseControl->setEnabled(false);
		stopControl->setEnabled(false);
		speedDnControl->setEnabled(false);
		speedUpControl->setEnabled(false);
		vuSlider->setEnabled(false);

		// add buttons to layout
		hControlBox->addWidget(playControl);
		hControlBox->addWidget(pauseControl);
		hControlBox->addWidget(stopControl);
		hControlBox->addWidget(speedDnControl);
		hControlBox->addWidget(speedUpControl);
		hControlBox->addWidget(ejectControl);

		// everything
		QVBoxLayout *vBox = new QVBoxLayout(playWindow);
		vBox->addLayout(vBoxVU);
		vBox->addLayout(hControlBox);

		playWindow->setLayout(vBox);
		playControl->setDefault(true);

		playWindow->setWindowTitle(item->text());
		playWindow->setWindowIcon(QIcon(QPixmap(":/images/icons/appIcon.png")));

		// kill any music if around
		connect(playWindow, SIGNAL(destroyed()), this, SLOT(KillWindow()));

		playWindow->setAttribute(Qt::WA_DeleteOnClose);

		playWindow->exec();
	}
}

// ***********************************************************************************************
// Load file and play :)
// ***********************************************************************************************

void PlayBack::FModDoPlay()
{
	fmodSound->soundPlay();

	// re-enable buttons as required
	playControl->setEnabled(false);
	pauseControl->setEnabled(true);
	stopControl->setEnabled(true);
	speedDnControl->setEnabled(true);
	speedUpControl->setEnabled(true);
	vuSlider->setEnabled(true);
}

// ***********************************************************************************************
// pause sond playback
// ***********************************************************************************************

void PlayBack::FModDoPause()
{
	fmodSound->soundPause();

	// re-enable buttons as required
	playControl->setEnabled(true);
	pauseControl->setEnabled(false);
	stopControl->setEnabled(true);
	speedDnControl->setEnabled(false);
	speedUpControl->setEnabled(false);
	vuSlider->setEnabled(true);
}

// ***********************************************************************************************
// stop playback
// ***********************************************************************************************

void PlayBack::FModDoStop()
{
	fmodSound->soundStop();

	// re-enable buttons as required
	playControl->setEnabled(true);
	pauseControl->setEnabled(false);
	stopControl->setEnabled(false);
	speedDnControl->setEnabled(false);
	speedUpControl->setEnabled(false);
	vuSlider->setEnabled(false);
}

// ***********************************************************************************************
// lower frequency of sound
// ***********************************************************************************************

void PlayBack::FModDoSpeedDn()
{
	fmodSound->soundSpeedDn();

	// re-enable buttons as required
	playControl->setEnabled(false);
	pauseControl->setEnabled(true);
	stopControl->setEnabled(true);
	speedDnControl->setEnabled(true);
	speedUpControl->setEnabled(true);
	vuSlider->setEnabled(true);
}

// ***********************************************************************************************
// increase frequency of sound
// ***********************************************************************************************

void PlayBack::FModDoSpeedUp()
{
	fmodSound->soundSpeedUp();

	// re-enable buttons as required
	playControl->setEnabled(false);
	pauseControl->setEnabled(true);
	stopControl->setEnabled(true);
	speedDnControl->setEnabled(true);
	speedUpControl->setEnabled(true);
	vuSlider->setEnabled(true);
}


// ***********************************************************************************************
// Moves the cursor position of the playback
// ***********************************************************************************************

void PlayBack::FModDoPos(int pos)
{
	fmodSound->soundPosition(pos);

	// re-enable buttons as required
	playControl->setEnabled(false);
	pauseControl->setEnabled(true);
	stopControl->setEnabled(true);
	speedDnControl->setEnabled(true);
	speedUpControl->setEnabled(true);
	vuSlider->setEnabled(true);
}

// ***********************************************************************************************
// sets the slider min and max values
// ***********************************************************************************************

void PlayBack::SetSliderLen(int len)
{
	vuSlider->setRange(0, len);
}

// ***********************************************************************************************
// Deletes any sound which might have been playing
// ***********************************************************************************************

void PlayBack::KillWindow()
{
	delete fmodSound;
//	fmodSound->soundStop();
}
