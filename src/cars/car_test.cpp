// car_test.cpp : Defines the entry point for the console application.
//

#include <tchar.h>
#include <irrlicht.h>

#include "Car.h"
#include "World.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

int _tmain(int argc, _TCHAR* argv[])
{
	MyEventReceiver receiver;

	// init Irrlicht
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640,480), 16, false, false, false, &receiver);
	if (!device)
		return 1;

	device->setWindowCaption(L"Cars and Walls");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	//gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	// init ODE
	dInitODE2(0);
	
	CWorld *world = CWorld::create(smgr, device->getFileSystem());
	CCar *car = world->addCar("../media/car5.xml", core::vector3df(4.0f, 0.0f, 0.0f));
	
	CCar *dam_car = world->addCar("../media/car5.xml", core::vector3df(0.5f, 0.0f, 0.0f));
	dam_car->damage();
	
	// setup camera
	scene::ICameraSceneNode * camera = smgr->addCameraSceneNodeFPS(0,100.0f,0.01f);
	//scene::ICameraSceneNode * camera = smgr->addCameraSceneNode();
	camera->setPosition(core::vector3df(0.0f, 0.0f, -4.0f));
	camera->setTarget(core::vector3df(0.0f, 0.0f, 0.0f));

	device->getCursorControl()->setVisible(false);

	while (device->run())
	{
		// process keyboard clicks
		if (receiver.IsKeyDown(irr::KEY_KEY_W))
		{
			car->accelerate();
			dam_car->accelerate();
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_S))
		{
			car->slowdown();
			dam_car->slowdown();
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_A))
		{
			car->turnLeft();
			dam_car->turnLeft();
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_D))
		{
			car->turnRight();
			dam_car->turnRight();
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_R))
		{
			dam_car->setPosition(core::vector3df(0.0f, 3.0f, 0.0f));
			dam_car->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));	
		}

		driver->beginScene(true, true, video::SColor(255,100,101,140));
		smgr->drawAll();
		driver->endScene();
	}

	delete world;

	// destroy ODE	
	dCloseODE();

	// destroy irricht
	device->drop();

	return 0;
}
