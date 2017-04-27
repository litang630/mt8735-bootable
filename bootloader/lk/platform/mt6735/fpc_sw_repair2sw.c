#include <platform/mt_reg_base.h>
#include <platform/mt_typedefs.h>
#include <platform/mtk_wdt.h>
#include <platform/mt_gpt.h>
#include <platform/ddp_hal.h>
#include <printf.h>

#define MBIST_CHK_SRAM_REPAIRED       1
#define MBIST_CHK_SRAM_NOT_REPAIR     0

#define CHECK_OR_ALL_ONE       1
#define CHECK_AND_ALL_ZERO     0

struct sram_repair_chk_items
{
    UINT32      addr;
    UINT32      mask;
    UINT32      rule;
    const char *name;
};

struct sram_repair_chk_items chk_items_not_repair[]=
{
    {0x12000024, 0xfffff800, CHECK_OR_ALL_ONE, "MJC_MBIST_DONE"},
    {0x2011002c, 0xfffff801, CHECK_OR_ALL_ONE, "MDMCU_MBIST_DONE"},
    {0x2405001c, 0x897fffff, CHECK_OR_ALL_ONE, "TDD_MBIST_DONE"},
    {0x26706038, 0x00000001, CHECK_OR_ALL_ONE, "MML1_FC1_MBIST_DONE_0"},
    {0x2670603c, 0xffffe3e0, CHECK_OR_ALL_ONE, "MML1_FC1_MBIST_DONE_1"},
    {0x26716034, 0xf0000001, CHECK_OR_ALL_ONE, "MML1_BC1_MBIST_DONE_0"},
    {0x26726060, 0x00000001, CHECK_OR_ALL_ONE, "MML1_MC1_MBIST_DONE_0"},
    {0x26726064, 0x00000078, CHECK_OR_ALL_ONE, "MML1_MC1_MBIST_DONE_1"},
    {0x26726068, 0xfffffffe, CHECK_OR_ALL_ONE, "MML1_MC1_MBIST_DONE_2"},
    {0x26736034, 0xfffe0001, CHECK_OR_ALL_ONE, "MML1_FC2_MBIST_DONE_0"},
    {0x26736038, 0xfffffffe, CHECK_OR_ALL_ONE, "MML1_FC2_MBIST_DONE_1"},
    {0x2674602c, 0xfe000001, CHECK_OR_ALL_ONE, "MML1_MC2_MBIST_DONE_0"},
    {0x26756030, 0xf0000001, CHECK_OR_ALL_ONE, "MML1_MC3_MBIST_DONE_0"},
};


