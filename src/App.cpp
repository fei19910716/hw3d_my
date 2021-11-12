#include "App.h"
#include "Graphics/Drawable/Geometry/Triangle.h"
#include "Graphics/Drawable/Geometry/Box.h"
#include "Graphics/Drawable/Geometry/SkinnedBox.h"
#include "Graphics/Drawable/Geometry/Melon.h"
#include "Graphics/Drawable/Geometry/Cylinder.h"
#include "Graphics/Drawable/Geometry/Pyramid.h"
#include "Graphics/Drawable/Geometry/Sheet.h"
#include "Graphics/Drawable/Geometry/AssTest.h"
#include "Graphics/Drawable/Geometry/MathUtil.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>

#include "Graphics/Surface.h"
#include "Graphics/GDIPlusManager.h"
#include "imgui/imgui.h"


GDIPlusManager gdipm;


App::App():wnd(640,480,TEXT("The Donkey Fart Box")), light(wnd.GetGraphics()){

	wnd.GetGraphics().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,480.0f / 640.0f,0.5f,40.0f ) );
}

App::~App(){

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
    auto dt = timer.Mark() * speed_factor;

	wnd.GetGraphics().BeginFrame(0.7f,0.0f,0.52f);
	wnd.GetGraphics().SetCamera(camera.GetMatrix());
	light.Bind(wnd.GetGraphics(),camera.GetMatrix());

	nano.Draw( wnd.GetGraphics() );
	light.Draw(wnd.GetGraphics());

	// imgui windows
	camera.SpawnControlWindow();
	light.SpawnControlWindow();

	// present
    wnd.GetGraphics().EndFrame();
}