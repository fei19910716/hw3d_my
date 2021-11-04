#include "App.h"
#include "Graphics/Drawable/Geometry/Triangle.h"
#include "Graphics/Drawable/Geometry/Box.h"
#include "Graphics/Drawable/Geometry/SkinnedBox.h"
#include "Graphics/Drawable/Geometry/Melon.h"
#include "Graphics/Drawable/Geometry/Pyramid.h"
#include "Graphics/Drawable/Geometry/Sheet.h"
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
	class Factory
{
	public:
		Factory( Graphics& gfx )
			:
			gfx( gfx )
		{}
		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist( rng ),cdist( rng ),cdist( rng ) };
			return std::make_unique<Box>(
				gfx,rng,adist,ddist,
				odist,rdist,bdist,mat
			);
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,4 };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
	};

	drawables.reserve( nDrawables );
	std::generate_n( std::back_inserter( drawables ),nDrawables,Factory{ wnd.GetGraphics() } );
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
	static float speed_factor = 1.0f;
    auto dt = timer.Mark() * speed_factor;

	wnd.GetGraphics().BeginFrame(0.7f,0.0f,0.52f);
	wnd.GetGraphics().SetCamera(camera.GetMatrix());
	light.Bind(wnd.GetGraphics());

	for(auto& b: drawables){
		b->Update(wnd.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : dt);
		b->Draw(wnd.GetGraphics());
	}
	light.Draw(wnd.GetGraphics());

	// imgui window to control simulation speed
	if( ImGui::Begin( "Simulation Speed" ) )
	{
		ImGui::SliderFloat( "Speed Factor",&speed_factor,0.0f,4.0f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)",1000.0f / ImGui::GetIO().Framerate,ImGui::GetIO().Framerate );
		ImGui::Text( "Status: %s",wnd.kbd.KeyIsPressed( VK_SPACE ) ? "PAUSED" : "RUNNING" );
	}
	ImGui::End();

	camera.SpawnControlWindow();
	light.SpawnControlWindow();

	// present
    wnd.GetGraphics().EndFrame();
}