#ifndef PE
#define PE

#include "core.h"

void pe(block_stream* B_cids, block_stream* B_vals, block_stream* B_cidsd, block_stream* B_valsd, pe_stream* task, partial_stream* partial,
        id_stream* B_rlen, mcontrol_stream* mcontrol, bool_stream* pe_idle, bool_stream* efinish);

void merger(mcontrol_stream* control, merge_stream* from, merge_stream* result, bool_stream* efinish);

#endif
