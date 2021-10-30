#include "App.h"
#include <sstream>
#include <iomanip>
App::App():wnd(640,480,TEXT("The Donkey Fart Box")){

}

int App::Go() {
    while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
    const float t = sin(timer.Peek())/2.0f + 0.5f;
    wnd.GetGraphics().ClearBuffer(t,t,1.0f);
	wnd.GetGraphics().DrawTestTriangle();
    wnd.GetGraphics().EndFrame();
}