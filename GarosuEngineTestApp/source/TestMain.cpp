#include <GarosuTypedef.h>

#include <GarosuEngine.h>

#include <Windows.h>

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
	Garosu::Engine engine;
	engine.Initialize(NULL);
	engine.Finalize();

	return 0;
}