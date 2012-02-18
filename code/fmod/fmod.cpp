//
//	Application:	Voice Logger - Standard
//
//	File:			fmod.cpp
//	Purpose:		low-level sound system interface
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#include "fmod.h"

FMod::FMod(QWidget *parent)
	: QWidget(parent)
{
	// init vars
	sound = 0;
	system = 0;

	savedRecDevice = -1;

	// we dont want to start recording just yet :)
	beginRecordToFile = false;
	recFile = "";
	// dtmf stuff
	dtmfTimer = 0;
	lastDTMFCode = "";

	// Create a System object and initialize.
	result = FMOD_System_Create(&system);
	errCheck(result);

	result = FMOD_System_GetVersion(system, &version);
	errCheck(result);

	if (version < FMOD_VERSION)
	{
		QMessageBox::critical(0, "FMOD SubSystem Error", QString("You are using an old version of FMOD %1. \n This program requires %2")
	        														.arg(version)
	        														.arg(FMOD_VERSION)
																	);
	}
}

FMod::~FMod()
{
	// Shut down
    result = FMOD_Sound_Release(sound);
    errCheck(result);

    result = FMOD_System_Release(system);
    errCheck(result);
}

// ***********************************************************************************************
// Initialize the FMOD system
// ***********************************************************************************************

void FMod::initSystem()
{
	// init vars
	sound = 0;
	system = 0;

	savedRecDevice = -1;

	// we dont want to start recording just yet :)
	beginRecordToFile = false;
	recFile = "";
	// dtmf stuff
	dtmfTimer = 0;
	lastDTMFCode = "";

	// Create a System object and initialize.
	result = FMOD_System_Create(&system);
	errCheck(result);

	result = FMOD_System_GetVersion(system, &version);
	errCheck(result);

	if (version < FMOD_VERSION)
	{
		QMessageBox::critical(0, "FMOD SubSystem Error", QString("You are using an old version of FMOD %1. \n This program requires %2")
	        														.arg(version)
	        														.arg(FMOD_VERSION)
																	);
	}

	result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
	errCheck(result);
}

// ***********************************************************************************************
// Initialize the FMOD drivers
// ***********************************************************************************************

void FMod::setDriver(char* deviceDriver, int recDriver, int playDriver)
{
	// set global device
	strcpy(savedDeviceDriver, deviceDriver);
	savedRecDevice = recDriver;
	savedPlayDevice = playDriver;

	// check which driver is used
	#ifdef WIN32
	{
		if(deviceDriver == "FMOD_OUTPUTTYPE_DSOUND")
		{
        	result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_DSOUND);
	    }

		if(deviceDriver == "FMOD_OUTPUTTYPE_WINMM")
		{
        	result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_WINMM);

		}

		if(deviceDriver == "FMOD_OUTPUTTYPE_ASIO")
		{
        	result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_ASIO);
		}

	}
	#endif

	// set playback driver
   	result = FMOD_System_SetDriver(system, playDriver);
   	errCheck(result);

	// set record driver
    result = FMOD_System_SetRecordDriver(system, recDriver);
    errCheck(result);


}

// ***********************************************************************************************
// some basic error checking
// ***********************************************************************************************

int FMod::errCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        QMessageBox::critical(0, "FMOD SubSystem Error", QString("%1 \n %2")
        														.arg(result)
        														.arg(FMOD_ErrorString(result))
        														);
        // there was an error, FMOD doesnt work
        return(-1);
    }

	// no error, can continue
	return 0;
}

// ***********************************************************************************************
// Create wav file
// ***********************************************************************************************

