#ifndef _INCLUDE_YCMD5_H_
#define _INCLUDE_YCMD5_H_

#include <string>

#define uint8 unsigned char
#define uint32 unsigned long int

/*
 * FGPMd5 : 
 *
 *
 */
class YCAPI YCMd5
{
public:
	//! construct a FGPMd5 from any buffer
	void GenerateMD5(unsigned char* buffer,int bufferlen);

	//! construct a FGPMd5
	YCMd5();

	//! construct a md5src from char *
	YCMd5(const char * md5src);

	//! construct a FGPMd5 from a 16 bytes md5
	YCMd5(unsigned long* md5src);

	//! add a other md5
	YCMd5 operator +(YCMd5 adder);

	//! just if equal
	bool operator ==(YCMd5 cmper);

	//! give the value from equer
	// void operator =(FGPMd5 equer);

	//! to a string
	std::string ToString();

	unsigned long m_data[4];

private:

	struct md5_context
	{
		uint32 total[2];
		uint32 state[4];
		uint8 buffer[64];
	};

	void md5_starts( struct md5_context *ctx );
	void md5_process( struct md5_context *ctx, uint8 data[64] );
	void md5_update( struct md5_context *ctx, uint8 *input, uint32 length );
	void md5_finish( struct md5_context *ctx, uint8 digest[16] );
};

#endif

