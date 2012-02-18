//
//	Application:	Voice Logger - Standard
//
//	File:			vu.c
//	Purpose:		VU Meter Widget
//
//	Author:			Theo Ireton
//
//	Copyright (c):	2006 Coldshift Technologies
//
//


#include "vumeter.h"

// ***********************************************************************************************
// class entry point
// ***********************************************************************************************

VUMeter::VUMeter(QWidget *parent)
	: QWidget(parent)
{

	mousePosX = 0;
	mousePosY = 0;

	// reset VU Thresholds

	// do VUMode 1
	thresHoldData[0][0] = 0;
	thresHoldData[0][1] = 0;
	thresHoldData[0][2] = 0;

	// do VUMode 2
	thresHoldData[1][0] = 0;
	thresHoldData[1][1] = 0;
	thresHoldData[1][2] = 0;

	// do VUMode 3
	thresHoldData[2][0] = 0;
	thresHoldData[2][1] = 0;
	thresHoldData[2][2] = 0;

	// check if we have any settings saved
	QSettings vuSettings;

	// default VU Mode
	VUMode = vuSettings.value("VU/defaultVU").toInt();			// default VU Meter Modew

	// no VU Mode saved
	if(VUMode < 1 || VUMode > 3)
	{
		VUMode = 1;			// our default vu mode - lines
	}

	// settings for mode 1
	thresHoldData[0][0] = vuSettings.value("VU/VU1/X").toInt();
	thresHoldData[0][1] = vuSettings.value("VU/VU1/Y").toInt();;
	thresHoldData[0][2] = vuSettings.value("VU/VU1/Z").toInt();;

	// settings for mode 2
	thresHoldData[1][0] = vuSettings.value("VU/VU2/X").toInt();
	thresHoldData[1][1] = vuSettings.value("VU/VU2/Y").toInt();
	thresHoldData[1][2] = vuSettings.value("VU/VU2/Z").toInt();

	// settings for mode 3
	thresHoldData[2][0] = vuSettings.value("VU/VU3/X").toInt();
	thresHoldData[2][1] = vuSettings.value("VU/VU3/Y").toInt();
	thresHoldData[2][2] = vuSettings.value("VU/VU3/Z").toInt();

	// not recording initially
	isRecording = false;
	emittedThreshold = false;
	underThresholdTimer = -1;

	// no playback initially
	isPlaying = false;

	// current len
	newLen = -1;
	newFileLen = 0;

	// our under threshold timeout

	thresHoldTimeout = 10;				// 10 seconds
	thresHoldTimeout = vuSettings.value("config/recording/Timer").toInt();;

	thresHoldTimer = new QTimer(this);	// set our threshold timeout
	underThresholdTimer = -1;

	connect(thresHoldTimer, SIGNAL(timeout()), this, SLOT(updateThresholdTimer()));

	setPalette(QPalette(QColor(0, 0, 0)));
	setAutoFillBackground(true);


	// reset points
	oldXPoint = 0;
	oldYPoint = 0;
	newXPoint = 0;
	newYPoint = 0;

	update();
}

// ***********************************************************************************************
// class exit point - cleanup
// ***********************************************************************************************

VUMeter::~VUMeter()
{

}

// ***********************************************************************************************
// actual painter object
// ***********************************************************************************************

void VUMeter::paintEvent(QPaintEvent *)
{

	QPainter painter(this);
	painter.translate(-0, -0);

	painter.setPen(QColor(0, 0, 0));


	drawGrid(painter);

	if(isPlaying == false && newYPoint > 0)
	{
		drawThreshold(painter);

		// check if our new VU values go over our threshold
		checkThreshold();
	}

	if(isRecording == true)
	{
		// show record light
		drawRecord(painter);

		// timer
		drawLen(painter);
	}

	if(isPlaying == true)
	{
		// timer
		drawLen(painter);
	}

	// need to reset the pen style after our threshold drawing
	painter.setPen(Qt::NoPen);

	painter.setRenderHint(QPainter::Antialiasing, true);

	// draw audio level
	drawVU(painter);

}

// ***********************************************************************************************
// checks if we've gone over our threshold
// ***********************************************************************************************

