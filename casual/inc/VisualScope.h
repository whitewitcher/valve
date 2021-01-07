#ifndef _VISUALSCOPE_H
#define _VISUALSCOPE_H

#include "stm32f10x.h"

static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void VS4Channal_Send(int n_dataCH1, int n_dataCH2, int n_dataCH3, int n_dataCH4);


#endif
