


#include "iamp_msg.h"
#include "can_msg.h"
#include "busmsg.h"
#include "lib/l_uart.h"
#include "heating_dock.h"

heat_channel_st heat_tcs[NUM_DOCKS * NUM_HEAT_TC];
heat_dock_st heat_docks[NUM_DOCKS];
heat_com_st heat_coms[NUM_COMS];



//static uint8_t msg_txbuff[256];


#define     PREAMBLE_CODE		         FRAME_HEAD   


struct {
    volatile uint8_t idtable[NUM_COM_DOCKS];
    volatile uint8_t count;
} dockid_table[NUM_COMS];

#define reset_dockid_table()    memset(dockid_table, 0, sizeof(dockid_table))
#define addto_dockid_table(p_dock)      dockid_table[(p_dock)->com_id].idtable[dockid_table[(p_dock)->com_id].count++] = (p_dock)->dock_id


void InitHeatingDock(void)
{
    heat_dock_st *dock;
    heat_channel_st *tc;
    heat_com_st *com;
    int i, j, k;
    int idx;
    
    
    
    for(i=0; i<NUM_COMS; i++){
        
        com =  &heat_coms[i];
        com->com_id = i;
        com->rxflag = 0;
        com->txflag = 0;
        com->update = 0;
        
        for(j=0; j<NUM_COM_DOCKS; j++){
            
            idx = TO_DOCK_INDEX(i, j);
            dock = &heat_docks[idx];
            dock->idx = idx;
            dock->dock_id = j;
            dock->com_id = i;
            dock->in_place = 0;
            
            com->docks[j] = dock;
            
            for(k=0; k<NUM_HEAT_TC; k++){
                idx = dock->idx * NUM_HEAT_TC + k;
                tc = &heat_tcs[idx];
    //            tc = dock->tcs[j];
                dock->tcs[k] = tc;

                tc->idx = idx;
                tc->tc_id = k;
                tc->dock_id = j;
                tc->duration = 0;
                tc->target_temperature = 0;
                tc->current_temperature = 0;
            }
        }
    }
    
}



static inline int check_rcc(uint8_t *pdata, int len)
{
	int i;

	uint8_t sum = 0;
    
	for(i=0; i<len; i++){
		sum += pdata[i];
	}

	return (sum ==  pdata[len]);
}

int send_heatdock_request(uint8_t com_id, uint8_t cmd, uint8_t *pdata, uint8_t dlen)
{

	return can_sendmsg(cmd | (com_id << 8), pdata, dlen);

}

int ps96_process_heatdock_msg(uint8_t cmd, uint8_t *data, int dlen)
{

	unsigned char *comTxBuff, *pbuff;
	int txlen = 0;
	int i, j, k;
    int offs = 0;

	uint16_t v_u16;

	uint8_t dock_id, index;
	uint8_t status = 0;
	heat_dock_st *p_dock;
    
    unsigned char txBuff[8] = {0, 1, 2, 3, 4, 5};

    printfln("ps96_process %#.2x heatdock_msg %d bytes", cmd, dlen);

    reset_dockid_table();
    
    switch(cmd){
        case PCR_INPLACE:
        	if(dlen == 0){
        		for(i=0; i<NUM_COMS; i++){
					send_heatdock_request(i, cmd, txBuff, 6);
				}
        		break;
        	}
        case PCR_STARTHEAT:
        case PCR_STOPHEAT:
        case PCR_CURTEMP:
        case PCR_GETTEMP:
        case PCR_GETTIME:

            for(offs=0; offs<dlen; offs++){
                index = data[offs];
                printfln("%#.2x", index);
                if(index < NUM_DOCKS){
                    p_dock = &heat_docks[index];
                    addto_dockid_table(p_dock);
                }else{
                    status = 0x01;
                }
            }
            for(i=0; i<NUM_COMS; i++){
                printfln("%d com , count = %d", i, dockid_table[i].count);
                if(dockid_table[i].count > 0){
                    send_heatdock_request(i, cmd, dockid_table[i].idtable, dockid_table[i].count);
                }
            }
            break;
        case PCR_SETTEMP:
        case PCR_SETTIME:
            while(offs < dlen){
                index = data[offs++];
                printfln("%#.2x", index);
                if(index < NUM_DOCKS){
                    p_dock = &heat_docks[index];
                    addto_dockid_table(p_dock);
                }else{
                    p_dock = NULL;
                    status = 0x01;
                }
                for(i=0; i<NUM_HEAT_TC; i++){
                    v_u16 = data[offs++] << 8;
                    v_u16 |= data[offs++] << 0;
                    if(p_dock){
                        if(cmd == PCR_SETTEMP){
                            p_dock->tcs[i]->target_temperature = v_u16;
                        }else if(cmd == PCR_SETTIME){
                            p_dock->tcs[i]->duration = v_u16 * 1000L;
                        }
                    }
                }
            }
            
            for(i=0; i<NUM_COMS; i++){
                
                printfln("%d com , count = %d", i, dockid_table[i].count);

                for(j=0; j<dockid_table[i].count; j++){
					pbuff = txBuff;
					txlen = 0;
                    dock_id = dockid_table[i].idtable[j];
                    printfln("%d com , idtable[%d] = %d", i, j, dock_id);
                    p_dock = heat_coms[i].docks[dock_id];
                    pbuff[txlen++] = dock_id;
                    for(k=0; k<NUM_HEAT_TC; k++){
                        if(cmd == PCR_SETTEMP){
                            v_u16 = p_dock->tcs[k]->target_temperature;
                        }else if(cmd == PCR_SETTIME){
                            v_u16 = p_dock->tcs[k]->duration / 1000L;
                        }
                        pbuff[txlen++] = (v_u16 >> 8) & 0xff;
                        pbuff[txlen++] = (v_u16 >> 0) & 0xff;
                    }
					send_heatdock_request(i, cmd, txBuff, txlen);
                }
                
            }

            break;

    }


	return 0;
}

