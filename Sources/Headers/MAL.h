/*
 * MAL.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Robo
 */

#ifndef MAL_H_
#define MAL_H_

#define PB_UP		SIU.GPDI[64].B.PDI
#define PB_DOWN 	SIU.GPDI[66].B.PDI
#define PB_AnPi 	SIU.GPDI[67].B.PDI

void isr(void);

#endif /* MAL_H_ */
