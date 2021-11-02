#include "App.h"
#include "Graphics/Drawable/Geometry/Triangle.h"
#include "Graphics/Drawable/Geometry/Box.h"
#include "Graphics/Drawable/Geometry/Melon.h"
#include "Graphics/Drawable/Geometry/Pyramid.h"
#include "Graphics/Drawable/Geometry/MathUtil.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
App::App():wnd(640,480,TEXT("The Donkey Fart Box")){
	class Factory
{
	public:
		Factory( Graphics& gfx )
			:
			gfx( gfx )
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch( typedist( rng ) )
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx,rng,adist,ddist,
					odist,rdist
				);
			case 1:
				return std::make_unique<Box>(
					gfx,rng,adist,ddist,
					odist,rdist,bdist
				);
			case 2:
				return std::make_unique<Melon>(
					gfx,rng,adist,ddist,
					odist,rdist,longdist,latdist
				);
			default:
				assert( false && "bad drawable type in factory" );
				return {};
			}
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
		std::uniform_int_distribution<int> typedist{ 0,2 };
	};

	Factory f( wnd.GetGraphics() );
	drawables.reserve( nDrawables );
	std::generate_n( std::back_inserter( drawables ),nDrawables,f );
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
	for(auto& b: drawables){
		b->Update(dt);
		b->Draw(wnd.GetGraphics());
	}
    wnd.GetGraphics().EndFrame();
}