static char check_heatcom_update(void)
{
    char ok = 1;
    int i;
    
    for(i=0; i<NUM_COMS; i++){
        if(heat_coms[i].update == 0){
            ok = 0;
            break;
        }
        heat_coms[i].update = 0;
    }
    for(; i<NUM_COMS; i++){
        heat_coms[i].update = 0;
    }
    
    return ok;
}

static char check_heatcom_rxflag(uint8_t cmd)
{
    char ok = 1;
    int i;
    
    for(i=0; i<NUM_COMS; i++){
        if(heat_coms[i].rxflag & (1 << (cmd & 0x7)) == 0){
            ok = 0;
            break;
        }
        heat_coms[i].rxflag &= ~(1 << (cmd & 0x7));
    }
    for(; i<NUM_COMS; i++){
        heat_coms[i].rxflag &= ~(1 << (cmd & 0x7));
    }
    
    return ok;
}

#define     FRAME_HEAD		         0x5e                         //帧头
#define     FRAME_HEAD_INDEX	     0
#define     FRAME_CMD_INDEX	         1
#define     FRAME_NO_INDEX           2
#define     FRAME_LTHH_INDEX	     3
#define     FRAME_LTHL_INDEX	     4

#define     FRAME_HEAD_LEN          4

#define     FRAME_STATE_INDEX	     1   //倒数2
#define     FRAME_CRC_INDEX	         0   //倒数1
#define     PERDATALTH	             2   //单个数据长度

int send_ps96msg(unsigned char cmd, unsigned char *pbuff, unsigned char *pdata, unsigned int dlen)
{
    unsigned int frm_len = dlen + 5;

    pbuff[0]  = FRAME_HEAD;
    pbuff[1]  = cmd | 0X80;
    pbuff[2]  = 0;
    pbuff[3]  = frm_len & 0xff;


    if(pdata && (dlen > 0)){
        memcpy(&pbuff[4], pdata, dlen);
    }

    dlen = frm_len - 1;
    pbuff[dlen++] = (unsigned char) calc_rcc(pbuff, dlen);

    write_bus_msg(pbuff, frm_len);

    return 0;
}


int heatdock_response_handler(uint8_t com_id, uint8_t cmd, uint8_t *data, int dlen)
{
    uint8_t txbuff[80];
	uint8_t *pbuff = txbuff;
    uint8_t txlen = 0;
	int i, offs = 0;

	uint16_t v_u16;

	uint8_t dock_id;
	uint8_t status = 0;
	heat_dock_st *p_dock;
	
    pbuff += 4;

    printfln("");
        
	if(cmd & 0x80){
        
        cmd &= 0x7f;

        printfln("heatdock_response : %d com , cmd %#.2x, %d bytes", com_id, cmd, dlen);

            
        switch(cmd){

        case PCR_SETTEMP :
        case PCR_SETTIME :
        	dock_id = data[0];
        	status = data[1];
        	p_dock = heat_coms[com_id].docks[dock_id];
        	pbuff[txlen++] = p_dock->idx;
        	pbuff[txlen++] = status;
        	send_ps96msg(cmd, txbuff, NULL, txlen);
        	break;

		case PCR_STARTHEAT :
		case PCR_STOPHEAT :
		case PCR_INPLACE :
			dlen--;
			for(offs=0; offs<dlen; offs++){
				dock_id = data[offs] >> 1;
				p_dock = heat_coms[com_id].docks[dock_id];
				status = data[offs] & 0x1 ? 1 : 0;
				pbuff[txlen++] = status | (p_dock->idx << 1);
				if(cmd == PCR_INPLACE){
					p_dock->in_place = status;
				}
			}
			pbuff[txlen++] = 0;
			send_ps96msg(cmd, txbuff, NULL, txlen);

			break;

		case PCR_CURTEMP :
		case PCR_GETTEMP :
		case PCR_GETTIME :
			dlen--;
			dock_id = data[offs++];
			status = data[dlen];
			if(dock_id < NUM_COM_DOCKS){
				p_dock = heat_coms[com_id].docks[dock_id];
				pbuff[txlen++] = p_dock->idx;
			}else{
//				pbuff[txlen++] = dock_id;
				p_dock = NULL;
			}
			i = 0;
			while(offs<dlen){
				v_u16 = data[offs++] << 8;
				v_u16 |= data[offs++] << 0;
				pbuff[txlen++] = (v_u16 >> 8) & 0xff;
				pbuff[txlen++] = (v_u16 >> 0) & 0xff;
				if(p_dock == NULL){
					continue;
				}
				if(cmd == PCR_CURTEMP){
					p_dock->tcs[i]->current_temperature = v_u16 / 10;
				}
				else if(cmd == PCR_GETTEMP){
					p_dock->tcs[i]->target_temperature = v_u16;
				}
				else if(cmd == PCR_GETTIME){
					p_dock->tcs[i]->duration = v_u16 * 1000;
				}
				i++;
			}
			for(i=0; i<NUM_HEAT_TC; i++){
			}
			pbuff[txlen++] = status;
			send_ps96msg(cmd, txbuff, NULL, txlen);


			break;
        
        }
        
	}

	return 0;
}



