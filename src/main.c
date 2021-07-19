#include <vitasdk.h>
#include <taihen.h>
#define printf sceClibPrintf


SceUID hook;
tai_hook_ref_t ref;

SceUInt32 FUN_811c9bfa_Patch(void)
{
    //No need to display dialog (Pretend we've shown it before and it connected successfully)
    return 1;
}

int DisableThread(SceSize args, void *argp)
{
    sceKernelDelayThread(5 * 1000000);

    tai_module_info_t modinfo;
    modinfo.size = sizeof(modinfo);
    taiGetModuleInfo("SceShell", &modinfo);

    hook = taiHookFunctionOffset(&ref, modinfo.modid, 0, 0x1c9bfa, 1, FUN_811c9bfa_Patch);    

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