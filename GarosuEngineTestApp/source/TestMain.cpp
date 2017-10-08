#include <GarosuTypedef.h>

#include <GarosuEngine.h>

#include <Windows.h>

class EngineTestWindow
{
public:
	EngineTestWindow(HINSTANCE);
	EngineTestWindow(const EngineTestWindow&) = delete;
	EngineTestWindow& operator=(const EngineTestWindow&) = delete;

	virtual ~EngineTestWindow(void);

	bool Initialize(void);
	bool Finalize(void);

	void Run(void);

private:
	Garosu::Engine engine;
};

EngineTestWindow::EngineTestWindow(HINSTANCE hInstance)
{

}

EngineTestWindow::~EngineTestWindow(void)
{

}

bool EngineTestWindow::Initialize(void)
{
	// initialize window

	// initialize engine

	return true;
}

bool EngineTestWindow::Finalize(void)
{
	// finalize engine

	// finalize window

	return true;
}

void EngineTestWindow::Run(void)
{
	// windowws message loop
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	EngineTestWindow win(hInstance);

	do {
		if (!win.Initialize()) break;

		win.Run();

		if (!win.Finalize()) break;
	} while (false);

	return 0;
}