#include "KeyboardHook.h"


LRESULT LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode != HC_ACTION) return CallNextHookEx(NULL, nCode, wParam, lParam); // курим мсдн
    /*
    nCode
    [in] Specifies a code the hook procedure uses to determine how to process the message.
    If nCode is less than zero, the hook procedure must pass the message to the CallNextHookEx function
    without further processing and should return the value returned by CallNextHookEx.
    This parameter can be one of the following values.
    HC_ACTION
    */
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
    switch (wParam)
    {
    /*
        wParam
        [in] Specifies the identifier of the keyboard message.
        This parameter can be one of the following messages:
        WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP.
        */
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        //Блокируем системные клавиши
        if((p->vkCode == VK_ACCEPT)||
                (p->vkCode == VK_F1)||
                p->vkCode ==VK_MENU
                )
        {
            //MessageBeep(0xffffffff);
            //printf("%d",p->vkCode);
            return(1); // блокируем кнопку пуск,
        };

//        if (p->vkCode == VK_TAB && p->flags == LLKHF_ALTDOWN ) return 1; // Alt+Tab
//        if (p->vkCode == VK_LSHIFT && p->flags == LLKHF_ALTDOWN ) return 1;
//        if (p->flags == LLKHF_ALTDOWN ) return 1;
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        //Отжатия клавиш тоже никчему пропускать.
        if((p->vkCode == VK_ACCEPT)||
                (p->vkCode == VK_F1)||
                p->vkCode ==VK_MENU

                )
        {
            //MessageBox(0,"Button START is pressed or released","OK",MB_OK);
            //Поскольку приложение засыпает, не выйдя из обработчика пуск открывается.
            return(1); // блокируем кнопку пуск,
        };
        break;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
