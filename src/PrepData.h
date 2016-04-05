/*
 * PrepData.h
 *
 *  Created on: Apr 4, 2016
 *      Author: zaqc
 */

#ifndef PREPDATA_H_
#define PREPDATA_H_

namespace vector {

class TrackChannel;

#define	MAX_TRACK_COUNT		10
#define	MAX_CH_PER_TRACK	4

class TapeTrack {
public:
	int m_RealHeight;			// real height (in pixel)
	int m_ScreenHeight;			// output height (in pixel)

	TrackChannel *m_Channel[MAX_CH_PER_TRACK];	// channel list (max count = 4)

	TapeTrack(int aRealHeight, int aScreenHeight);
	virtual ~TapeTrack();

	void AddChannel(int aChNumber, int aTickCount, int aDataOffset,
			int aColorIndex);
};

class TrackChannel {
public:
	int m_ChNumber;				// channel number
	int m_TickCount;			// real tick count per channel (default = 128)
	int m_DataOffset;			// data offset in data buffer
	int m_ColorIndex;			// index of color in color lookup table

	TrackChannel(int aChNumber, int aTickCount, int aDataOffset,
			int aColorIndex);
	virtual ~TrackChannel();
};

class ChColor {
public:
};

class PrepData {
private:
	TapeTrack *m_TapeTrack[MAX_TRACK_COUNT];
	unsigned int *m_Clut;
public:
	unsigned int *m_Buf;	// 800 * 480 * 4 output RGB data

	PrepData();
	virtual ~PrepData();

	void CreateCLT(void);
	void DefaultConfig(void);

	void SetChMaskAndLen(unsigned int aChMask, int *aChLen);
	void AddTrack(int aHeight);
	void PutData(unsigned char *aData, int aSize);
	void CopyTo(unsigned int *aBuf);
};

} /* namespace vector */

#endif /* PREPDATA_H_ */
