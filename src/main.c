#include <vitasdk.h>
#include <taihen.h>
#define printf sceClibPrintf


SceUID hook;
tai_hook_ref_t ref;

SceUInt32 USBDisable_Patch(void)
{
    //No need to display dialog (Pretend we've shown it before and it connected successfully)
    return 1;
}

int DisableThread(SceSize args, void *argp)
{
    sceKernelDelayThread(5 * 1000000);

    tai_module_info_t modinfo;
    modinfo.size = sizeof(modinfo);

    if(taiGetModuleInfo("SceShell", &modinfo) >= 0)
    {
        switch (modinfo.module_nid)
        {
            case 0x0552F692: // 3.60 retail
                hook = taiHookFunctionOffset(&ref, modinfo.modid, 0, 0x1c9bfa, 1, USBDisable_Patch); 
                break;

            case 0xEAB89D5C: // 3.60 testkit
                hook = taiHookFunctionOffset(&ref, modinfo.modid, 0, 0x1c202e, 1, USBDisable_Patch); 
                break;
            
            case 0x587F9CED: // 3.65 testkit
                hook = taiHookFunctionOffset(&ref, modinfo.modid, 0, 0x1c20f6, 1, USBDisable_Patch);
                break;
                
            case 0x5549BF1F: // 3.65 retail
            case 0x34B4D82E: // 3.67 retail
            case 0x12DAC0F3: // 3.68 retail
            case 0x0703C828: // 3.69 retail
            case 0x2053B5A5: // 3.70 retail
            case 0xF476E785: // 3.71 retail
            case 0x939FFBE9: // 3.72 retail
            case 0x734D476A: // 3.73 retail
            case 0x51CB6207: // 3.74 retail
                hook = taiHookFunctionOffset(&ref, modinfo.modid, 0, 0x1c9cc2, 1, USBDisable_Patch); 
                break;
        }
    }

    return sceKernelExitDeleteThread(0);
}


void _start() __attribute__((weak, alias("module_start")));
int module_start(SceSize argc, const void *args)
{
    printf("USBDisable by Ibrahim\n");
    int thread = sceKernelCreateThread("usbDisable", DisableThread, 191, 0x1000, 0, 0, NULL);
    sceKernelStartThread(thread, 0, NULL);
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
    if(hook >= 0)
        taiHookRelease(hook, ref);

    return SCE_KERNEL_STOP_SUCCESS;
}
