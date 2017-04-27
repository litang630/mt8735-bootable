LOCAL_DIR := $(GET_LOCAL_DIR)

TARGET := wisky8735m_3g_h

MODULES += app/mt_boot \
           dev/lcm


MTK_EMMC_SUPPORT = yes
DEFINES += MTK_NEW_COMBO_EMMC_SUPPORT
MTK_KERNEL_POWER_OFF_CHARGING = yes
DEFINES += SWCHR_POWER_PATH
DEFINES += MTK_NCP1854_SUPPORT
DEFINES += MTK_BATLOWV_NO_PANEL_ON_EARLY
MTK_LCM_PHYSICAL_ROTATION = 0
CUSTOM_LK_LCM="ek79023_dsi_wsvga_vdo"
#hx8392a_dsi_cmd = yes

#FASTBOOT_USE_G_ORIGINAL_PROTOCOL = yes
MTK_SECURITY_SW_SUPPORT = yes
MTK_VERIFIED_BOOT_SUPPORT = yes
MTK_SEC_FASTBOOT_UNLOCK_SUPPORT = yes

DEBUG := 0
BOOT_LOGO=wsvga

#DEFINES += WITH_DEBUG_DCC=1
DEFINES += WITH_DEBUG_UART=1
#DEFINES += WITH_DEBUG_FBCON=1
#DEFINES += MACH_FPGA=y
#DEFINES += SB_LK_BRINGUP=y
DEFINES += MTK_GPT_SCHEME_SUPPORT
#DEFINES += MACH_FPGA_NO_DISPLAY=y
