/*
 * HAL.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Robo
 */

#ifndef HAL_H_
#define HAL_H_

void initModesAndClock(void);
void initPIT(uint32_t LDVALOR_0, uint32_t LDVALOR_1);
void initGPIO(void);

#endif /* HAL_H_ */
