/*
 * FrameBuffer.cpp
 *
 *  Created on: Apr 2, 2016
 *      Author: zaqc
 */

#include "FrameBuffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

#include <sys/mman.h>
#include <linux/fb.h>

#include <sys/time.h>

#include <pthread.h>

#include <iostream>

#include "../altera_include/hps_0.h"
#include "AccordDeviceBase.h"
#include "PrepData.h"
//----------------------------------------------------------------------------

namespace vector {

FrameBuffer *g_FrameBuffer;

unsigned int *m_Buf1;
unsigned int *m_Buf2;

unsigned int *m_PrepBuf;

pthread_mutex_t m_Lock;
pthread_cond_t m_BufReady;
pthread_cond_t m_NeedData;

unsigned int *m_Target;
unsigned int *m_Render;
unsigned int *m_Cache;

bool m_BufPrepared = false;
bool m_InWaitData = false;

void * scroll_thread_proc(void *param) {

	return NULL;
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(1, &set);
	std::cout << "sched_setaffinity="
			<< sched_setaffinity(getpid(), sizeof(set), &set) << std::endl;

	while (true) {
		pthread_mutex_lock(&m_Lock);

		if (!m_BufPrepared) {
			memcpy(m_PrepBuf, m_PrepBuf + 1, 800 * 480 * 4 - 4);
			for (int i = 0; i < 480; i++) {
				m_PrepBuf[799 + i * 800] = (unsigned int) 255;
			}
			m_BufPrepared = true;
			if (m_InWaitData) {
				m_InWaitData = false;
				pthread_cond_signal(&m_BufReady);
			}
		} else {
			pthread_cond_wait(&m_NeedData, &m_Lock);
		}

		pthread_mutex_unlock(&m_Lock);
	}

	return NULL;
}

FrameBuffer::FrameBuffer() {
	// TODO Auto-generated constructor stub

	PrepData *pd = new PrepData();
	pd->DefaultConfig();

	int fd = open("/dev/fb0", O_RDWR);

	void *fb_ptr = mmap(NULL, 800 * 480 * 4, (PROT_READ | PROT_WRITE),
	MAP_SHARED, fd, 0);

	timeval ts, ts_prev;
	gettimeofday(&ts, 0);
	int fc = 0;
	ts_prev = ts;

	pthread_mutex_init(&m_Lock, NULL);
	pthread_cond_init(&m_BufReady, NULL);
	pthread_cond_init(&m_NeedData, NULL);

	m_Target = (unsigned int*) malloc(800 * 480 * 4);
	m_Render = (unsigned int*) malloc(800 * 480 * 4);
	m_Cache = (unsigned int*) malloc(800 * 480 * 4);
	for (int i = 0; i < 800 * 480 * 4; i++) {
		*((unsigned char *) m_Cache + i) = rand() % 256;
		*((unsigned char *) m_Target + i) = rand() % 256;
		*((unsigned char *) m_Render + i) = rand() % 256;
	}

	pthread_t pt;
	pthread_create(&pt, NULL, &scroll_thread_proc, NULL);
	usleep(100000);
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(1, &cpu);
	std::cout << "af thread="
			<< pthread_setaffinity_np(pt, sizeof(cpu_set_t), &cpu) << std::endl;

	CPU_ZERO(&cpu);
	CPU_SET(0, &cpu);
	std::cout << "af main="
			<< pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu)
			<< std::endl;

	int ss = 4;
	while (true) {
		unsigned int *tmp = m_Target;
		m_Target = m_Render;
		m_Render = tmp;
		memcpy(m_Target, m_Render + ss, 800 * 480 * 4 - ss * 4);

		unsigned char data[4096];
		for (int n = 0; n < ss; n++) {
			g_Device->GetData(data, 4096);
			pd->PutData(data, 4096);

//			for (int i = 0; i < 128; i++) {
//				m_Target[800 - ss + n + (i % 480) * 800] =
//						(((unsigned int) data[i]))
//								| (((unsigned int) data[i]) << 8)
//								| ((unsigned int) data[i]) << 16;
//			}
		}

		pd->CopyTo(m_Target);

		g_Device->SyncWait();
		memcpy(fb_ptr, m_Target, 800 * 480 * 4);
//		memcpy(fb_ptr, pd->m_Buf, 800 * 480 * 4);

		fc++;

		gettimeofday(&ts, 0);
		float delta = (ts.tv_sec * 1000000 + ts.tv_usec)
				- (ts_prev.tv_sec * 1000000 + ts_prev.tv_usec);

		if (delta >= 1000000.0f) {
			std::cout << (float) fc * 1000000.0 / delta << std::endl;

			fc = 0;
			ts_prev = ts;
		}
	}

	//----------------------------

	m_Buf1 = (unsigned int*) malloc(800 * 480 * 4);
	m_Buf2 = (unsigned int*) malloc(800 * 480 * 4);
	m_PrepBuf = m_Buf1;

	for (int i = 0; i < 800 * 480 * 4; i++) {
		*((unsigned char *) m_Buf1 + i) = rand() % 256;
		*((unsigned char *) m_Buf2 + i) = rand() % 256;
	}

	for (int i = 0; i < 1600; i++) {

		pthread_mutex_lock(&m_Lock);

		if (!m_BufPrepared) {
			m_InWaitData = true;
			//std::cout << "waiting data..." << std::endl;
			pthread_cond_wait(&m_BufReady, &m_Lock);
		}

		void *ptr = m_PrepBuf;
		m_PrepBuf = (m_PrepBuf == m_Buf1) ? m_Buf2 : m_Buf1;
		m_BufPrepared = false;
		pthread_cond_signal(&m_NeedData);
		pthread_mutex_unlock(&m_Lock);

		g_Device->SyncWait();
		memcpy(fb_ptr, ptr, 800 * 480 * 4);
//		m_BufPrepared = false;
//		m_PrepBuf = (m_PrepBuf == m_Buf1) ? m_Buf2 : m_Buf1;
//		pthread_cond_signal(&m_NeedData);

		fc++;

		gettimeofday(&ts, 0);
		float delta = (ts.tv_sec * 1000000 + ts.tv_usec)
				- (ts_prev.tv_sec * 1000000 + ts_prev.tv_usec);

		if (delta >= 1000000.0f) {
			std::cout << (float) fc * 1000000.0 / delta << std::endl;

			fc = 0;
			ts_prev = ts;
		}
	}

	munmap(fb_ptr, 800 * 480 * 4);
	close(fd);
}

FrameBuffer::~FrameBuffer() {
	// TODO Auto-generated destructor stub
}

} /* namespace vector */