void FMod::writeWavHeader(FILE *fp, FMOD_SOUND *sound, int length)
{
    int	channels, bits;
    float rate;

    if (!sound)
    {
        return;
    }

    fseek(fp, 0, SEEK_SET);


    FMOD_Sound_GetFormat  (sound, 0, 0, &channels, &bits);
    FMOD_Sound_GetDefaults(sound, &rate, 0, 0, 0);

    {
        #if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
        #pragma pack(1)
        #endif


        // WAV Structures

        typedef struct
        {
	        signed char id[4];
	        int 		size;
        } RiffChunk;

        struct
        {

            RiffChunk       chunk           __PACKED;
            unsigned short	wFormatTag      __PACKED;    // format type
            unsigned short	nChannels       __PACKED;    // number of channels (i.e. mono, stereo...)
            unsigned int	nSamplesPerSec  __PACKED;    // sample rate
            unsigned int	nAvgBytesPerSec __PACKED;    // for buffer estimation
            unsigned short	nBlockAlign     __PACKED;    // block size of data
            unsigned short	wBitsPerSample  __PACKED;    // number of bits per sample of mono data

        } FmtChunk  = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, (int)rate, (int)rate * channels * bits / 8, 1 * channels * bits / 8, bits } __PACKED;

        struct
        {
            RiffChunk   chunk;
        } DataChunk = { {{'d','a','t','a'}, length } };
        struct
        {
            RiffChunk   chunk;
	        signed char rifftype[4];
        } WavHeader = { {{'R','I','F','F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + length }, {'W','A','V','E'} };

        #if defined(WIN32) || defined(_WIN64) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
        #pragma pack()
        #endif

        // Write out the WAV header.
        fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
        fwrite(&FmtChunk, sizeof(FmtChunk), 1, fp);
        fwrite(&DataChunk, sizeof(DataChunk), 1, fp);

		// get actual file length using formula:
		// datalength (ie file size in bytes) * 8 (to get bits) / bits (16) / channels (2) / rate (ie 44100)  = seconds

		lastDataLen = ((((datalength * 8) / bits) / channels) / rate);
   	}
}

// ***********************************************************************************************
// Show Driver configuration
// ***********************************************************************************************
void FMod::showConfig()
{
	// if on windows platform, show supported fmod windows drivers
	#ifdef WIN32
	{
		// create dialog
		QDialog *window = new QDialog;

			// FMOD Logo header
			QPixmap *pixLogo = new QPixmap;

			// load logo image from file
			pixLogo->load(":/images/fmod_logo.png");

			// show logo file on label
			QLabel *labelFMODLogo = new QLabel;
			labelFMODLogo->setPixmap(*pixLogo);
			labelFMODLogo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			labelFMODLogo->setLineWidth(2);

			// show windows driver choices
			QGroupBox *groupBoxDrivers = new QGroupBox(tr("Sound Drivers"));

		    radDS = new QRadioButton(tr("&Direct Sound"));
		    radMM = new QRadioButton(tr("&Windows MM"));
		    radAS = new QRadioButton(tr("&ASIO"));

			// create layout for drivers
			QVBoxLayout *vBoxDrive = new QVBoxLayout;

		    vBoxDrive->addWidget(radDS);
		    vBoxDrive->addWidget(radMM);
		    vBoxDrive->addWidget(radAS);

	        groupBoxDrivers->setLayout(vBoxDrive);

			// show windows devices
			QGroupBox *groupBoxDevices = new QGroupBox(tr("Sound Devices"));

				// List record devices
				QLabel *labelRecDev = new QLabel;
				labelRecDev->setText("&Recording Device");

				comboRecDev = new QComboBox();
				comboRecDev->setFixedWidth(200);

				// link labelRecDev to comboRecDev (for kb access)
				labelRecDev->setBuddy(comboRecDev);

				// List playback devices
				QLabel *labelPlayDev = new QLabel;
				labelPlayDev->setText("&Playback Device");

				comboPlayDev = new QComboBox();
				comboPlayDev->setFixedWidth(200);

				// link labelPlayDev to comboPlayDev (for kb access)
				labelPlayDev->setBuddy(comboPlayDev);

				// layout for devices
				QGridLayout *gBoxDev = new QGridLayout;
				gBoxDev->addWidget(labelRecDev, 0, 0);
				gBoxDev->addWidget(comboRecDev, 0, 1);
				gBoxDev->addWidget(labelPlayDev, 1, 0);
				gBoxDev->addWidget(comboPlayDev, 1, 1);

	        groupBoxDevices->setLayout(gBoxDev);

			// create slots for getting driver devices on radio button change
			connect(radDS, SIGNAL(toggled(bool)), this, SLOT(listDrivers(bool)));
			connect(radMM, SIGNAL(toggled(bool)), this, SLOT(listDrivers(bool)));
			connect(radAS, SIGNAL(toggled(bool)), this, SLOT(listDrivers(bool)));


			// check if we have any saved devices
			if(savedRecDevice != -1)
			{
				if(strcmp(savedDeviceDriver, "FMOD_OUTPUTTYPE_DSOUND") == 0)
				{
		        	radDS->setChecked(true);
	 		   	}

				if(strcmp(savedDeviceDriver,  "FMOD_OUTPUTTYPE_WINMM") == 0)
				{
			        radMM->setChecked(true);
				}

				else if(strcmp(savedDeviceDriver,  "FMOD_OUTPUTTYPE_ASIO") == 0)
				{
		        	radAS->setChecked(true);
				}

			}

			// no saved devices, so make default
			else
			{
				// set default to Windows MM
		        radMM->setChecked(true);
			}

		// our complete configuration layout
		QHBoxLayout *hBox = new QHBoxLayout;

		hBox->addWidget(groupBoxDrivers);
		hBox->addWidget(groupBoxDevices);

			// Buttons
			QHBoxLayout *hButtonBox = new QHBoxLayout;
			hButtonBox->addStretch(1);

			// create ok button
			QPushButton *okButton = new QPushButton(tr("&Ok"));
			okButton->setDefault(true);

			// connect button accept slot to set sound device
			connect(okButton, SIGNAL(clicked()), this, SLOT(setDriver()));

			// set ok button to default and accept
			connect(okButton, SIGNAL(clicked()), window, SLOT(accept()));

			// create cancel button
			QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
			connect(cancelButton, SIGNAL(clicked()), window, SLOT(reject()));

			hButtonBox->addWidget(okButton);
			hButtonBox->addWidget(cancelButton);

		// everything
		QVBoxLayout *vBox = new QVBoxLayout(window);
		vBox->addWidget(labelFMODLogo);
        vBox->addLayout(hBox);				// drivers / devices
        vBox->addLayout(hButtonBox);		// ok and cancel buttons

		window->setLayout(vBox);

		window->setGeometry(200,200,400,50);
		window->setWindowTitle("FMOD Sound Configuration");
		window->exec();


	}
	#endif

	//return fmodDriver;
}

// ***********************************************************************************************
// lists devices in combo boxes based on selected drivers
// ***********************************************************************************************

void FMod::listDrivers(bool checked)
{
	// check which radio button was checked
	#ifdef WIN32
	{
		if(radDS->isChecked() && checked == true)
		{
        	result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_DSOUND);
	    }

		if(radMM->isChecked() && checked == true)
		{
        	result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_WINMM);
		}

		if(radAS->isChecked() && checked == true)
		{
        	result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_ASIO);
		}

	}
	#endif

    if(errCheck(result) != 0)
    {
		return;
	}

	// clear devices
	comboRecDev->clear();

	booDriver = true;

	// get recording devices
    result = FMOD_System_GetRecordNumDrivers(system, &numdrivers);

    if(errCheck(result) != 0)
    {
		return;
	}

	// if there are actually playback devices
	if(numdrivers > 0)
	{
		for (count=0; count < numdrivers; count++)
		{
	        char name[256];

	        result = FMOD_System_GetRecordDriverName(system, count, name, 256);

	        if(errCheck(result) != 0)
	        {
				return;
			}

			QString strName  = QString("%1") .arg(name);

			// if item not already in list and not empty
			if(comboRecDev->findText(strName) == -1 && strName.size() > 1)
			{
				comboRecDev->addItem(strName);
				comboRecDev->setDisabled(false);
			}

			// if name empty - disable recording device
			if(strName.size() <= 1)
			{
				comboRecDev->addItem("No recording devices for this driver");
				comboRecDev->setDisabled(true);
			}
		}
    }
	else
	{
		// no devices - disable
		comboRecDev->addItem("No recording devices for this driver");
		comboRecDev->setDisabled(true);
		booDriver = false;

	}

	// clear devices
	comboPlayDev->clear();

	// get playback devices
    result = FMOD_System_GetNumDrivers(system, &numdrivers);

    if(errCheck(result) != 0)
    {
		return;
	}

	// if there are actually playback devices
	if(numdrivers > 0)
	{
		for (count=0; count < numdrivers; count++)
		{
		        char name[256];

		        result = FMOD_System_GetDriverName(system, count, name, 256);

		        if(errCheck(result) != 0)
		        {
					return;
				}

				QString strName  = QString("%1") .arg(name);

				// if item not already in list and not empty
				if(comboPlayDev->findText(strName) == -1 && strName.size() > 1)
				{
					comboPlayDev->addItem(strName);
					comboPlayDev->setDisabled(false);
				}

				// if name empty - disable playback device
				if(strName.size() <= 1 || strName == "")
				{
					comboPlayDev->addItem("No playback devices for this driver");
					comboPlayDev->setDisabled(true);
				}

   		}
	}
	else
	{
		// no devices - disable :)
		comboPlayDev->addItem("No playback devices for this driver");
		comboPlayDev->setDisabled(true);
		booDriver = false;
	}

	// check if our drivers are saved
	if(savedRecDevice != -1)
	{
		comboPlayDev->setCurrentIndex(savedPlayDevice);
		comboRecDev->setCurrentIndex(savedRecDevice);
	}
}

