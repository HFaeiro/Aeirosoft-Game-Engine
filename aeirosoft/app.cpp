#include "app.h"
#include "Inc/DirectX/SimpleMath.h"
#include "Player.h"
#include <fstream>
#include <sstream>
#include "Zombie.h"
#include <thread>
app::app(HINSTANCE hinst, const std::wstring windowName, POINT p) :
	window(hinst, windowName, p)
{
	hInst = hinst;
	m_Window = getHWND();
	m_Timer.Start();

}

app::~app()
{
	DestroyWindow(getHWND());
}
bool app::SetupApplication()
{

	//cleanup
	events.clear();
	if (s != nullptr) {
		delete s;
	}

	vBoxes.clear();
	if (gui != nullptr)
		delete gui;
	////

	s = new Scenes(m_Graphics, i);
	gui = new Gui(m_Graphics, i);
	StartupGui(gui);

	s->CreateScene(L"Scene1", gui, false, true);
	s->CreateScene(L"MainMenu", gui, true);




	auto t1 = std::thread(&app::AddEntitiesToScene,this,  std::ref(s), std::ref(gui));

	s->AddOnKeyEventToScene(L"Scene1", L"Pause", gui, false, DIK_ESCAPE);

	events.push_back(i);
	events.push_back(s);


	s->SetActiveScene(restart ? L"Scene1" : L"MainMenu");

	t1.join();
	//t2.join();
	for (const auto& E : events)
		if (!E->Initialize())
			return false;

}
std::vector<int> app::GetStartInfo(std::string filename, std::vector<std::string> vsfind, std::vector<int> defaults)
{
	FILE* file = nullptr;
	errno_t err =  fopen_s(&file, filename.c_str(), "r");
	std::vector<int> vDat;
	if (!err)
	{
		std::string buffer;
		buffer.resize(200);
		if (fread(buffer.data(), sizeof(char), 200, file))
		{
			int count = 0;
			for (const auto& find : vsfind)
			{
				std::size_t found = buffer.find(find);
				if (found != std::string::npos)
				{
					found += find.length();
					vDat.push_back(atoi(&buffer[found]));
				}
				else
				{

					vDat.push_back(defaults[count]);
				}
				count++;
			}
		}

	}
	else
	{

		err = fopen_s(&file, filename.c_str(), "a+");
		if (!err)
		{
			int count = 0;
			for (auto& find : vsfind)
			{

				find += std::to_string(defaults[count]) + "\n";
				fwrite(find.c_str(), sizeof(char), find.length(), file);
				vDat.push_back(defaults[count]);
				count++;
			}
		}
	}
	if(file)
		fclose(file);
	return vDat;
}
int app::begin()
{

	std::vector<int> settings = GetStartInfo("settings.txt", { "FullScreen:", "VSYNC:", "BOXES:" }, { 0, 0, 3 });
	
	m_Graphics = new graphics(m_Window, settings[0], settings[1]);

	if (!m_Graphics->Initialize())
		return false;

	i = new input(m_Graphics, static_cast<window*>(this), &m_Graphics->m_Camera, &m_Timer);

	Boxes = settings[2];
	SetupApplication();
	double loadTime = m_Timer.GetSecondsElapsed();
	std::wstringstream wss;
	wss << L"Load Time: " << loadTime << "s\n Enemies: " << Boxes;
	try {
		while (true)
		{
			if (const auto optional = processMessages())
				return *optional;

			m_Graphics->BeginScene(0.f, 0.f, 0.f, 1.f);

			// start of event que
			std::vector<Events*> queued;

			for (const auto& E : events)
				if (const auto optional = E->Queue())
					queued.emplace_back(*optional);

			for (const auto& E : queued)
				E->Update();

			queued.clear();
			//end of event que

			//m_Graphics->pSpriteBatch->Begin();
			//m_Graphics->pSpriteFont->DrawString(m_Graphics->pSpriteBatch.get(), i->fpsString.c_str(), DirectX::XMFLOAT2(0, 20));
			//m_Graphics->pSpriteFont->DrawString(m_Graphics->pSpriteBatch.get(), wss.str().c_str(), DirectX::XMFLOAT2(0, 40));
			//m_Graphics->pSpriteBatch->End();

			m_Graphics->EndScene();

			/*	if (restart)
				{
					if (SetupApplication())
						restart = false;
					else
						return false;
				}*/

		}
	}
	catch (std::exception erro)
	{
		MessageBoxA(this->hWnd, erro.what(), "Exception!", MF_POPUP);
		
	}
}

DirectX::XMFLOAT3 SetRandomSpawn()
{
	return { (float)(rand() % 1200 + (-800)) , 50, (float)(rand() % 1200 + (-800)) };
}

