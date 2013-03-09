/************************************************************************/
/* hardware access functions or macros */
/************************************************************************/


#ifndef  _IMX35_H_
#define  _IMX35_H_


/* Memory word access, 16 bit */
#define CANinw(bd,adr)          \
                (__raw_readw (&((flexcan_t *)can_base[bd])->adr  ))


/* Memory long word access */
#if 0
#define CANoutl(bd,adr,v)       do { \
	printk(" write 0x%08x to %p\n", v, &((flexcan_t *)can_base[bd])->adr);\
                (__raw_writel(v, __iomem  &((flexcan_t *)can_base[bd])->adr ));\
                } while(0)
#endif
#define CANoutl(bd,adr,v)	\
                (__raw_writel(v, __iomem  &((flexcan_t *)can_base[bd])->adr ))

#define CANsetl(bd,adr,m)       \
        __raw_writel((__raw_readl((u32) &((flexcan_t *)can_base[bd])->adr)) \
                | (m) , (u32) &((flexcan_t *)can_base[bd])->adr )

#define CANresetl(bd,adr,m)     \
        __raw_writel((__raw_readl((u32) &((flexcan_t *)can_base[bd])->adr)) \
                & ~(m), (u32) &((flexcan_t *)can_base[bd])->adr )

#define CANinl(bd,adr)          \
                (__raw_readl ((u32) &((flexcan_t *)can_base[bd])->adr  ))

#define CANtestl(bd,adr,m)      \
        (__raw_readl((u32) &((flexcan_t *)can_base[bd])->adr  ) & (m) )

#endif          /* _IMX35_H_ */