// ***********************************************************************************************
// Sets up the driver devices for FMod
// ***********************************************************************************************

void FMod::setDriver()
{


	// if booDriver is true - setDevices
	if(booDriver == true)
	{
		// set record driver
    	result = FMOD_System_SetRecordDriver(system, comboRecDev->currentIndex());
    	errCheck(result);

		// set playback driver
   		result = FMOD_System_SetDriver(system, comboPlayDev->currentIndex());
   		errCheck(result);

		char* deviceDriver = "none";

		// get which device driver to use
		#ifdef WIN32
		{
			if(radDS->isChecked())
			{
	        	deviceDriver = "FMOD_OUTPUTTYPE_DSOUND";
		    }

			if(radMM->isChecked())
			{
	        	deviceDriver = "FMOD_OUTPUTTYPE_WINMM";
			}

			if(radAS->isChecked())
			{
	        	deviceDriver = "FMOD_OUTPUTTYPE_ASIO";
			}

		}
		#endif

		// save driver settings
		audioSettings.setValue("fmod/device", deviceDriver)					;			// the device driver
		audioSettings.setValue("fmod/record", comboRecDev->currentIndex());				// record driver
		audioSettings.setValue("fmod/playback", comboPlayDev->currentIndex());			// playback driver

		emit(SavedSettings());
	}
	else
	{
		QMessageBox::information(0, "FMod System", QString("No valid devices found"));
	}

}

