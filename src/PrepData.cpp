/*
 * PrepData.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: zaqc
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>

#include <iostream>

#include "PrepData.h"
//----------------------------------------------------------------------------

namespace vector {

//============================================================================
//	TapeTrack
//============================================================================
TapeTrack::TapeTrack(int aRealHeight, int aScreenHeight) {
	m_RealHeight = aRealHeight;
	m_ScreenHeight = aScreenHeight;
	for (int i = 0; i < 4; i++)
		m_Channel[i] = NULL;
}
//----------------------------------------------------------------------------

TapeTrack::~TapeTrack() {
	for (int i = 0; i < 4; i++) {
		if (m_Channel[i])
			delete m_Channel[i];
	}
}
//----------------------------------------------------------------------------

void TapeTrack::AddChannel(int aChNumber, int aTickCount, int aDataOffset,
		int aColorIndex) {
	TrackChannel *ch = new TrackChannel(aChNumber, aTickCount, aDataOffset,
			aColorIndex);
	int i = 0;
	while (i < 4) {
		if (m_Channel[i] == NULL) {
			m_Channel[i] = ch;
			break;
		}
		i++;
	}
	if (i >= 4) {
		std::cout << "ERROR: Too many channel per track" << std::endl;
		delete ch;
	}
}
//----------------------------------------------------------------------------

//============================================================================
//	TrackChannel
//============================================================================
TrackChannel::TrackChannel(int aChNumber, int aTickCount, int aDataOffset,
		int aColorIndex) {
	m_ChNumber = aChNumber;
	m_TickCount = aTickCount;
	m_DataOffset = aDataOffset;
	m_ColorIndex = aColorIndex;
}
//----------------------------------------------------------------------------

TrackChannel::~TrackChannel() {
}
//----------------------------------------------------------------------------

//============================================================================
//	PrepData
//============================================================================
PrepData::PrepData() {
	for (int i = 0; i < MAX_TRACK_COUNT; i++)
		m_TapeTrack[i] = NULL;
	m_Buf = new unsigned int[800 * 480 * 4];
	m_Clut = NULL;
}
//----------------------------------------------------------------------------

PrepData::~PrepData() {
	delete[] m_Buf;
	for (int i = 0; i < MAX_TRACK_COUNT; i++) {
		if (m_TapeTrack[i])
			delete m_TapeTrack[i];
		m_TapeTrack[i] = NULL;
	}
}
//----------------------------------------------------------------------------

#define RGB(r, g, b) (((unsigned int)r & 0xFF) << 16) | (((unsigned int)g & 0xFF) << 8) | ((unsigned int)b & 0xFF)

#define	GET_R(c)	((c >> 16) & 0xFF)
#define	GET_G(c)	((c >> 8) & 0xFF)
#define	GET_B(c)	(c & 0xFF)

unsigned int GetMdlColor(unsigned int c1, unsigned int c2, int val, int max) {

	int r1 = GET_R(c1);
	int g1 = GET_G(c1);
	int b1 = GET_B(c1);

	int r2 = GET_R(c2);
	int g2 = GET_G(c2);
	int b2 = GET_B(c2);

	unsigned int r = r1 + (r2 - r1) * val / max;
	unsigned int g = g1 + (g2 - g1) * val / max;
	unsigned int b = b1 + (b2 - b1) * val / max;

	return RGB(r, g, b);
}

#define COLOR_TABLE(c1, c2, i1, i2) \
	m_Clut[c1 + c2 * 256 + ((i1 + i2 - ((i1 == 0) ? 1 : 0)) * 256 * 256)]

void fill_rect_clut(unsigned int *m_Clut, unsigned int cb, unsigned int c1,
		unsigned int c2, unsigned int cx) {

	for (int n = 0; n < 256; n++) {
		m_Clut[n] = GetMdlColor(cb, c1, n, 255);
		m_Clut[n * 256] = GetMdlColor(cb, c2, n, 255);
		m_Clut[n + n * 256] = GetMdlColor(cb, cx, n, 255);
	}

	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < j; i++) {
			m_Clut[i + j * 256] = GetMdlColor(m_Clut[j * 256],
					m_Clut[j + j * 256], i, j);
		}
	}

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < i; j++) {
			m_Clut[i + j * 256] = GetMdlColor(m_Clut[i], m_Clut[i + i * 256], j,
					i);
		}
	}
}

void PrepData::CreateCLT(void) {
	unsigned int cc[4];
	cc[0] = RGB(255, 255, 0);	// yellow
	cc[1] = RGB(0, 255, 255);	// cyan
	cc[2] = RGB(255, 0, 255);	// fucshia
	cc[3] = RGB(0, 255, 0);		// lime

	unsigned int cb = RGB(0, 0, 0);
	unsigned int cx = RGB(255, 0, 0);

	if (m_Clut) {
		delete[] m_Clut;
		m_Clut = NULL;
	}

	m_Clut = new unsigned int[256 * 256 * 6];
	memset(m_Clut, 0, 256 * 256 * 6 * sizeof(unsigned int));
	unsigned int *clut_ptr = m_Clut;
	for (int i = 0; i < 6; i++) {
		int n1[6] = { 0, 0, 0, 1, 1, 2 };
		int n2[6] = { 1, 2, 3, 2, 3, 3 };
		fill_rect_clut(clut_ptr, cb, cc[n1[i]], cc[n2[i]], cx);
		clut_ptr += 256 * 256;
	}
}
//----------------------------------------------------------------------------

void PrepData::DefaultConfig(void) {
	for (int i = 0; i < MAX_TRACK_COUNT; i++) {
		if (m_TapeTrack[i])
			delete m_TapeTrack[i];
		m_TapeTrack[i] = NULL;
	}

	CreateCLT();

	TapeTrack *tt;

	tt = new TapeTrack(128, 120);
	tt->AddChannel(0, 128, 0, 0);
	tt->AddChannel(1, 128, 128, 1);
	tt->AddChannel(2, 128, 256, 2);
	tt->AddChannel(3, 128, 384, 3);
	m_TapeTrack[0] = tt;
}
//----------------------------------------------------------------------------

#define ch_max(nn)						\
if (ch##nn)	{							\
	if (*ch##nn > c##nn)				\
		c##nn = *ch##nn;				\
	ch##nn++;							\
}
//----------------------------------------------------------------------------

#define max2(n1, n2, n3, n4)										\
		if (c##n2 > c##n3) {										\
			*out = COLOR_TABLE(c##n1, c##n2, n1 - 1, n2 - 1);		\
		} else {													\
			if (c##n1 > c##n4) {									\
				*out = COLOR_TABLE(c##n1, c##n3, n1 - 1, n3 - 1);	\
			} else {												\
				*out = COLOR_TABLE(c##n3, c##n4, n3 - 1, n3 - 1);	\
			}														\
		}
//----------------------------------------------------------------------------

int x_pos = 0;

void PrepData::PutData(unsigned char *aData, int aSize) {

//	for (int n = 0; n < 6; n++) {
//		for (int j = 0; j < 256; j++) {
//			for (int i = 0; i < 256; i++) {
//				m_Buf[i + j * 800 + (n % 3) * 256 + (n / 3) * 256 * 800] =
//						m_Clut[i + j * 256 + n * 256 * 256]; //RGB(i, j, n * 42);//
//			}
//		}
//	}

	TapeTrack **tt = m_TapeTrack;
	unsigned int *out = &m_Buf[x_pos % 800];
	x_pos++;
//	for(int i = 0; i < 480; i++){
//		*out = RGB(aData[i], 0, 0);
//		out += 800;
//	}
//	return;
//
	while (*tt) {
		TrackChannel **tc = (*tt)->m_Channel;
		unsigned char *ch1 = (*tc) ? &aData[(*tc)->m_DataOffset] : NULL;
		tc++;
		unsigned char *ch2 = (*tc) ? &aData[(*tc)->m_DataOffset] : NULL;
		tc++;
		unsigned char *ch3 = (*tc) ? &aData[(*tc)->m_DataOffset] : NULL;
		tc++;
		unsigned char *ch4 = (*tc) ? &aData[(*tc)->m_DataOffset] : NULL;
		tc++;

		int l = 0;
		int d = 0;
		unsigned char c1 = 0;
		unsigned char c2 = 0;
		unsigned char c3 = 0;
		unsigned char c4 = 0;

		// max of 4

		while (l < (*tt)->m_ScreenHeight) {
			if (d <= 0) {
				if (c1 > c2) {
					if (c3 > c4) {
						max2(1, 2, 3, 4);
					} else {
						max2(1, 2, 4, 3);
					}
				} else {
					if (c3 > c4) {
						max2(2, 1, 3, 4)
					} else {
						max2(2, 1, 4, 3)
					}
				}
				c1 = c2 = c3 = c4 = 0;
				out += 800;
				d += (*tt)->m_RealHeight;
				l++;
			} else {
				ch_max(1);
				ch_max(2);
				ch_max(3);
				ch_max(4);
				d -= (*tt)->m_ScreenHeight;
			}
		}
		tt++;
	}
}
//----------------------------------------------------------------------------

void PrepData::CopyTo(unsigned int *aBuf) {
	x_pos %= 800;
	unsigned int *src = m_Buf;
	unsigned int *dst = &aBuf[799 - x_pos];
	for (int i = 0; i < 480; i++) {
		memcpy(dst, src, x_pos * 4);
		dst += 800;
		src += 800;
	}
	x_pos = 0;
}
//----------------------------------------------------------------------------

} /* namespace vector */
