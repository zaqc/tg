/*
 * AccordDeviceBase.h
 *
 *  Created on: Apr 1, 2016
 *      Author: zaqc
 */

#ifndef ACCORDDEVICEBASE_H_
#define ACCORDDEVICEBASE_H_

namespace vector {

#define mk_cmd(addr, data) \
	((((unsigned int)addr << 16) & 0xFFFF0000) | (data & 0xFFFF))

#define mk_addr(loop_num, param_num, addr) \
	(0x7000 | ((loop_num << 9) & 0x0E00) | ((param_num << 5) & 0x1E0) | (addr & 0x1F))

#define PRM_SET_PARAM		0
#define	PRM_SET_DELAY		4
#define	PRM_VRC_WRITE		5
#define	PRM_ADC_ACCUM		6
#define PRM_DATA_LEN		7
#define	PRM_FILT_KOFF		8
#define	PRM_VRC_PTR_RST		9

#define H2F_LW_BASE_ADDR	0xFF200000

class AccordDeviceBase {
protected:
	void *m_H2F_LW;

	void *m_PioSync;
	void *m_PioReset;
	void *m_CtrlStart;
	void *m_CtrlCmd;
	void *m_RecvAddr;

	int m_MemFD;

	void *m_FR;
public:
	AccordDeviceBase();
	virtual ~AccordDeviceBase();

	void SyncWait(void);

	void SetDefaultParam(void);

	void alt_write_word(void *addr, unsigned int data);
	unsigned int alt_read_word(void *addr);

	void SetParam(int loop_num, int ch_num, int scan_type_a, int data_on,
			int imp_on, int pre_amp_on);

	void SetDelay(int loop_num, int delay);
	void SetVRC(int loop_num, int amp1, int amp2, int len);
	void SetAdcAccum(int loop_num, int adc_accum);
	void SetDataLen(int loop_num, int data_len);
	void SetFilter(int loop_num, bool filter_on = true);

	void GetData(unsigned char *aData, int aSize);
};

extern AccordDeviceBase *g_Device;

} /* namespace vector */

#endif /* ACCORDDEVICEBASE_H_ */