// ***********************************************************************************************
// Our wav/mp3 file which we record to
// ***********************************************************************************************

int FMod::setRecordFile(char* recordFile)
{
	recFile = recordFile;

	return 0;
}

// ***********************************************************************************************
// Record to disk function
// ***********************************************************************************************

int FMod::startRecordToFile()
{
	if(enableMP3 == true)
	{
		strcat(recFile, ".mp3");
    	fp = fopen(recFile, "wb");
	}
	else
	{
		strcat(recFile, ".wav");
    	fp = fopen(recFile, "wb");
	}

 	// check if file exists
    if (!fp)
    {
		QMessageBox::information(this, "FMod System Error", QString("Could not open %1 for writing") .arg(recFile));
        return 1;
    }

	// if not mp3
	if(enableMP3 == false)
	{
		// Write out the wav header.  As we don't know the length yet it will be 0.
	    writeWavHeader(fp, sound, datalength);
	}

	beginRecordToFile = true;
	datalength = 0;
	soundlength = 0;
	telNum = "";
	qstrAllDTMF = "";

	dtmfTimer = 0;
	dtmfInitialTimer = 0;
	lastDTMFCode = "";
	dtmfBuffer = 0;

	return 0;
}

// ***********************************************************************************************// Our basic input monitor
//	Starts the record monitor
// ***********************************************************************************************