void app::AddEntitiesToScene(Scenes* s, Gui* gui)
{

	s->AddEntityToScene(L"Scene1", new Entity(m_Graphics, L"Data\\map\\falling.obj", 1), { 0,0,0 }, { 0,0,0 });
	
	ItemStruct iSAK =
	{
	1,
	true,
	1,
	1,
	"AK47",
	L"Data\\Guns\\AK47\\AK471.obj",
	L"Data\\FpsArms\\FpsArmsAnimated.fbx",
	L"",
	ItemStatus::Ground
	};

	GunStats gsAK;
	gsAK.vRecoil = 20.f;
	gsAK.hRecoil = 12.f;
	gsAK.spread = 0.f;
	gsAK.fireRate = 0.125f;
	gsAK.recoverRate = .0925f;
	gsAK.damage = 0.f;
	gsAK.bulletSpeed = 0.f;
	gsAK.hipPenalty = 1.6f;
	gsAK.automatic = true;
	gsAK._shotSound = L"Data\\Sounds\\Gun_AK47_Single_Shot.wav";

	s->AddEntityToScene(L"Scene1", new gun(m_Graphics, iSAK, gsAK), { 0,20,0 }, { 0,0,0 });

	ItemStruct iSDeagle =
	{
	2,
	2,
	1,
	1,
	"Desert Eagle",
	L"Data\\Guns\\Deagle.obj",
	L"Data\\Guns\\Deagle.obj",
	L"",
	ItemStatus::Ground
	};
	GunStats gsDeagle
	{
	false,
	42.f,
	10.f,
	0.f,
	0.25f,
	0.75f,
	0.f,
	0.f,
	1.2f,
	L"Data\\Sounds\\Desert Eagle_shot.wav",
	};

	s->AddEntityToScene(L"Scene1", new gun(m_Graphics, iSDeagle, gsDeagle), { 20,20,10 }, { 0,0,0 });

	s->AddEntityToScene(L"Scene1", new Player(m_Graphics, i, L""), { 20,20,10 }, { 0,0,0 });

//#ifdef _DEBUG
//	Zombie z(m_Graphics);
//
//	for (int i = 0; i < 1; i++)
//	{
//		//s->AddEntityToScene(L"Scene1", new Zombie(z));
//	}
//#else
//	MovingAimBox m(m_Graphics);
//	Zombie z(m_Graphics);
//	for (int i = 0; i < Boxes; i++)
//	{
//		//s->AddEntityAiToScene(L"Scene1", new MovingAimBox(m));
//		//s->AddEntityAiToScene(L"Scene1", new Zombie(z));
//	}
//	Boxes *= 2;
//#endif // DEBUG
}

void app::StartupGui(Gui* gui)
{
	RECT r;

	GetWindowRect(m_Window, &r);
	r.right = r.right - r.left;
	r.bottom = r.bottom - r.top;

	r.top = r.bottom * .5;
	r.left = -r.right * .5;

	r.bottom = -r.top;
	r.right = -r.left;

	float middleX = r.left * .5f;
	float middleY = r.bottom * .5f;

	float sizeMult = .5f;


	float stBWit = 424 * sizeMult;
	float stBHei = 113 * sizeMult;
	float offset = 50;
	std::function<void(void*)> startfunc = [](void* s) { ((Scenes*)s)->SetActiveScene(L"Scene1");  };
	gui->AddButton(startfunc, s, L"Main", L"Data\\Menu\\Main\\Start.png", L"Data\\Menu\\Main\\Start_Selected.png",
		{ -stBWit, -stBHei + offset }, { stBWit, stBHei + offset});


	float setBWit = 408 * sizeMult;
	float setBHei = 67 * sizeMult;
	
	std::function<void(void*)> setfunc = [](void* g) { /*((Gui*)g)->ActivateMenu(L"Main")*/return; };
	gui->AddButton(setfunc, gui, L"Main", L"Data\\Menu\\Main\\Settings_Selected.png" /*L"Data\\Menu\\Main\\settings.png"*/, L"Data\\Menu\\Main\\Settings_Selected.png",
		{ -setBWit, -setBHei - stBHei}, { setBWit, setBHei - stBHei });

	float qBWit = 211 * sizeMult;
	float qBHei = 69 * sizeMult;
	offset = -((-setBHei - stBHei) - 50);
	std::function<void(void*)> quitfunc = [](void* g) { SendMessage(((window*)g)->getHWND(), WM_CLOSE, NULL, NULL); };
	gui->AddButton(quitfunc, (window*)this, L"Main", L"Data\\Menu\\Main\\quit.png", L"Data\\Menu\\Main\\quit_Selected.png",
		{ -qBWit, -qBHei - offset }, { qBWit, qBHei - offset });


	//std::function<void(void*)> refunc = [](void* g) { ((app*)g)->restart = true; };
	//gui->AddButton(refunc, (window*)this, L"Pause", L"Data\\Menu\\Main\\Restart.png", L"Data\\Menu\\Main\\Restart_Selected.png",
	//	{ -stBWit, -stBHei + 50 }, { stBWit, stBHei + 50 });
	std::function<void(void*)> refunc = [](void* s) {
		((Scenes*)s)->ResetActiveScene();
		((Scenes*)s)->RemoveActiveKeyGui();
	};
	gui->AddButton(refunc, (Scenes*)s, L"Pause", L"Data\\Menu\\Main\\Restart.png", L"Data\\Menu\\Main\\Restart_Selected.png",
		{ -stBWit, -stBHei + 50 }, { stBWit, stBHei + 50 });
	gui->AddExistingButton(L"Pause", L"Settings_Selected.png"/* L"settings.png"*/);
	gui->AddExistingButton(L"Pause", L"quit.png");


	gui->SetMainMenu(L"Main");

}


