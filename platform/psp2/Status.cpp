#include "Status.h"
#include "Logger.h"
#include "Quirks.h"
#include "platform.h"
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

using namespace WalrusRPG;

namespace
{
    static bool askedToQuit;
}

void Status::init()
{
    askedToQuit = false;
}

void Status::deinit()
{
    sceKernelExitProcess(0);
}

void Status::update()
{
    // if (!aptMainLoop())
    //     askedToQuit = true;
    if (Quirks::get_key(SCE_CTRL_TRIANGLE))
        askedToQuit = true;
}

bool Status::mustQuit()
{
    return askedToQuit;
}
