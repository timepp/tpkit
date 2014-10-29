#include "stdafx.h"
#include <tplib/include/cmdlineparser.h>
#include <tplib/include/convert.h>
#include <tplib/include/service.h>
#include <tplib/include/tplib.h>
#include <iostream>
#include <sstream>

const wchar_t* const CMDOPT_ACTION = L"action";

//LONG WINAPI WinStationBroadcastSystemMessage(
//    HANDLE  hServer,
//    BOOL    sendToAllWinstations,
//    ULONG   sessionID,
//    ULONG   timeOut,
//    DWORD   dwFlags,
//    DWORD   *lpdwRecipients,
//    ULONG   uiMessage,
//    WPARAM  wParam,
//    LPARAM  lParam,
//    LONG    *pResponse
//);

void NotifyAllTerminals()
{
    // broadcast to all terminal users
    //DWORD   dwRecipients = 0;
    //LONG    dwResponse = 0;
    //WinStationBroadcastSystemMessage(
    //    (HANDLE) NULL, TRUE, 0, 1, BSF_NOHANG, &dwRecipients,
    //    WM_WININICHANGE, 0L, (LPARAM) L"Environment", &dwResponse);
}

int dispatch_action(const std::wstring& action_name)
{
    if (action_name == L"BroadcastEnvironmentChange")
    {
        BOOL ret = ::SendNotifyMessageW(HWND_BROADCAST, WM_WININICHANGE, 0L, (LPARAM) L"Environment");
        tp::throw_winerr_when(ret != TRUE);
    }
    else
    {
        std::wcout << L"Invalid action: " << action_name << std::endl;
    }

    return 0;
}

int rollonce(int n)
{
    int p = 0;
    for (;;)
    {
        int s = rand() % 6 + 1;
        p += s;
        if (p == n) return 1;
        if (p > n) return 0;
    }
}

int wmain(int argc, WCHAR* argv[])
{
    tp::helper::register_tp_global_services();
    tp::cmdline_parser parser;
    parser.register_string_option(L"a", CMDOPT_ACTION);

    try
    {
        parser.parse(argc, argv);
        if (parser.option_exists(CMDOPT_ACTION))
        {
            std::wstring action_name = parser.get_string_option(CMDOPT_ACTION, L"");
            return dispatch_action(action_name);
        }
    }
    catch (tp::exception& e)
    {
        std::wcout << L"Error: " << e.message << std::endl;
    }

    // show help
    std::wcerr <<
        "Actions:\n"
        "    BroadcastEnvironmentChange: send a notify message indicating environment is changed.\n"
        ;

    return 1;
}

