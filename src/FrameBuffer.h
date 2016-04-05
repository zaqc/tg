/*
 * FrameBuffer.h
 *
 *  Created on: Apr 2, 2016
 *      Author: zaqc
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

namespace vector {

class FrameBuffer {
protected:
public:
	FrameBuffer();
	virtual ~FrameBuffer();
};

extern FrameBuffer *g_FrameBuffer;

} /* namespace vector */

#endif /* FRAMEBUFFER_H_ */
