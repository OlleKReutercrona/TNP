#pragma once

#define NETMESSAGE_SIZE 512
#define USERNAME_MAX_LENGTH 32
#define MESSAGE_MAX_SIZE 256

#define MAX_PLAYERS 8


static inline unsigned int PackColors(const unsigned char c1, const unsigned char c2, const unsigned char c3, const unsigned char c4)
{
	return (
		(int)((unsigned char)c1 << 24) |
		(int)((unsigned char)c2 << 16) |
		(int)((unsigned char)c3 << 8) |
		(int)((unsigned char)c4)
		);
}

static inline void UnpackColors(const unsigned int& packedColor, float& red, float& green, float& blue, float& alpha)
{
	unsigned char cred = packedColor >> 24 & 0xFF;
	unsigned char cgreen = packedColor >> 16 & 0xFF;
	unsigned char cblue = packedColor >> 8 & 0xFF;
	unsigned char calpha = packedColor & 0xFF;
	
	red		= (float)(cred / 255.0f);
	green	= (float)(cgreen / 255.0f);
	blue	= (float)(cblue / 255.0f);
	alpha	= (float)(calpha / 255.0f);
}