struct dos_partition {
	u8 bootable;			/* 0 = no, 0x80 = bootable */
	u8 s_head;				/* Starting head */
	u16 s_sector:6;			/* Starting sector */
	u16 s_cyl:10;			/* Starting cylinder */
	u8 id;					/* System ID */
	u8 e_head;				/* Ending Head */
	u16 e_sector:6;			/* Ending Sector */
	u16 e_cyl:10;			/* Ending Cylinder */
	u32 s_lba;				/* Starting LBA value */
	u32 size;				/* Total Sectors in partition */
} __attribute__ ((packed));