struct sram_repair_chk_items chk_items_repaired[]=
{
    {0x12000024, 0xfffff800, CHECK_OR_ALL_ONE,   "MJC_MBIST_DONE"},
    {0x12000028, 0xfffffffe, CHECK_AND_ALL_ZERO, "MJC_MBIST_FAIL_0"},
    {0x1200002c, 0xffffffff, CHECK_AND_ALL_ZERO, "MJC_MBIST_FAIL_1"},
    {0x12000030, 0xffffffff, CHECK_AND_ALL_ZERO, "MJC_MBIST_FAIL_2"},
    {0x2011002c, 0xfffff801, CHECK_OR_ALL_ONE,   "MDMCU_MBIST_DONE"},
    {0x20110030, 0xfffffffe, CHECK_AND_ALL_ZERO, "MDMCU_MBIST_FAIL_0"},
    {0x20110034, 0xffffffff, CHECK_AND_ALL_ZERO, "MDMCU_MBIST_FAIL_1"},
    {0x20110038, 0xffffffff, CHECK_AND_ALL_ZERO, "MDMCU_MBIST_FAIL_2"},
    {0x2011003c, 0xffffffff, CHECK_AND_ALL_ZERO, "MDMCU_MBIST_FAIL_3"},
    {0x2405001c, 0x897fffff, CHECK_OR_ALL_ONE,   "TDD_MBIST_DONE"},
    {0x24050030, 0xf7980000, CHECK_AND_ALL_ZERO, "TDD_MBIST_FAIL_3"},
    {0x24050034, 0x00000001, CHECK_AND_ALL_ZERO, "TDD_MBIST_FAIL_4"},
    {0x26706038, 0x00000001, CHECK_OR_ALL_ONE,   "MML1_FC1_MBIST_DONE_0"},
    {0x2670603c, 0xffffe3e0, CHECK_OR_ALL_ONE,   "MML1_FC1_MBIST_DONE_1"},
    {0x26706040, 0xfffffffe, CHECK_AND_ALL_ZERO, "MML1_FC1_MBIST_FAIL_0"},
    {0x26706044, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_FC1_MBIST_FAIL_1"},
    {0x26706048, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_FC1_MBIST_FAIL_2"},
    {0x2670604c, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_FC1_MBIST_FAIL_3"},
    {0x26706050, 0x00007801, CHECK_AND_ALL_ZERO, "MML1_FC1_MBIST_FAIL_4"},
    {0x26716034, 0xf0000001, CHECK_OR_ALL_ONE,   "MML1_BC1_MBIST_DONE_0"},
    {0x26716038, 0xfffffffe, CHECK_AND_ALL_ZERO, "MML1_BC1_MBIST_FAIL_0"},
    {0x2671603c, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_BC1_MBIST_FAIL_1"},
    {0x26716040, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_BC1_MBIST_FAIL_2"},
    {0x26716044, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_BC1_MBIST_FAIL_3"},
    {0x26716048, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_BC1_MBIST_FAIL_4"},
    {0x2671604c, 0x3fffffff, CHECK_AND_ALL_ZERO, "MML1_BC1_MBIST_FAIL_5"},
    {0x26726060, 0x00000001, CHECK_OR_ALL_ONE,   "MML1_MC1_MBIST_DONE_0"},
    {0x26726064, 0x00000078, CHECK_OR_ALL_ONE,   "MML1_MC1_MBIST_DONE_1"},
    {0x26726068, 0xfffffffe, CHECK_OR_ALL_ONE,   "MML1_MC1_MBIST_DONE_2"},
    {0x2672606c, 0x0000007e, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_0"},
    {0x26726070, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_1"},
    {0x26726074, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_2"},
    {0x26726078, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_3"},
    {0x2672607c, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_4"},
    {0x26726080, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_5"},
    {0x26726084, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_6"},
    {0x26726088, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_7"},
    {0x2672608c, 0xe0001fff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_8"},
    {0x26726090, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_9"},
    {0x26726094, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_A"},
    {0x26726098, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC1_MBIST_FAIL_B"},
    {0x26736034, 0xfffe0001, CHECK_OR_ALL_ONE,   "MML1_FC2_MBIST_DONE_0"},
    {0x26736038, 0xfffffffe, CHECK_OR_ALL_ONE,   "MML1_FC2_MBIST_DONE_1"},
    {0x2673603c, 0xfffffffe, CHECK_AND_ALL_ZERO, "MML1_FC2_MBIST_FAIL_0"},
    {0x26736040, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_FC2_MBIST_FAIL_1"},
    {0x26736044, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_FC2_MBIST_FAIL_2"},
    {0x26736048, 0x00000003, CHECK_AND_ALL_ZERO, "MML1_FC2_MBIST_FAIL_3"},
    {0x2674602c, 0xfe000001, CHECK_OR_ALL_ONE,   "MML1_MC2_MBIST_DONE_0"},
    {0x26746030, 0xfffffffe, CHECK_AND_ALL_ZERO, "MML1_MC2_MBIST_FAIL_0"},
    {0x26746034, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC2_MBIST_FAIL_1"},
    {0x26746038, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC2_MBIST_FAIL_2"},
    {0x2674603c, 0x003fffff, CHECK_AND_ALL_ZERO, "MML1_MC2_MBIST_FAIL_3"},
    {0x26756030, 0xf0000001, CHECK_OR_ALL_ONE,   "MML1_MC3_MBIST_DONE_0"},
    {0x26756034, 0xfffffffe, CHECK_AND_ALL_ZERO, "MML1_MC3_MBIST_FAIL_0"},
    {0x26756038, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC3_MBIST_FAIL_1"},
    {0x2675603c, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC3_MBIST_FAIL_2"},
    {0x26756040, 0xffffffff, CHECK_AND_ALL_ZERO, "MML1_MC3_MBIST_FAIL_3"},
    {0x26756044, 0x01ffffff, CHECK_AND_ALL_ZERO, "MML1_MC3_MBIST_FAIL_4"},
};



int sram_mbist_chk_result(int sarm_repaired)
{
    int error = 0;
    int size, i;
    volatile UINT32 data;
    struct sram_repair_chk_items *chk, *chk_end;

    switch(sarm_repaired){
    case MBIST_CHK_SRAM_NOT_REPAIR:
        size = sizeof(chk_items_not_repair)/ sizeof(struct sram_repair_chk_items);
        chk = &chk_items_not_repair[0];
        break;

    case MBIST_CHK_SRAM_REPAIRED:
        size = sizeof(chk_items_repaired)/ sizeof(struct sram_repair_chk_items);
        chk = &chk_items_repaired[0];
        break;
    }

    chk_end = chk + size;

    while(chk < chk_end)
    {
        if(chk->rule == CHECK_OR_ALL_ONE)
        {
            /* CHECK_OR_ALL_ONE */
            data = *((UINT32P)chk->addr) | chk->mask;
            if(~data)
            {
                dprintf(CRITICAL, "%s: 0x%x.\n", chk->name, data);
                error = 1;
            }
        }
        else
        {
            /* CHECK_AND_ALL_ZERO */
            data = *((UINT32P)chk->addr) & chk->mask;
            if(data)
            {
                dprintf(CRITICAL, "%s: 0x%x.\n", chk->name, data);
                error = 1;
            }

        }
        chk++;
    }

    if(error)
    {
        dprintf(CRITICAL, "ERROR!!!\n");
        dprintf(CRITICAL, "MBIST %s sram repair --- failed\n", sarm_repaired == MBIST_CHK_SRAM_NOT_REPAIR ? "before" : "after");
    }


    return error;

}

int skip_sw_sram_repair(void)
{
    UINT32 value = get_devinfo_with_index(5);

    /* bit 28:
     * 0: skip sw_sram_repair
     * 1: run sw_sram_repair routine
     */

    value &= (0x1 << 28);
    return value ? 0 : 1;
}

int repair_sram(void)
{
    volatile UINT32 rdata;
    UINT32 rstd_reg;
    int error = 0;


    if(skip_sw_sram_repair())
    {
        dprintf(CRITICAL, "SKIP sw sram repair\n");
        return error;
    }

    dprintf(CRITICAL, "repair_sram START\n");

    dprintf(INFO, "TURN ON MD and MJC MTCMOS\n");
    *((UINT32P)0x10006000) = 0x0b160001;

    dprintf(INFO, "TURN ON MMSYS\n");
    *((UINT32P)0x1000623c) = 0x00000f16;
    *((UINT32P)0x1000623c) = 0x00000f1e;
    *((UINT32P)0x1000623c) = 0x00000f0e;
    *((UINT32P)0x1000623c) = 0x00000f0c;
    *((UINT32P)0x1000623c) = 0x00000f0d;
    *((UINT32P)0x1000623c) = 0x00000e0d;
    *((UINT32P)0x1000623c) = 0x00000c0d;
    *((UINT32P)0x1000623c) = 0x0000080d;
    *((UINT32P)0x1000623c) = 0x0000000d;

    dprintf(INFO, "TURN ON MJC\n");
    *((UINT32P)0x10006298) = 0x00000f16;
    *((UINT32P)0x10006298) = 0x00000f1e;
    *((UINT32P)0x10006298) = 0x00000f0e;
    *((UINT32P)0x10006298) = 0x00000f0c;
    *((UINT32P)0x10006298) = 0x00000f0d;
    *((UINT32P)0x10006298) = 0x00000e0d;
    *((UINT32P)0x10006298) = 0x00000c0d;
    *((UINT32P)0x10006298) = 0x0000080d;
    *((UINT32P)0x10006298) = 0x0000000d;

    udelay(10);

    dprintf(INFO, "APPLL alreasy ON\n");
    dprintf(INFO, "MDPLL turn ON\n");
    *((UINT32P)0x201200ac) = 0x000001FF;
    *((UINT32P)0x2012004c) = 0x8300;
    *((UINT32P)0x20120028) = 0x0070;

    udelay(2);

    *((UINT32P)0x2012004c) = 0x0000;
    *((UINT32P)0x20120028) = 0x0000;
    *((UINT32P)0x20120048) = 0x0;
    *((UINT32P)0x20120024) = 0x10;
    *((UINT32P)0x20120700) = 0x10;
    *((UINT32P)0x20120050) = 0x13;
    *((UINT32P)0x20120054) = 0x0A25;

    udelay(3);

    *((UINT32P)0x20120208) = 0x8050;
    *((UINT32P)0x20120100) = 0x0000c70f;
    *((UINT32P)0x20120140) = 0x00008410;
    *((UINT32P)0x201201c0) = 0x00008800;
    *((UINT32P)0x20120200) = 0x00008800;
    *((UINT32P)0x20120150) = 0x00008810;
    *((UINT32P)0x201201E0) = 0x00008410;

    udelay(10);

    *((UINT32P)0x2012003C) = 0x0113;
    *((UINT32P)0x2012003C) = 0x0133;
    *((UINT32P)0x2012003C) = 0x5133;
    *((UINT32P)0x20120100) &= (~0x00008000);
    *((UINT32P)0x20120110) = 0x0002;
    *((UINT32P)0x20120100) |= 0x00008000;
    *((UINT32P)0x2012008C) = 0xC100;
    *((UINT32P)0x2000045c) = 0x21008510;
    *((UINT32P)0x20120060) = 0x2020;
    *((UINT32P)0x20120064) = 0x2000;
    *((UINT32P)0x20120068) = 0x6F40;
    *((UINT32P)0x2012006c) = 0x0423;
    *((UINT32P)0x20120094) = 0x4500;
    *((UINT32P)0x20120098) = 0x0020;
    *((UINT32P)0x2012009C) = 0x0003;
    *((UINT32P)0x201200A0) = 0x0001;
    *((UINT32P)0x201200A4) = 0x00FF;
    *((UINT32P)0x20000458) = 0xFFF50147;

    udelay(20);

    dprintf(INFO, "disable MJC DCM\n");
    *((UINT32P)0x12000000) = 0x00000000;
    *((UINT32P)0x12000010) = 0xffffffff;

    dprintf(INFO, "Turn ON TDD clock\n");
    *((UINT16P)0x24000422) = 0x4911;
    *((UINT16P)0x24000422) = 0xc911;

    dprintf(INFO, "enable mbist\n");
    // MJC
    *((UINT32P)0x12000040) = 0x00000000;
    *((UINT32P)0x12000040) = 0x00010000;
    *((UINT32P)0x12000050) = 0x00000000;
    *((UINT32P)0x12000050) = 0x00000001;
    *((UINT32P)0x12000038) = 0x00000000;
    *((UINT32P)0x12000038) = 0x000007ff;

    // MDMCU
    *((UINT32P)0x200004B0) = 0x00000001;
    *((UINT32P)0x20110074) = 0x00000000;
    *((UINT32P)0x20110074) = 0x00000007;

    // TDD
    *((UINT32P)0x24050068) = 0x00000000;
    *((UINT32P)0x24050068) = 0x00000001;
    *((UINT32P)0x24050000) = 0x3b400000;

    // MML1
    *((UINT32P)0x2670600c) = 0x00000000;
    *((UINT32P)0x2673600c) = 0x00000000;
    *((UINT32P)0x26716008) = 0x00000000;
    *((UINT32P)0x26726010) = 0x00000000;
    *((UINT32P)0x26746008) = 0x00000000;
    *((UINT32P)0x26756008) = 0x00000000;
    *((UINT32P)0x26756008) = 0x00000000;

    *((UINT32P)0x20110074) = 0x00000000;
    *((UINT32P)0x20110074) = 0x00000007;
    *((UINT32P)0x200004B0) = 0x00000201;

    udelay(1000);

    dprintf(INFO, "1. AFTER MBIST_RUN\n");
    error = sram_mbist_chk_result(MBIST_CHK_SRAM_NOT_REPAIR);

    if(error)
        return error;

    dprintf(INFO, "infra fuse_latch\n");
    *((UINT32P)0x100011a8) |= 0x00080000;

    dprintf(INFO, "mdmcu fuse_latch\n");
    *((UINT32P)0x200004b0) = 0x00000008;

    dprintf(INFO, "tdd fuse_latch\n");
    *((UINT16P)0x2400001e) = 0x004d;

    dprintf(INFO, "mml1 fuse_latch\n");
    *((UINT32P)0x26612028) = 0x00000004;
    *((UINT32P)0x2660202c) = 0x00000004;
    *((UINT32P)0x26632024) = 0x00000004;
    *((UINT32P)0x26622040) = 0x00000004;
    *((UINT32P)0x26642028) = 0x00000004;
    *((UINT32P)0x26652028) = 0x00000004;
    *((UINT32P)0x26652028) = 0x00000004;
    *((UINT32P)0x26612028) = 0x00000000;
    *((UINT32P)0x2660202c) = 0x00000000;
    *((UINT32P)0x26632024) = 0x00000000;
    *((UINT32P)0x26622040) = 0x00000000;
    *((UINT32P)0x26642028) = 0x00000000;
    *((UINT32P)0x26652028) = 0x00000000;
    *((UINT32P)0x26652028) = 0x00000000;

    dprintf(INFO, "rerun mbist\n");
    // MJC
    *((UINT32P)0x12000040) = 0x00000000;
    *((UINT32P)0x12000040) = 0x00010000;
    *((UINT32P)0x12000050) = 0x00000000;
    *((UINT32P)0x12000050) = 0x00000001;
    *((UINT32P)0x12000038) = 0x00000000;
    *((UINT32P)0x12000038) = 0x000007ff;

    // MDMCU
    *((UINT32P)0x200004B0) = 0x00000001;
    *((UINT32P)0x20110074) = 0x00000000;
    *((UINT32P)0x20110074) = 0x00000007;

    // TDD
    *((UINT32P)0x24050068) = 0x00000000;
    *((UINT32P)0x24050000) = 0x00000000;
    *((UINT32P)0x24050068) = 0x00000001;
    *((UINT32P)0x24050000) = 0x3b400000;

    // MML1
    *((UINT32P)0x20110074) = 0x00000000;
    *((UINT32P)0x20110074) = 0x00000007;
    *((UINT32P)0x200004B0) = 0x00000201;

    udelay(1000);
    dprintf(INFO, "2. AFTER MBIST_RERUN\n");
    error = sram_mbist_chk_result(MBIST_CHK_SRAM_REPAIRED);

    dprintf(CRITICAL, "repair_sram END\n");

    return error;
}

