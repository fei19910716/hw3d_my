#include "App.h"
#include "Graphics/Drawable/Geometry/Triangle.h"
#include "Graphics/Drawable/Geometry/Box.h"
#include <sstream>
#include <iomanip>
#include <memory>
App::App():wnd(640,480,TEXT("The Donkey Fart Box")){
	std::mt19937 rng( std::random_device{}() );
	std::uniform_real_distribution<float> adist( 0.0f,3.1415f * 2.0f );
	std::uniform_real_distribution<float> ddist( 0.0f,3.1415f * 1.0f );
	std::uniform_real_distribution<float> odist( 0.0f,3.1415f * 0.08f );
	std::uniform_real_distribution<float> rdist( 6.0f,20.0f );
	for( auto i = 0; i < 2; i++ )
	{
		boxes.push_back( std::make_unique<Box>(
			wnd.GetGraphics(),rng,adist,
			ddist,odist,rdist
		) );
	}

	for( auto i = 0; i < 1; i++ )
	{
		triangles.push_back( std::make_unique<Triangle>(
			wnd.GetGraphics()
		) );
	}

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
    auto dt = timer.Peek();
	wnd.GetGraphics().ClearBuffer(0.7f,0.0f,0.52f);
	for(auto& b: boxes){
		b->Update(dt);
		b->Draw(wnd.GetGraphics());
	}
	for(auto& b: triangles){
		b->Update(dt);
		b->Draw(wnd.GetGraphics());
	}
    wnd.GetGraphics().EndFrame();
}