void VUMeter::checkThreshold()
{
	// check which VU meter we're working with
	switch(VUMode)
	{
		case 1:
		{
			// if threshold defined
			if(thresHoldData[0][1] > 0)
			{
				// compare the Y points with our threshold
				if(newYPoint < thresHoldData[0][1])
				{
					if(emittedThreshold == false)
					{
						emit(overThreshold());
					}

					emittedThreshold = true;

					// over threshold, so tell everyone :)
					thresHoldTimer->stop();
					underThresholdTimer = 0;


					return;
				}

				// under threshold
				else if(newYPoint > thresHoldData[0][1])
				{
					if(thresHoldTimer->isActive() == false && (underThresholdTimer == 0))
					{
						// enable our timer to see how long we're under for
						thresHoldTimer->start(1000);
					}
					return;
				}
			}

			break;
		}

		case 2:
		{
			// if threshold defined
			if(thresHoldData[1][0] > 0)
			{
				// compare the X points with our threshold
				if(newXPoint > thresHoldData[1][0])
				{
					if(emittedThreshold == false)
					{
						emit(overThreshold());
					}

					emittedThreshold = true;

					// over threshold, so tell everyone :)
					thresHoldTimer->stop();
					underThresholdTimer = 0;


					return;
				}

				// under threshold
				else if(newXPoint < thresHoldData[1][0])
				{
					if(thresHoldTimer->isActive() == false && (underThresholdTimer == 0))
					{
						// enable our timer to see how long we're under for
						thresHoldTimer->start(1000);
					}
					return;
				}
			}

			break;
		}

		case 3:
		{
			// if threshold defined
			if(thresHoldData[2][2] > 0)
			{
				// compare the radius points with our threshold
				if(newYPoint > (thresHoldData[2][2] * 2))
				{

					if(emittedThreshold == false)
					{
						emit(overThreshold());
					}

					emittedThreshold = true;

					// over threshold, so tell everyone :)
					thresHoldTimer->stop();
					underThresholdTimer = 0;


					return;
				}

				// under threshold
				if(newYPoint < (thresHoldData[2][2] * 2))
				{
					if(thresHoldTimer->isActive() == false && (underThresholdTimer == 0))
					{
						// enable our timer to see how long we're under for
						thresHoldTimer->start(1000);
					}
					return;
				}
			}

			break;
		}
	}
}

// ***********************************************************************************************
// Draws the actual record light in the corner
// ***********************************************************************************************

void VUMeter::drawRecord(QPainter &painter)
{

	// make everything red
	painter.setBrush(QBrush(QColor(255,0,0)));
	painter.setPen(QColor(255, 0, 0));

	// draw circle
	painter.drawEllipse(width() - 15, height() - 15, 10, 10);

	// do text
	painter.setFont(QFont("arial", 8));
	painter.drawText(QPointF(width() - 70, height() - 6), tr("Recording"));

}

// ***********************************************************************************************
// Draws our threshold line on the VU
// ***********************************************************************************************

void VUMeter::drawThreshold(QPainter &painter)
{
	// make nice white lines :)
	painter.setPen(QColor(255, 0, 0));

	switch(VUMode)
	{
		case 1:
		{
			// X lines
			painter.drawLine(0, thresHoldData[0][1] , width(), thresHoldData[0][1]);

			break;
		}

		case 2:
		{
			// Y Lines
			painter.drawLine(thresHoldData[1][0], 0, thresHoldData[1][0], height());
			break;

		}

		case 3:
		{

			// circles
			painter.drawEllipse(thresHoldData[2][0] - (thresHoldData[2][2]), thresHoldData[2][1] - (thresHoldData[2][2]), thresHoldData[2][2] * 2, thresHoldData[2][2] * 2);
			break;
		}

	}
}

// ***********************************************************************************************
// draws the background grid for the VU
// ***********************************************************************************************

void VUMeter::drawGrid(QPainter &painter)
{
	// redraw grid
	int numChannelsY = 15;
	int numChannelsX = 8;

	int numLinesX = width() / numChannelsX;
	int numLinesY = height() / numChannelsY;
	int numCircles = 8;

	switch(VUMode)
	{
		case 1:
		{

			// circles
			for(int loopC = 0; loopC <= numCircles; loopC++)
			{
				painter.setPen(QColor(30, 0, 0));
				painter.drawEllipse((width() / 2 ) - loopC * 20, (height() / 2) - loopC * 20, loopC * 40, loopC * 40);
			}
			// Y lines
			for(int loopX = 0; loopX <= width(); loopX++)
			{
				painter.setPen(QColor(30, 0, 0));
				painter.drawLine(loopX + (numLinesX * loopX), 0, loopX  + (numLinesX * loopX), height());
			}

			// X lines
			for(int loopY = 0; loopY <= height(); loopY++)
			{
				painter.setPen(QColor(100, 0, 0));
				painter.drawLine(0, loopY + (numLinesY * loopY) , width(), loopY  + (numLinesY * loopY));
			}

			break;
		}

		case 2:
		{

			// X lines
			for(int loopY = 0; loopY <= height(); loopY++)
			{
				painter.setPen(QColor(30, 0, 0));
				painter.drawLine(0, loopY + (numLinesY * loopY) , width(), loopY  + (numLinesY * loopY));
			}

			// circles
			for(int loopC = 0; loopC <= numCircles; loopC++)
			{
				painter.setPen(QColor(30, 0, 0));
				painter.drawEllipse((width() / 2 ) - loopC * 20, (height() / 2) - loopC * 20, loopC * 40, loopC * 40);
			}


			// Y lines
			for(int loopX = 0; loopX <= width(); loopX++)
			{
				painter.setPen(QColor(100, 0, 0));
				painter.drawLine(loopX + (numLinesX * loopX), 0, loopX  + (numLinesX * loopX), height());
			}

			break;
		}

		case 3:
		{

			// X lines
			for(int loopY = 0; loopY <= height(); loopY++)
			{
				painter.setPen(QColor(30, 0, 0));
				painter.drawLine(0, loopY + (numLinesY * loopY) , width(), loopY  + (numLinesY * loopY));
			}

			// Y lines
			for(int loopX = 0; loopX <= width(); loopX++)
			{
				painter.setPen(QColor(30, 0, 0));
				painter.drawLine(loopX + (numLinesX * loopX), 0, loopX  + (numLinesX * loopX), height());
			}

			// circles
			for(int loopC = 0; loopC <= numCircles; loopC++)
			{
				painter.setPen(QColor(100, 0, 0));
				painter.drawEllipse((width() / 2 ) - loopC * 20, (height() / 2) - loopC * 20, loopC * 40, loopC * 40);
			}


			break;
		}

	}
}

