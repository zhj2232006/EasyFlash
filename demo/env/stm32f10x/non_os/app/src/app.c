#include "bsp.h"
#include "easyflash.h"
#include <stdio.h>
#include <stdlib.h>

#define APPLICATION_ADDRESS 0x08004000

static uint32_t update_file_total_size, iap_need_copy_app;
static void test_env(void);

typedef  void (*pFunction)(void);
__IO pFunction JumpToApplication;
__IO uint32_t JumpAddress;

int load_app(void)
{
    /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);

        /* Jump to application */
        JumpToApplication();

    }
    return 1;
}

static uint32_t iap_get_app_addr()
{
    return APPLICATION_ADDRESS;
}

int main(void){
#if 0
    BSP_Init();
#endif
    if (easyflash_init() == EF_NO_ERR) {
        /* test Env demo */
        //test_env();
    }
    
    char* c_iap_need_copy_app = ef_get_env("iap_need_copy_app");    
    iap_need_copy_app = atoi(c_iap_need_copy_app);
    char* c_file_size = ef_get_env("iap_copy_app_size");
    update_file_total_size = atoi(c_file_size);
    
    if(update_file_total_size && iap_need_copy_app)
    {
        /* copy downloaded application to application entry */
        if (ef_erase_user_app(iap_get_app_addr(), update_file_total_size)
                || ef_copy_app_from_bak(iap_get_app_addr(), update_file_total_size)) {
            // rt_kprintf("Update user app fail.\n");
        } else {
            // rt_kprintf("Update user app success.\n");
        }
        /* clean need copy application from backup section flag */
        ef_set_env("iap_need_copy_app", "0");
        ef_set_env("iap_copy_app_size", "0");
        ef_save_env();
    }

    load_app();
	// TODO: :LEDµ„¡¡
    while(1) {
      LED_RUN_ON;
      delay(1000000);
      LED_RUN_OFF;
      delay(1000000);      
    }

    return 0;
}

/**
 * Env demo.
 */
static void test_env(void) {
    uint32_t i_boot_times = NULL;
    char *c_old_boot_times, c_new_boot_times[11] = {0};

    /* get the boot count number from Env */
    c_old_boot_times = ef_get_env("boot_times");
    assert_param(c_old_boot_times);
    i_boot_times = atol(c_old_boot_times);
    /* boot count +1 */
    i_boot_times ++;
    EF_INFO("The system now boot %d times\n\r", i_boot_times);
    /* interger to string */
    sprintf(c_new_boot_times,"%ld", i_boot_times);
    /* set and store the boot count number to Env */
    ef_set_env("boot_times", c_new_boot_times);
    ef_save_env();
}
