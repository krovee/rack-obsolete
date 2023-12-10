
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*
* Rack Engine Runtime Starter include
*/
#include <RE/RT/Starter.h>

int WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
        
    re::main_starter::create_info starter_info = {
        nShowCmd
    };

    re::main_starter main_starter(starter_info);
    return 0;
}
