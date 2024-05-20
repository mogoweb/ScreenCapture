#include <QApplication>

#include <Windows.h>
#include "include/core/SkGraphics.h"
#include "App.h"

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Failed to initialize COM library.", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    SkGraphics::Init();
    App::Init(std::string(*argv));
    a.exec();
    App::Dispose();
    CoUninitialize();
    auto code = App::GetExitCode();

    return code;
}
