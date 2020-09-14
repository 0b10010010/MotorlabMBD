/*
 * vcp.c
 */

#include "vcp.h"
#include "stm32f4xx_rcc.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "misc.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

// print values into this buffer with sprintf
char vcp_output_buffer[VCP_OUTPUT_BUFFER_SIZE];

// Initialize USB virtual comm port
void vcp_init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Ensure all priority bits are assigned as preemption priority bits. */
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS
    USB_OTG_HS_CORE_ID,
#else
    USB_OTG_FS_CORE_ID,
#endif
    &USR_desc,
    &USBD_CDC_cb,
    &USR_cb);
}
