/* sha1.h

Copyright (c) 2005 Michael D. Leonhard

http://tamale.net/

This file is licensed under the terms described in the
accompanying LICENSE file.
*/

#ifndef SHA1_HEADER
#define SHA1_HEADER

typedef unsigned int Uint32;

#include <QWidget>

class SHA1 : public QWidget
{
	Q_OBJECT

	public:
		SHA1(QWidget *parent = 0);
		~SHA1();

		void addBytes( const char* data, int num );
		QString getDigest();

		// utility methods
		static Uint32 SHA1::lrot( Uint32 x, int bits );
		static void SHA1::storeBigEndianUint32( unsigned char* byte, Uint32 num );

	private:
		// fields
		Uint32 H0, H1, H2, H3, H4;
		unsigned char bytes[64];
		int unprocessedBytes;
		Uint32 size;
		void process();
};

#endif