// ***********************************************************************************************
// Draws the recording time
// ***********************************************************************************************

void VUMeter::drawLen(QPainter &painter)
{
	// make everything red
	painter.setBrush(QBrush(QColor(255,0,0)));
	painter.setPen(QColor(255, 0, 0));

	QDateTime qdtFileLen = QDateTime::fromString(QString("00"), QString("ss"));
	QDateTime qdtFileLen2 = qdtFileLen.addSecs(newLen);

	QString qstrFileLen = qdtFileLen2.toString(QString("hh:mm:ss"));

	// if per chance we're doing playback)
	if(newFileLen > 0)
	{
		QDateTime qdtFullFileLen = QDateTime::fromString(QString("00"), QString("ss"));
		QDateTime qdtFullFileLen2 = qdtFullFileLen.addSecs(newFileLen);

		QString qstrFullFileLen = qdtFullFileLen2.toString(QString("hh:mm:ss"));

		qstrFileLen = qstrFileLen + " / " + qstrFullFileLen;
	}

	painter.setFont(QFont("arial", 8));
	painter.drawText(QPointF(10, height() - 6), qstrFileLen);

}

// ***********************************************************************************************
// Draws the actual audio levels
// ***********************************************************************************************

void VUMeter::drawVU(QPainter &painter)
{

	switch(VUMode)
	{
		case 1:
		{
       		QLinearGradient linearGrad(QPointF(0, newYPoint), QPointF(width(), newYPoint));
       		linearGrad.setColorAt(0, QColor(255, 255, 0));
       		linearGrad.setColorAt(1, QColor(255, 0, 0));

			if(newYPoint < 0)
				newYPoint = 0;

			if(newYPoint > height())
				newYPoint = height();

			painter.setBrush(QBrush(linearGrad));
			painter.drawRect(0, newYPoint, width(), 2);
			break;
		}

		case 2:
		{
     		QLinearGradient linearGrad(QPointF(0, height() / 2), QPointF(newXPoint, height() / 2));
      		linearGrad.setColorAt(0, QColor(255, 255, 0));
      		linearGrad.setColorAt(1, QColor(255, 0, 0));

			if(newXPoint < 0)
				newXPoint = 0;

			if(newXPoint > width())
				newXPoint = width();

			painter.setBrush(QBrush(linearGrad));
			painter.drawRect(0, 0, newXPoint, height());
			break;
		}

		case 3:
		{
			// give a nice radial gradiant
       		QRadialGradient radialGrad(QPointF(width() / 2, height() / 2), newYPoint / 1.5, QPointF(width() / 2, height() / 2));
       		radialGrad.setColorAt(0, QColor(255, 255, 0));
       		radialGrad.setColorAt(1, QColor(255, 0, 0));
			painter.setBrush(QBrush(radialGrad));

			// big circle
			painter.drawEllipse((width() / 2 ) - (newYPoint / 2), (height() / 2) - (newYPoint / 2), newYPoint, newYPoint);

			break;
		}
	}

}

// ***********************************************************************************************
// Gets and sets the audio levels
// ***********************************************************************************************

void VUMeter::setVU(int point)
{
	oldYPoint = newYPoint;
	newYPoint = (height() / 2) + (point / 100);

	newXPoint = point / 10;

	update();
}

// ***********************************************************************************************
// Gets and sets current file playing length
// ***********************************************************************************************

