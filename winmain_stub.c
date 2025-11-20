// winmain_stub.c
// Small wrapper so MinGW can find WinMain and call our normal main()

#include <windows.h>

// Declare main so WinMain can call it
int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nShowCmd) {
    return main(__argc, __argv);
}
