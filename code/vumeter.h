#ifndef VUMETER_H
#define VUMETER_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <math.h>
#include <QApplication>
#include <QDateTime>
#include <QSettings>

class VUMeter : public QWidget
{
    Q_OBJECT

    public:
        VUMeter(QWidget *parent = 0);
        ~VUMeter();

    public slots:
		void setVU(int point);
		void setLen(int len);
		void setFileLen(int len);

		void showRecord();
		void hideRecord();
		void showPlayback();

    private slots:
    	void updateThresholdTimer();

    signals:
		void overThreshold();
		void underThreshold();
		void dtmfCode(char *code);

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);

    private:
		// draws the audio grid in the background
		void drawGrid(QPainter &painter);

		// draws our actual sound level
		void drawLen(QPainter &painter);

		// draws our actual sound level
		void drawVU(QPainter &painter);

		// draws the threshold line
		void drawThreshold(QPainter &painter);

		// draws the record light
		void drawRecord(QPainter &painter);

		// checks if the audio is over the threshold
		void checkThreshold();

		// VU painter
		int oldXPoint, newXPoint, oldYPoint, newYPoint, VUMode;
		int newLen;
		int newFileLen;

		// current mouse position
		int mousePosX, mousePosY;

		// VUMode x / y / width locations
		int thresHoldData[3][3];

		// are we recording
		bool isRecording;

		// are we in playback mode
		bool isPlaying;

		// threshold timeout in seconds
		int thresHoldTimeout;

		// our timer to check if we're under threshold
		QTimer *thresHoldTimer;
		int underThresholdTimer;
		bool emittedThreshold;



};

#endif
