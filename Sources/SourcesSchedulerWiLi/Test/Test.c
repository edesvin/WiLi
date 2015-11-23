#define TEST 0

#if TEST == 1
#define ValPeriod 10

extern volatile T_UWORD ruw_CountTest = ValPeriod - 1;
/*==============================================================================
* Function: task1ms
* 
* Description: Tarea que se ejecuta cada ms, su funcion es crear pulsos 
* para emular los push button con el objetivo de hacer pruebas al sistema.
*
==============================================================================*/
void task1ms(void){
	if(ruw_CountTest > 0){
		ruw_CountTest--;
	}
	else{
		ruw_CountTest = ValPeriod - 1;
		SIU.GPDO[68].B.PDO = !SIU.GPDO[68].B.PDO;	
	}

}
#endif
