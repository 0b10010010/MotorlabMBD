/*
 * vcp.h
 */

#ifndef _VCP_H_
#define _VCP_H_

#include "usb_dcd.h"
#include "usbd_def.h"
#include "usbd_conf.h"

// The size of the output buffer, this is the maximum amount of chars you
// can us in comm_printf()
#define VCP_OUTPUT_BUFFER_SIZE 32

// print values into this buffer with sprintf
extern char vcp_output_buffer[VCP_OUTPUT_BUFFER_SIZE];

// Initialize USB virtual comm port
void vcp_init();

#endif /* _VCP_H_ */