void VUMeter::setLen(int len)
{
	newLen = len;
	update();
}

// ***********************************************************************************************
// Gets and sets file length
// ***********************************************************************************************

void VUMeter::setFileLen(int len)
{
	newFileLen = len;
	update();
}

// ***********************************************************************************************
// Mouse event driver
// ***********************************************************************************************

void VUMeter::mousePressEvent(QMouseEvent *event)
{

	QSettings vuSettings;

	if (event->button() == Qt::RightButton)
	{
		// do VU Mode
		if(VUMode == 3)
		{
			VUMode = 1;
		}
		else
		{
			VUMode++;
		}

		// save default VU Mode
		vuSettings.setValue("VU/defaultVU", VUMode);			// default VU Meter Modew

	}

	if (event->button() == Qt::LeftButton)
	{
		// threshold data
		QPoint pos = event->pos();
		mousePosX = pos.x();
		mousePosY = pos.y();


		// VUMode 1
		if(VUMode == 1)
		{
			thresHoldData[0][0] = mousePosX;
			thresHoldData[0][1] = mousePosY;

			vuSettings.setValue("VU/VU1/X", thresHoldData[0][0]);			// threshold data
			vuSettings.setValue("VU/VU1/Y", thresHoldData[0][1]);			// threshold data
			vuSettings.setValue("VU/VU1/Z", thresHoldData[0][2]);			// threshold data
		}

		if(VUMode == 2)
		{

			// VUMode 2
			thresHoldData[1][0] = mousePosX;
			thresHoldData[1][1] = mousePosY;

			vuSettings.setValue("VU/VU2/X", thresHoldData[1][0]);			// threshold data
			vuSettings.setValue("VU/VU2/Y", thresHoldData[1][1]);			// threshold data
			vuSettings.setValue("VU/VU2/Z", thresHoldData[1][2]);			// threshold data
		}

		if(VUMode == 3)
		{
			// VUMode 3 - center
			thresHoldData[2][0] = width() / 2;
			thresHoldData[2][1] = height() / 2;

			// use Pythagoras' theorem to determine radius of circle based on mousePosX and Y
			/*
					c (center)
					+
					|\
					|  \ 	a
				b	|    \
					|      \
					++        \
			      a	++--------+ b
						c
			*/

			// so... lets get the values for "b"
			double bVal = mousePosY - (height() / 2);

			// and c
			double cVal = mousePosX - (width() / 2);

			// and finally some trig... "a"
			int aVal;

			// a^2 = b^2 + c^2
			aVal = (int) sqrt(pow(bVal, 2) + pow(cVal, 2));

			thresHoldData[2][2] = aVal;

			vuSettings.setValue("VU/VU3/X", thresHoldData[2][0]);			// threshold data
			vuSettings.setValue("VU/VU3/Y", thresHoldData[2][1]);			// threshold data
			vuSettings.setValue("VU/VU3/Z", thresHoldData[2][2]);			// threshold data

		}
	}
}

// ***********************************************************************************************
// Mouse Moved
// ***********************************************************************************************

void VUMeter::mouseMoveEvent(QMouseEvent *event)
{

/*
	;QPoint pos = event->pos();
		if (pos.x() <= 0)
	pos.setX(1);
		if (pos.y() >= height())
	pos.setY(height() - 1);
	double rad = atan(((double)rect().bottom() - pos.y()) / pos.x());
	setAngle(qRound(rad * 180 / 3.14159265));
*/
}

// ***********************************************************************************************
// Mouse button released
// ***********************************************************************************************

void VUMeter::mouseReleaseEvent(QMouseEvent *event)
{
//if (event->button() == Qt::LeftButton)
//barrelPressed = false;
}

// ***********************************************************************************************
// Used to show the record light
// ***********************************************************************************************

void VUMeter::showRecord()
{
	isRecording = true;
}

// ***********************************************************************************************
// Used to hide the record light
// ***********************************************************************************************

void VUMeter::hideRecord()
{
	isRecording = false;
}

// ***********************************************************************************************
// used for our threshold timout - just increases the timer variable until it reaches a define
// ***********************************************************************************************

void VUMeter::updateThresholdTimer()
{
	if(underThresholdTimer < thresHoldTimeout)
	{
		underThresholdTimer++;
		emittedThreshold = true;

		return;
	}

	// under threshold for defined period of time
	if(underThresholdTimer == thresHoldTimeout)
	{
		// reset timer
		emittedThreshold = false;

		// stop timer
		thresHoldTimer->stop();

		// tell everyone
		emit(underThreshold());

		return;
	}

}

// ***********************************************************************************************
// Used to show playback details
// ***********************************************************************************************

void VUMeter::showPlayback()
{
	isPlaying = true;
}

