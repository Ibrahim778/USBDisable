#include <vitasdk.h>

/*    sceShellUtilInitEvents(0);
    sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_QUICK_MENU |
                    SCE_SHELL_UTIL_LOCK_TYPE_POWEROFF_MENU |
                    SCE_SHELL_UTIL_LOCK_TYPE_USB_CONNECTION |
                    SCE_SHELL_UTIL_LOCK_TYPE_MC_INSERTED |
                    SCE_SHELL_UTIL_LOCK_TYPE_MC_REMOVED |
                    SCE_SHELL_UTIL_LOCK_TYPE_MUSIC_PLAYER |
                    SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN_2);*/

int DisableThread(SceSize args, void *argp)
{
    sceKernelDelayThread(4 * 1000000);
    sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_USB_CONNECTION);
    return sceKernelExitDeleteThread(0);
}

void _start() __attribute__((weak, alias("module_start")));
int module_start(SceSize argc, const void *args)
{
    int thread = sceKernelCreateThread("usbDisable", DisableThread, 191, 0x1000, 0, 0, NULL);
    sceKernelStartThread(thread, 0, NULL);
    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
    sceShellUtilUnlock(SCE_SHELL_UTIL_LOCK_TYPE_USB_CONNECTION);
    return SCE_KERNEL_STOP_SUCCESS;
}