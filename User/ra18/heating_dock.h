/* 
 * File:   heating_dock.h
 * Author: abel
 *
 * Created on February 19, 2025, 11:05 AM
 */

#ifndef HEATING_DOCK_H
#define	HEATING_DOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_COMS		3

#define NUM_DOCKS		18
#define NUM_HEAT_TC		3

#define NUM_COM_DOCKS    (NUM_DOCKS / NUM_COMS)


#define TO_DOCK_INDEX(com_id, dock_id)      ((dock_id) + (com_id) * NUM_COM_DOCKS)
    
    
typedef struct{
	volatile char idx;
	volatile char tc_id;
	volatile char dock_id;

	volatile uint32_t duration;	// in millisecond
	volatile float target_temperature;		// in deg.c
	volatile float current_temperature;     // in deg.c

}heat_channel_st;

typedef struct{
    volatile char idx;
	volatile char dock_id;
	volatile char com_id;
	volatile char in_place;
//	volatile char do_flag;
	heat_channel_st *tcs[NUM_HEAT_TC];
}heat_dock_st;


typedef struct{
	volatile char com_id;
	volatile char update;
	volatile char txflag;
	volatile char rxflag;
	heat_dock_st *docks[NUM_COM_DOCKS];
}heat_com_st;


extern heat_channel_st heat_tcs[];
extern heat_dock_st heat_docks[];
extern heat_com_st heat_coms[];


typedef struct{
    uint8_t type;   // 0 request , 1 response
    uint8_t cmd;
    uint8_t status;
    uint8_t idx_qty;
    uint8_t dock_idxs[NUM_DOCKS];
    uint8_t *pdata;
    uint8_t dlen;
}ps96_message_st;



void InitHeatingDock(void);

extern int ps96_process_heatdock_msg(uint8_t cmd, uint8_t *data, int dlen);
extern int heatdock_response_handler(uint8_t com_id, uint8_t cmd, uint8_t *data, int dlen);


#ifdef	__cplusplus
}
#endif

#endif	/* HEATING_DOCK_H */

