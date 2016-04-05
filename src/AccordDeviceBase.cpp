/*
 * AccordDeviceBase.cpp
 *
 *  Created on: Apr 1, 2016
 *      Author: zaqc
 */

#include "AccordDeviceBase.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

#include <sys/mman.h>

#include <iostream>

#include "../altera_include/hps_0.h"
//----------------------------------------------------------------------------

int g_WaitSyncCount = 0;

namespace vector {

AccordDeviceBase *g_Device;

AccordDeviceBase::AccordDeviceBase() {
	m_MemFD = open("/dev/mem", (O_RDWR | O_SYNC));
	if (-1 == m_MemFD) {
		std::cout << "ERROR: Can't open /dev/mem" << std::endl;
		goto fd_open_error;
	}

	m_H2F_LW = mmap(NULL, 0x40000, (PROT_READ | PROT_WRITE), MAP_SHARED,
			m_MemFD,
			H2F_LW_BASE_ADDR);
	if (MAP_FAILED == m_H2F_LW) {
		std::cout << "ERROR: Can't map memory region (H2F_LW:0x40000)"
				<< std::endl;
		goto mmap_error;
	}

	m_PioReset = (void*) ((unsigned int) m_H2F_LW + PIO_RESET_BASE);
	m_PioSync = (void*) ((unsigned int) m_H2F_LW + PIO_SYNC_BASE);
	m_CtrlStart = (void*) ((unsigned int) m_H2F_LW + CHANNAL_CNTRL_BASE);
	m_CtrlCmd = (void*) ((unsigned int) m_H2F_LW + CHANNAL_CNTRL_BASE + 4);

	m_RecvAddr = (void*) ((unsigned int) m_H2F_LW + DATA_RECEIVER_BASE);

	m_FR = (void*) ((unsigned int) m_H2F_LW + ALT_VIP_VFR_0_BASE + 0);

	alt_write_word((unsigned int *) m_FR, 3);
	std::cout << alt_read_word((unsigned int *) m_FR) << std::endl;
	/*
	 std::cout << alt_read_word((unsigned int *)m_FR + 3) << std::endl;
	 std::cout << alt_read_word((unsigned int *)m_FR + 4) << std::endl; //, 0x3ec00000);
	 std::cout << alt_read_word((unsigned int *)m_FR + 5) << std::endl; //, 0x177000);
	 std::cout << alt_read_word((unsigned int *)m_FR + 6) << std::endl; //  one cycle color pattern
	 std::cout << alt_read_word((unsigned int *)m_FR + 8) << std::endl; //, 800);
	 std::cout << alt_read_word((unsigned int *)m_FR + 9) << std::endl; //, 480);
	 std::cout << alt_read_word((unsigned int *)m_FR) << std::endl; //, 11);

	 */
	return;

	mmap_error: close(m_MemFD);

	fd_open_error: throw "ERROR: Can't initialize device...";
}
//----------------------------------------------------------------------------

AccordDeviceBase::~AccordDeviceBase() {
	if (NULL != m_H2F_LW) {
		if (munmap(m_H2F_LW, 0x40000) != 0)
			std::cout << "ERROR: Can't unmap memory region (H2F_LW:0x40000)"
					<< std::endl;
	}

	if (-1 != m_MemFD)
		close(m_MemFD);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SyncWait(void) {
	alt_write_word((unsigned int *) m_FR + 2, 2);
	while ((alt_read_word((unsigned int *) m_FR + 2) & 2) == 0) {
		g_WaitSyncCount++;
		usleep(1000);
	}
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetDefaultParam(void) {
	alt_write_word(m_PioReset, 0);
	alt_write_word(m_PioReset, 1);	// reset DScope
	usleep(20000);
	alt_write_word(m_PioReset, 0);
	usleep(20000);

	// set all channels to ScanType = "B" DataAccept="on" SendImpulse="on" PreAmping="off"
	for (int i = 0; i < 8; i++)
		SetParam(i, i, 0, 1, 1, 0);

	// set default VRC parameters
	for (int i = 0; i < 8; i++) {
		SetVRC(i, 140, 240, 0);
	}

	// set FIR filter 2.5 MHz for all channel
	for (int i = 0; i < 8; i++) {
		SetFilter(i, true);
	}

	// set data length 32 * 4 == 128 byte
	for (int i = 0; i < 8; i++) {
		SetDataLen(i, 32);
	}

	for (int i = 0; i < 8; i++)
		SetDelay(i, 0);

	for (int i = 0; i < 8; i++)
		SetAdcAccum(i, 16);

	// switch to normal mode (from "POWER-ON 0x0007" test mode)
	alt_write_word(m_CtrlCmd, 0x02);
	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlStart) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::alt_write_word(void *addr, unsigned int data) {
	*((volatile unsigned int *) addr) = data;
}
//----------------------------------------------------------------------------

unsigned int AccordDeviceBase::alt_read_word(void *addr) {
	return *((volatile unsigned int*) addr);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetParam(int loop_num, int ch_num, int scan_type_a,
		int data_on, int imp_on, int pre_amp_on) {
	unsigned int param = ch_num & 0x7;
	param |= (scan_type_a << 3) & 0x08;
	param |= (data_on << 4) & 0x10;
	param |= (imp_on << 5) & 0x20;
	param |= (pre_amp_on << 6) & 0x40;

	alt_write_word(m_CtrlCmd,
			mk_cmd(mk_addr(loop_num, PRM_SET_PARAM, 0), param));
	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlCmd) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetDelay(int loop_num, int delay) {
	alt_write_word(m_CtrlCmd,
			mk_cmd(mk_addr(loop_num, PRM_SET_DELAY, 0), delay));
	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlStart) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetVRC(int loop_num, int amp1, int amp2, int len) {
	alt_write_word(m_CtrlCmd, mk_cmd(mk_addr(loop_num, PRM_VRC_PTR_RST, 0), 0));
	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlStart) != 1)
		usleep(100);

	if (len > 0) {
		for (int i = 0; i <= len; i++) {
			int a = (amp1 * 2) * i / len;

			a = a < 256 ? a << 8 : 0xFF00 | (a - 255);
			alt_write_word(m_CtrlCmd,
					mk_cmd(mk_addr(loop_num, PRM_VRC_WRITE, 0), a));
		}
	}

	for (int i = 1; i <= 60 - len; i++) {
		int a = amp1 * 2
				+ (float) (amp2 - amp1) * 2 * (float) i / (float) (60 - len);

		a = a < 256 ? a << 8 : 0xFF00 | (a - 255);
		alt_write_word(m_CtrlCmd,
				mk_cmd(mk_addr(loop_num, PRM_VRC_WRITE, 0), a));
	}

	for (int i = 60; i <= 256; i++) {
		alt_write_word(m_CtrlCmd,
				mk_cmd(mk_addr(loop_num, PRM_VRC_WRITE, 0), 0x8080));
	}

	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlStart) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetAdcAccum(int loop_num, int adc_accum) {
	alt_write_word(m_CtrlCmd,
			mk_cmd(mk_addr(loop_num, PRM_ADC_ACCUM, 0), adc_accum));
	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlStart) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetDataLen(int loop_num, int data_len) {
	alt_write_word(m_CtrlCmd,
			mk_cmd(mk_addr(loop_num, PRM_DATA_LEN, 0), data_len));
	alt_write_word(m_CtrlStart, 0x0);
	while (alt_read_word(m_CtrlStart) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::SetFilter(int loop_num, bool filter_on) {
	const short FIRCoef[23] = { -3067, -1093, -565, -2779, -7429, -12290,
			-14061, -10091, -127, 12960, 24117, 28506, 24117, 12960, -127,
			-10091, -14061, -12290, -7429, -2779, -565, -1093, -3067 };

	for (int i = 0; i < 23; i++) {
		alt_write_word(m_CtrlCmd,
				mk_cmd(mk_addr(loop_num, PRM_FILT_KOFF, i), FIRCoef[i]));
		alt_write_word(m_CtrlStart, 0);
		while (alt_read_word(m_CtrlCmd) != 1)
			usleep(100);
	}
	alt_write_word(m_CtrlCmd,
			mk_cmd(mk_addr(loop_num, PRM_FILT_KOFF, 23), filter_on ? 15: 0));
	alt_write_word(m_CtrlStart, 0);
	while (alt_read_word(m_CtrlCmd) != 1)
		usleep(100);
}
//----------------------------------------------------------------------------

void AccordDeviceBase::GetData(unsigned char *aData, int aSize) {

	for (int i = 0; i < 8; i++) {
		alt_write_word((void *) ((long int) m_RecvAddr + 0x3000 * 4),
				0x00000000);	// clear data counter
		alt_write_word((void *) ((long int) m_RecvAddr + 0x3000 * 4),
				0xFFFFFFFF);	// enable data counter
		alt_write_word((void *) ((long int) m_RecvAddr + 0x0000 * 4), 32);// set data fill level

		alt_write_word(m_PioSync, 1);	// pulse sync
		alt_write_word(m_PioSync, 0);

		while ((alt_read_word((void *) ((long int) m_RecvAddr + 0x3400 * 4)) & 1)
				== 0) {
			usleep(10);
		}

		memcpy(&aData[i * 128], (void *) *&m_RecvAddr, 128);
	}
}
//----------------------------------------------------------------------------

} /* namespace vector */