void FMod::startMonitor()
{
	// initialize system
	initSystem();


	// ***********************************************
	// wave format
	// ***********************************************

	// create file attributes
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = 2;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = 44100;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 2;

	// ***********************************************
	// mp3 format
	// ***********************************************

	BE_CONFIG	     beConfig = {0,};

    state = (outputmp3_state *)calloc(sizeof(outputmp3_state), 1);

	state->mDLL = LoadLibrary("lame_enc.dll");

	if (!state->mDLL)
	{
		QMessageBox::information(this, "Lame Enc", "Can't open lame_enc.dll, mp3 encoding disabled");
		enableMP3 = false;
	}
	else
	{
		enableMP3 = true;

		state->beInitStream	    = (BEINITSTREAM)     GetProcAddress(state->mDLL, TEXT_BEINITSTREAM);
		state->beEncodeChunk	= (BEENCODECHUNK)    GetProcAddress(state->mDLL, TEXT_BEENCODECHUNK);
		state->beDeinitStream	= (BEDEINITSTREAM)   GetProcAddress(state->mDLL, TEXT_BEDEINITSTREAM);
		state->beCloseStream	= (BECLOSESTREAM)    GetProcAddress(state->mDLL, TEXT_BECLOSESTREAM);
		state->beWriteVBRHeader = (BEWRITEVBRHEADER) GetProcAddress(state->mDLL, TEXT_BEWRITEVBRHEADER);
	}


	// Check if all interfaces are present
	if(!state->beInitStream || !state->beEncodeChunk || !state->beDeinitStream || !state->beCloseStream || !state->beWriteVBRHeader)
	{
		QMessageBox::information(this, "Lame Enc", "Error accessing Lame Enc interface, mp3 encoding disabled");
		enableMP3 = false;
	}

	if(enableMP3 == true)
	{

	    memset(&beConfig,0,sizeof(beConfig));					// clear all fields


		// use the LAME config structure
		beConfig.dwConfig = BE_CONFIG_LAME;

		beConfig.format.LHV1.dwStructVersion	= 1;
		beConfig.format.LHV1.dwStructSize		= sizeof(beConfig);
		beConfig.format.LHV1.dwSampleRate		= 44100;           		// INPUT FREQUENCY
		beConfig.format.LHV1.dwReSampleRate		= 0;					// DON"T RESAMPLE
		beConfig.format.LHV1.nMode				= BE_MP3_MODE_STEREO;	// OUTPUT IN STREO
		beConfig.format.LHV1.dwBitrate			= 128;					// MINIMUM BIT RATE
		beConfig.format.LHV1.nPreset			= LQP_VOICE_QUALITY;		    // QUALITY PRESET SETTING
		beConfig.format.LHV1.dwMpegVersion		= MPEG1;				// MPEG VERSION (I or II)
		beConfig.format.LHV1.dwPsyModel			= 0;					// USE DEFAULT PSYCHOACOUSTIC MODEL
		beConfig.format.LHV1.dwEmphasis			= 0;					// NO EMPHASIS TURNED ON
		beConfig.format.LHV1.bOriginal			= TRUE;					// SET ORIGINAL FLAG

		// Init the MP3 Stream
		err = state->beInitStream(&beConfig, &state->dwSamples, &state->dwMP3Buffer, &state->hbeStream);

		// Check result
		if(err != BE_ERR_SUCCESSFUL)
		{
			QMessageBox::information(this, "Lame Enc", "Error initializing Lame Encoder, mp3 encoding disabled");
			enableMP3 = false;
		}
		// Allocate MP3 buffer
		state->pMP3Buffer = (PBYTE)malloc(state->dwMP3Buffer);

		if(!state->pMP3Buffer)
		{
			QMessageBox::information(this, "Lame Enc", "Error allocating memory to Lame Encoder, mp3 encoding disabled");
			enableMP3 = false;
		}

	}

    result = FMOD_System_CreateSound(system, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
    errCheck(result);

    result = FMOD_System_RecordStart(system, sound, TRUE);
    errCheck(result);

    result = FMOD_Sound_GetLength(sound, &soundlength, FMOD_TIMEUNIT_PCM);
    errCheck(result);

/*
	// file for testing frequency bins
	QString qstrTestFileName = QString("%1_test.txt").arg(recFile);
	testFile.setFileName(qstrTestFileName);

    if (!testFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
*/

	telNum = "";
	dtmfTimer = 0;
	lastDTMFCode = "";

	// create a timer to do the recording
	recordTimer = new QTimer(this);
	connect(recordTimer, SIGNAL(timeout()), this, SLOT(timedRecordToFile()));
	recordTimer->start(35);
}

// ***********************************************************************************************// Our basic input monitor
//	Stops the record monitor
// ***********************************************************************************************

void FMod::stopMonitor()
{

	// stop timer
	recordTimer->stop();

	// Shut down
	if(sound)
	{
//    	result = FMOD_Sound_Release(sound);
//    	errCheck(result);
	}

	if(system)
	{
//    	result = FMOD_System_Release(system);
//    	errCheck(result);
	}

    	result = FMOD_System_Close(system);
    	errCheck(result);

}


// ***********************************************************************************************
// Does the actual file record based on a timer of 10ms
// ***********************************************************************************************

void FMod::timedRecordToFile()
{


 	static unsigned int lastrecordpos = 0;
	unsigned int recordpos = 0;

	FMOD_System_GetRecordPosition(system, &recordpos);
	errCheck(result);

	// pointers to the pcm sample
	short *wptr = 0;
	int ptrLen = 0;

	// mp3 capabilities
	dwRead=0;
	dwWrite=0;
	dwDone=0;
	dwFileSize=0;

	if (recordpos != lastrecordpos)
	{

	   	void *ptr1, *ptr2;
	    int blocklength;
	    unsigned int len1, len2;

	    blocklength = (int)recordpos - (int)lastrecordpos;

	   	if (blocklength < 0)
	    {
	   	  	blocklength += soundlength;
	    }

		// Lock the sound to get access to the raw data.
	    FMOD_Sound_Lock(sound, lastrecordpos * exinfo.numchannels * 2, blocklength * exinfo.numchannels * 2, &ptr1, &ptr2, &len1, &len2);   // * exinfo.numchannels * 2 = stereo 16bit.  1 sample = 4 bytes.

 		// determine incoming levels
	    if (ptr1 && len1)
	 	{
			wptr = (short *)ptr1;
			ptrLen = len1;
		}

	    if (ptr2 && len2)
	 	{
			wptr = (short *)ptr2;
			ptrLen = len2;
		}


		if(ptrLen > 1)
		{

			// ***************************************************************
			// DO NOT DELETE
			// ***************************************************************

  	    	memset(&exinfoDTMF, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    		exinfoDTMF.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    		exinfoDTMF.length           = ptrLen;
 			exinfoDTMF.numchannels      = 2;
 	 		exinfoDTMF.format           = FMOD_SOUND_FORMAT_PCM16;
    		exinfoDTMF.defaultfrequency = 44100;

			static float spectrum[SPECTRUMSIZE];

			// create a custom dtmf sound based on the last second of audio recorded
	 	    result = FMOD_System_CreateSound(system, (const char *)wptr, FMOD_SOFTWARE | FMOD_OPENMEMORY | FMOD_OPENRAW , &exinfoDTMF, &soundDTMF);
	 	    errCheck(result);

			// play the last second of recorded audio
		    result = FMOD_System_PlaySound(system, FMOD_CHANNEL_REUSE, soundDTMF, FALSE, &channelDTMF);
		    errCheck(result);

		    // Dont hear what is being recorded otherwise it will feedback.  Spectrum analysis is done before volume scaling in the DSP chain
		    result = FMOD_Channel_SetVolume(channelDTMF, 0);
		    errCheck(result);

			// get spectrum for last second
		    result = FMOD_Channel_GetSpectrum(channelDTMF, spectrum, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
		    errCheck(result);

			// get dtmf
			DTMF(spectrum);


			// loop to test which bin contains which freq
/*			int loopSpec;
			float binHz = ((float)(OUTPUTRATE / 2) / (float)SPECTRUMSIZE);

			QTextStream out(&testFile);
			for(loopSpec = 0; loopSpec < SPECTRUMSIZE; loopSpec++)
			{
				if(spectrum[loopSpec] > 0.01f)
				{
    				 out << "bin: " << loopSpec << " freq: " << (float)binHz * loopSpec << " val: " << spectrum[loopSpec] << "\r\n";
				}
			}

			out << "\r\n";
*/
		}

	  	// Write it to disk.(if over threshold)
	    if(beginRecordToFile == true)
	    {
			if(enableMP3 == true)
			{
				dwRead = (short)ptrLen / 2;

				err = state->beEncodeChunk(state->hbeStream, dwRead, wptr, state->pMP3Buffer, &dwWrite);

				// Check result
				if(err != BE_ERR_SUCCESSFUL)
				{
					// ignore err messages as we dont know the whether the pc can handle encoding properly
				}
				datalength += fwrite(state->pMP3Buffer, 1, dwWrite, fp);

				emit(lenChanged(datalength / 16000));
			}
			else
			{
		 		// write wav file
		 		datalength += fwrite(wptr, 1, ptrLen, fp);
				emit(lenChanged(datalength));
			}
		}



		// emit the first sound level of the sample
		emit(waveChanged((int)wptr[1]));

		// Unlock the sound to allow FMOD to use it again.
	    FMOD_Sound_Unlock(sound, ptr1, ptr2, len1, len2);
	}

	lastrecordpos = recordpos;

    FMOD_System_Update(system);


}



// ***********************************************************************************************
// Stops all disk recording
// ***********************************************************************************************

void FMod::stopRecordToFile()
{
	beginRecordToFile = false;
	if(enableMP3 == true)
	{

		// Deinit the stream
		err = state->beDeinitStream(state->hbeStream, state->pMP3Buffer, &dwWrite);

		if(err != BE_ERR_SUCCESSFUL)
		{
			// close the MP3 Stream
			state->beCloseStream( state->hbeStream );
		}

		// Are there any bytes returned from the DeInit call?
		// If so, write them to disk

		if( dwWrite )
		{
			if( fwrite( state->pMP3Buffer, 1, dwWrite, fp ) != dwWrite )
			{
				//return FMOD_ERR_FILE_BAD;
			}
		}

		state->beWriteVBRHeader(recFile);

	    if (!sound)
	    {
	        return;
	    }

		// save file len
		lastDataLen = datalength / 16000; // 16000 bytes per sec or 960 000 bytes per min
	}
	else
	{
		// Write back the wav header now that we know its length.
		writeWavHeader(fp, sound, datalength);
	}

	// close file
	fclose(fp);

	// get file hash
	QFile file(recFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QByteArray fileToHash = file.readAll();

	SHA1* sha1 = new SHA1();

	sha1->addBytes(fileToHash.data(), fileToHash.size());
	QString shaDigest = sha1->getDigest();

	delete sha1;
	file.close();

	fileHash = shaDigest;

	// save last tel number
	lastTelNum = telNum;
}

// ***********************************************************************************************
// Returns the last datalength (actual time of file)
// ***********************************************************************************************

float FMod::getLastDataLen()
{
	return lastDataLen;
}

// ***********************************************************************************************
// Returns the sha1 file hash of the recording (for tamper prevention)
// ***********************************************************************************************

QString FMod::getFileHash()
{
	return fileHash;
}

// ***********************************************************************************************
// Finds the DTMF tone corresponding to the min and max frequencies
// ***********************************************************************************************

void FMod::DTMF(float spectrum[8192])
{
	// increase timer
	dtmfInitialTimer++;

	// check low bins (freqs)
	int lowBins[4] =
	{
		240,
		261,
		288,
		320
	};


	// check high bins (freqs)
	int highBins[4] =
	{
		416,
		459,
		505,
		560
	};

  	char* row_col_ascii_codes[4][4] = {
  	  {"1", "2", "3", "A"},
  	  {"4", "5", "6", "B"},
  	  {"7", "8", "9", "C"},
  	  {"*", "0", "#", "D"}};

	// check low freq bins
	int col = -1;
	int loopLowBins;

	for(loopLowBins = 0; loopLowBins <= 3; loopLowBins++)
	{
		// check bins 10 freq each way
		int loopLowBinsCheck;

		for(loopLowBinsCheck = lowBins[loopLowBins] - 5; loopLowBinsCheck <= lowBins[loopLowBins] + 5; loopLowBinsCheck++)
		{

			// if freq exists
			if(spectrum[loopLowBinsCheck] > 0.03f)
			{
				col = loopLowBins;
				break;
			}
		}
	}

	// check high freq bins
	int row = -1;
	int loopHighBins;

	for(loopHighBins = 0; loopHighBins <= 3; loopHighBins++)
	{
		// check bins 10 freq each way
		int loopHighBinsCheck;

		for(loopHighBinsCheck = highBins[loopHighBins] - 5; loopHighBinsCheck <= highBins[loopHighBins] + 5; loopHighBinsCheck++)
		{
			// if freq and energy high enough
			if(spectrum[loopHighBinsCheck] > 0.03f)
			{
				row = loopHighBins;
				break;
			}
		}
	}


	if(row > -1 && col > -1)
	{
		if(dtmfTimer < 4 && lastDTMFCode != row_col_ascii_codes[col][row])
		{
			lastDTMFCode = row_col_ascii_codes[col][row];
			dtmfTimer = 0;
		}

		if(dtmfTimer < 4 && row_col_ascii_codes[col][row] == lastDTMFCode)
		{
			dtmfTimer++;
		}

		if (dtmfTimer == 4 && row_col_ascii_codes[col][row] == lastDTMFCode)
		{
			// check first 10 seconds for telephone number
			if(dtmfInitialTimer < 300)
			{
				// we have a match
				telNum = telNum + QString("%1").arg(row_col_ascii_codes[col][row]);
			}

			// we have a match
			qstrAllDTMF = qstrAllDTMF + QString("%1").arg(row_col_ascii_codes[col][row]);

			lastDTMFCode = "";
			dtmfTimer = 0;
		}

	}


	// no match
	return;
}


// ***********************************************************************************************
// returns the last telnumber
// ***********************************************************************************************
QString FMod::getTelNum()
{
	return lastTelNum;
}

// ***********************************************************************************************
// initalizes playback
// ***********************************************************************************************

int FMod::initPlayback(char* fileName)
{

	// initialize system
	initSystem();

	// ***********************************************
	// wave format
	// ***********************************************

	// create file attributes
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = 2;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = 44100;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 2;

	result = FMOD_System_CreateSound(system, fileName, FMOD_SOFTWARE | FMOD_ACCURATETIME, 0, &sound);
	errCheck(result);

	// unpause
	paused = false;

	return -1;
}

// ***********************************************************************************************
// Plays a sound from disk
// ***********************************************************************************************

void FMod::soundPlay()
{
	// if sound not paused, then play from scratch
	if(paused == false)
	{
    	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_REUSE, sound, false, &channel);
//    	errCheck(result);

    	result = FMOD_Sound_GetLength(sound, &soundlength, FMOD_TIMEUNIT_MS);
//    	errCheck(result);

		emit(lenFileChanged(soundlength / 1000));

		// create a timer to display playback waveform
		spectrumTimer = new QTimer(this);
		connect(spectrumTimer, SIGNAL(timeout()), this, SLOT(timedSpectrum()));
		spectrumTimer->start(50);
	}
	else
	{
		result = FMOD_Channel_SetPaused(channel, false);
//	    errCheck(result);
		paused = false;
	}
}

// ***********************************************************************************************
// Displays the spectrum of current playing file
// ***********************************************************************************************

void FMod::timedSpectrum()
{
	FMOD_BOOL IsPlaying = -1;

	FMOD_Channel_IsPlaying(channel, &IsPlaying);

	if(IsPlaying == 1)
	{
		static float spectrum[SPECTRUMSIZE];
		unsigned int ms = 0;

    	result = FMOD_Sound_GetLength(sound, &soundlength, FMOD_TIMEUNIT_MS);
//    	errCheck(result);

		FMOD_Channel_GetPosition(channel, &ms, FMOD_TIMEUNIT_MS);
//		errCheck(result);

		if(ms < soundlength)
		{
			ms = ms / 1000;		// get seconds

			emit(lenChanged(ms));

			result = FMOD_Channel_GetWaveData(channel, spectrum, SPECTRUMSIZE, 0);
//		    errCheck(result);

			float db = spectrum[1] * 20000;
			emit(waveChanged((int)db));

			FMOD_Channel_GetPosition(channel, &ms, FMOD_TIMEUNIT_MS);
//			errCheck(result);

		    FMOD_System_Update(system);
		}

		// stop timer and emit song fin signal
		else
		{
			spectrumTimer->stop();
			emit(playBackDone());
		}
	}
}

// ***********************************************************************************************
// Pauses a playing sound
// ***********************************************************************************************

void FMod::soundPause()
{
	result = FMOD_Channel_SetPaused(channel, true);
//    errCheck(result);

	paused = true;
}

// ***********************************************************************************************
// stops a playing sound and resets counter to 0
// ***********************************************************************************************

void FMod::soundStop()
{
	result = FMOD_Channel_Stop(channel);
//    errCheck(result);
	spectrumTimer->stop();

	paused = false;

	emit(waveChanged(0));
	emit(lenChanged(0));
	emit(playBackDone());
}

// ***********************************************************************************************
// slows down the sound playback
// ***********************************************************************************************

void FMod::soundSpeedDn()
{
	float freq;

	// get current frequency
	result = FMOD_Channel_GetFrequency(channel, &freq);
//    errCheck(result);

	// speed down by 1000 Hz
	result = FMOD_Channel_SetFrequency(channel, freq - 1000);
//    errCheck(result);

}

// ***********************************************************************************************
// speeds up the sound playback
// ***********************************************************************************************

void FMod::soundSpeedUp()
{
	float freq;

	// get current frequency
	result = FMOD_Channel_GetFrequency(channel, &freq);
//    errCheck(result);

	// speed up by 1000 Hz
	result = FMOD_Channel_SetFrequency(channel, freq + 1000);
//    errCheck(result);

}


// ***********************************************************************************************
// Moves the position of the sound cursor
// ***********************************************************************************************

void FMod::soundPosition(int pos)
{
	result = FMOD_Channel_SetPosition(channel, pos * 1000, FMOD_TIMEUNIT_MS);
//    errCheck(result);

}


