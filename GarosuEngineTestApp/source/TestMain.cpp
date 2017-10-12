#include <GarosuTypedef.h>

#include <GarosuEngine.h>

#include <Windows.h>

class EngineTestWindow
{
public:
	bool Initialize(void);
	bool Finalize(void);
};

bool EngineTestWindow::Initialize(void)
{

	return true;
}

bool EngineTestWindow::Finalize(void)
{

	return true;
}

class EngineTestApp
{
public:
	EngineTestApp(HINSTANCE);
	EngineTestApp(const EngineTestApp&) = delete;
	EngineTestApp& operator=(const EngineTestApp&) = delete;

	virtual ~EngineTestApp(void);

	bool Initialize(void);
	bool Finalize(void);

	void Run(void);

private:
	Garosu::IEngine* engine;
	EngineTestWindow window;
};

EngineTestApp::EngineTestApp(HINSTANCE hInstance)
{

}

EngineTestApp::~EngineTestApp(void)
{

}

bool EngineTestApp::Initialize(void)
{
	// initialize window
	if (!window.Initialize()) return false;

	// initialize engine
	if (engine != nullptr) return false;
	engine = MakeGarosuEngine();
	if (!engine->Initialize()) return false;

	return true;
}

bool EngineTestApp::Finalize(void)
{
	bool ret = false;
	// finalize engine
	if (engine != nullptr)
	{
		ret = engine->Finalize();
		delete engine;
		engine = nullptr;
	}
	if (!ret)
	{
		// engine finalization failed
	}

	// finalize window
	ret = window.Finalize();
	if (!ret)
	{

	}

	return true;
}

void EngineTestApp::Run(void)
{
	// windows message loop
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	EngineTestApp win(hInstance);
	
	bool ret = win.Initialize();
	if (ret)
		win.Run();
	win.Finalize();

	return 0;
}