//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : TaskStateSegment.h
// Date de création : Octobre 2015
// Description : Cette structure représente la TSS 
// représentant le contexte de chaque processus
//----------------------------------------------------

#ifndef TASK_STATE_SEGMENT_H
#define TASK_STATE_SEGMENT_H

#include "Types.h"
#include "Defines.h"

struct TaskStateSegment
{
    uint16 previous_task, __previous_task_unused;
    uint32 esp0;
    uint16 ss0, __ss0_unused;
    uint32 esp1;
    uint16 ss1, __ss1_unused;
    uint32 esp2;
    uint16 ss2, __ss2_unused;
    uint32 cr3;
    uint32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint16 es, __es_unused;
    uint16 cs, __cs_unused;
    uint16 ss, __ss_unused;
    uint16 ds, __ds_unused;
    uint16 fs, __fs_unused;
    uint16 gs, __gs_unused;
    uint16 ldt_selector, __ldt_sel_unused;
    uint16 debug_flag, io_map;
	
} GEM_PACK;


#endif