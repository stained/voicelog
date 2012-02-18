//
//	Application:	Voice Logger - Standard
//
//	File:			fmod.h
//	Purpose:		low-level sound system interface
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//

#ifndef FMOD_H
#define FMOD_H

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
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QApplication>

// FMOD Specific
#include "api/inc/fmod.h"
#include "api/inc/fmod_errors.h"

// mp3 coding capabilities
#include "BladeMP3EncDLL.h"

// hashing facility
#include "../sha1/sha1.h"

#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
    #define __PACKED                         /* dummy */
#else
    #define __PACKED __attribute__((packed)) /* gcc packed */
#endif

class FMod : public QWidget
{
	Q_OBJECT

	public:
		FMod(QWidget *parent = 0);
		~FMod();

		// init FMOD System
		void initSystem();
		int setRecordFile(char* recordFile);
		void setDriver(char* deviceDriver, int recDriver, int playDriver);
		float getLastDataLen();
		QString getFileHash();

		int initPlayback(char* fileName);

		void soundPlay();				// play sound from disk
		void soundPause();				// pause sound
		void soundStop();				// stop playback
		void soundSpeedDn();			// slow playback
		void soundSpeedUp();			// speedup playback

		void soundPosition(int pos);	// reposition sound cursor

		QString getTelNum();

	public slots:
		// our configuration page;
		void showConfig();

		// input monitor
		void startMonitor();
		void stopMonitor();

		// recording tools
		int startRecordToFile();
		void stopRecordToFile();

	signals:
		void waveChanged(int waveForm);
		void freqChanged(float frequency);
		void dtmfCode(char* code);
		void waveLen(int waveLen);
		void SavedSettings();
		void lenChanged(int filelen);
		void lenFileChanged(int filelen);
		void playBackDone();

	private slots:
		// our method to get the devices for each driver
		void listDrivers(bool checked);

		// our method to set the devices for each driver
		void setDriver();

		// do recording based on timer of 10ms
		void timedRecordToFile();

		// show spectrum during playback
		void timedSpectrum();

	protected:

	private:
		// error checking
		int errCheck(FMOD_RESULT result);
		// dtmf decoding
		void DTMF(float spectrum[8192]);
		short *dtmfBuffer;

		// dtmf timer
		int dtmfTimer;
		int dtmfInitialTimer;
		char* lastDTMFCode;

		QString telNum;
		QString lastTelNum;
		QString qstrAllDTMF;

		// FMOD specific variables
   		FMOD_SYSTEM          	*system;
    	FMOD_SOUND           	*sound;
    	FMOD_SOUND           	*soundDTMF;

   		FMOD_RESULT          	result;
   		FMOD_CREATESOUNDEXINFO 	exinfo;
   		FMOD_CREATESOUNDEXINFO 	exinfoDTMF;

   		FMOD_CHANNEL			*channel;
   		FMOD_CHANNEL			*channelDTMF;

   		int                    	key, driver, numdrivers, count, bin;
    	unsigned int           	version;
    	FILE                  	*fp;
    	unsigned int          	datalength, soundlength;
    	float					lastDataLen;

		short					*dtmfPtr;
		int						dtmfPtrLen;

		// file hash
		QString fileHash;


		// mp3 encoding

		// wav writing function
		void writeWavHeader(FILE *fp, FMOD_SOUND *sound, int length);

		// our current driver
		int fmodDriver;

		// configuration specific widgets

		// audio settings
		QSettings audioSettings;

		// radio buttons for drivers - win32
        QRadioButton *radDS;
        QRadioButton *radMM;
        QRadioButton *radAS;

		// device buttons (lists)
		QComboBox *comboRecDev;
		QComboBox *comboPlayDev;

		// check if driver works
		bool booDriver;

		// record file
		char* recFile;

		// actually record to file
		bool beginRecordToFile;

		// record timer
		QTimer *recordTimer;

		// playback timer
		QTimer *spectrumTimer;

		// the saved device driver data
		char savedDeviceDriver[30];
		int savedRecDevice, savedPlayDevice;

		// dtmf
		#define OUTPUTRATE          44100
		#define SPECTRUMSIZE        8192
		#define SPECTRUMRANGE       ((float)OUTPUTRATE / 2.0f)      /* 0 to nyquist */

		#define BINSIZE      		(SPECTRUMRANGE / (float)SPECTRUMSIZE)

		// lame enc
		bool enableMP3;

		typedef struct
		{
		    FILE               *mFP;
			HINSTANCE	        mDLL;
			HBE_STREAM	        hbeStream;
			PBYTE               pMP3Buffer;
			PSHORT              pWAVBuffer;
			BE_VERSION	        Version;
		    DWORD		        dwMP3Buffer;
		    DWORD               dwSamples;
		    BEINITSTREAM        beInitStream;
		    BEENCODECHUNK       beEncodeChunk;
		    BEDEINITSTREAM      beDeinitStream;
		    BECLOSESTREAM       beCloseStream;
		    BEVERSION           beVersion;
		    BEWRITEVBRHEADER    beWriteVBRHeader;

		} outputmp3_state;

		outputmp3_state *state;

		// mp3 capabilities
		DWORD dwRead;
		DWORD dwWrite;
		DWORD dwDone;
		DWORD dwFileSize;
	    BE_ERR err;

		// playback
		bool paused;

		// test
		QFile testFile;


};

#endif


