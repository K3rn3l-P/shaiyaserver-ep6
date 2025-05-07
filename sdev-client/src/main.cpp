#include <util/util.h>
#include "include/main.h"
#include "include/shaiya/include/CCharacter.h"
#include "include/shaiya/include/CWindow.h"

using namespace shaiya;



void Main()
{
    hook::gui();
    hook::packet();

    //Nuovi setting
    hook::camera_limit();
    hook::cmd();

    //Nuovi setting
    // character memory (0x43C to 0x444)
    util::write_memory((void*)0x41CC97, 0x44, 1);
    util::write_memory((void*)0x41F055, 0x44, 1);
    util::write_memory((void*)0x476F9F, 0x44, 1);
    util::write_memory((void*)0x490250, 0x44, 1);
    util::write_memory((void*)0x4EFE68, 0x44, 1);
    util::write_memory((void*)0x4F7318, 0x44, 1);
    util::write_memory((void*)0x59A3F3, 0x44, 1);
    util::write_memory((void*)0x59A6DD, 0x44, 1);
}
