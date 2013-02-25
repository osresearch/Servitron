#include "quadrature.h"
#include "config.h"
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

Quadrature::Quadrature() : pru_data_map(-1) {
}

void Quadrature::init(uint8_t ch, uint8_t a, uint8_t b, uint8_t idx) {
  QuadState* qs = (QuadState*)(pru_data_map + QUAD_STATE_BASE);
  qs += ch;
  qs->position = 0;
  qs->state = 0;
  qs->pinA = a;
  qs->pinB = b;
  qs->pinIdx = idx;
  qs->errors = 0;
}

void Quadrature::start() {
    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    Channel* channels;

    /* Initialize the PRU */
    prussdrv_init ();		
    
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_0);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        return (ret);
    }
    
    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);

    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, (void*)&pru_data_map);
    init(0, Q0A, Q0B, Q0I);
    init(1, Q1A, Q1B, Q1I);
    init(2, Q2A, Q2B, Q2I);
    init(3, Q3A, Q3B, Q3I);
    init(4, Q4A, Q4B, Q4I);
    init(5, Q5A, Q5B, Q5I);

    /* Execute quadrature peripheral on PRU */
    prussdrv_exec_program (PRU_NUM, "./quad.bin");
}

Report Quadrature::getNextReport() {

    /* Wait until PRU0 has finished execution */
    printf("\tWaiting for report...\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    Report r = *((Report*)(pru_data_map + REPORT_BASE));
    prussdrv_pru_clear_event (PRU0_ARM_INTERRUPT);

    for (int i = 0; i < 6; i++) {
      printf("\tENCODER %d VALUE %d ERRORS %d\r\n",channels[i].position,channels[i].errCount);
    }
}

void Quadrature::stop() {
    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable (PRU_NUM);
    prussdrv_exit ();
}