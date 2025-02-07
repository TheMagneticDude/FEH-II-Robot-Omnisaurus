#include <FEHLCD.h>
#include <Windows.h>
//gamepad
#include <concrt.h>
#include <winrt/Windows.Gaming.Input.h>
using namespace winrt;
using namespace Windows::Gaming::Input;
int main(void)
{
    LCD.Clear(BLACK);
    LCD.WriteLine("Hello, World!");
	return 0;
}
