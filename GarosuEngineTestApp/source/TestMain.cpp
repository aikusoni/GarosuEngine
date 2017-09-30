#include <GarosuTypedef.h>

#include <GarosuEngine.h>

#include <Windows.h>

using namespace Garosu;

class TestApp
{
public:
	TestApp(void);
	TestApp(const TestApp&) = delete;
	TestApp& operator=(const TestApp&) = delete;

	virtual ~TestApp(void);
};

TestApp::TestApp(void)
{

}

TestApp::~TestApp(void)
{

}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	return 0;
}