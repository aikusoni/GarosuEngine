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

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	return 0;
}