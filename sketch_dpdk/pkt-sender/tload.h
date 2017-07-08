#ifndef TLOAD_H
#define TLOAD_H

#include <stdio.h>
#include "pm.h"

#define INVALID_LINE -1
#define END_LINE 0
#define VALID_LINE 1

#define NB_FIELD 7

int load_trace_line(FILE *fp, struct packet_model *pm);
int load_trace(const char *file, struct packet_model pms[]);


int load_vxlan_trace_line(FILE *fp, struct packet_model *pm);
int load_vxlan_trace(const char *file, struct packet_model pms[]);


#endif
