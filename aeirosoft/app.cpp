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
	gui = new Gui(m_Graphics, i, (window*)this);
	StartupGui(gui);

	s->CreateScene(L"Scene1", gui, false, true);
	s->CreateScene(L"MainMenu", gui, true);




	auto t1 = std::thread(&app::CreateScenes,this,  std::ref(s), std::ref(gui));
	//CreateScenes(s, gui);
	//auto t2 = std::thread(std::bind(&Scenes::AddEntityToScene, s, L"Scene1", new Player(m_Graphics, i, L"Data\\FpsArms\\FpsArmsAnimated.fbx",L"Data\\Sounds\\Gun_AK47_Single_Shot.wav",L"")));

	s->AddOnKeyEventToScene(L"Scene1", L"Pause", gui, false, DIK_ESCAPE);

	events.push_back(i);
	events.push_back(s);
#ifdef _DEBUG
	Zombie z(m_Graphics);

	s->AddEntityAiToScene(L"Scene1", new Zombie(z));
	s->AddEntityAiToScene(L"Scene1", new Zombie(z));
#else
	MovingAimBox m(m_Graphics);
	Zombie z(m_Graphics);
	for (int i = 0; i < Boxes; i++)
	{
		s->AddEntityAiToScene(L"Scene1", new MovingAimBox(m));
		s->AddEntityAiToScene(L"Scene1", new Zombie(z));
	}
	Boxes *= 2;
#endif // DEBUG
	s->AddEntityToScene(L"Scene1", new Player(m_Graphics, i, L"Data\\FpsArms\\FpsArmsAnimated.fbx",
		L"Data\\Sounds\\Gun_AK47_Single_Shot.wav",
		L""));
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

		m_Graphics->pSpriteBatch->Begin();
		m_Graphics->pSpriteFont->DrawString(m_Graphics->pSpriteBatch.get(), i->fpsString.c_str(), DirectX::XMFLOAT2(0, 20));
		m_Graphics->pSpriteFont->DrawString(m_Graphics->pSpriteBatch.get(), wss.str().c_str(), DirectX::XMFLOAT2(0, 40));
		m_Graphics->pSpriteBatch->End();

		m_Graphics->EndScene();

		if (restart)
		{
			if (SetupApplication())
				restart = false;
			else
				return false;
		}

	}
}

DirectX::XMFLOAT3 SetRandomSpawn()
{
	return { (float)(rand() % 1200 + (-800)) , 50, (float)(rand() % 1200 + (-800)) };
}

void app::CreateScenes(Scenes* s, Gui* gui)
{

	s->CreateEntityObject(L"Data\\map\\falling.obj");
	s->AddObjectToScene(L"Scene1", L"falling.obj", { 0,0,0 }, { 0,0,0 });













	//s->CreateScene(L"Scene1", gui, false, true);
	//s->CreateScene(L"MainMenu", gui, true);
	//s->CreateEntityObject(L"Data\\map\\falling.obj");
	//s->AddObjectToScene(L"Scene1", L"falling.obj", { 0,0,0 }, { 0,0,0 });
	//s->CreateEntityObject(L"Data\\FpsArms\\AnimatedZombie.fbx");
	//s->AddObjectToScene(L"Scene1", L"AnimatedZombie.fbx", { 20,0,20 }, { 0,0,0 });

	//s->CreateEntityObject(L"Data\\FpsArms\\TestAnimMonster.fbx");
	//s->AddObjectToScene(L"Scene1", L"TestAnimMonster.fbx", { -200,0,-200 }, { 0,0,0 });
	//s->CreateEntityObject(L"Data\\Objects\\Wall\\Wall.obj");
	//s->CreateEntityObject(L"Data\\Objects\\Bully\\untitled.obj");

	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0});
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });
	////s->AddObjectToScene(L"Scene1", L"floor.obj", SetRandomSpawn(), { DirectX::XM_PI * .5f,0,0 });

	//s->AddObjectToScene(L"Scene1", L"untitled.obj", { 0,0,0 }, { 0,0,0 });
	//s->AddObjectToScene(L"Scene1", L"Wall.obj", { -250,0,200 }, { 0,0,0 });
	//s->AddObjectToScene(L"Scene1", L"Wall.obj", { 250,0,200 }, { 0,0,0 });
	//s->AddObjectToScene(L"Scene1", L"Wall.obj", { 0,0,-200 }, { 0,DirectX::XM_PI,0 });
	//s->AddObjectToScene(L"Scene1", L"Wall.obj", { 200,0,0 }, { 0,DirectX::XM_PI*.5f,0 });
	//s->AddObjectToScene(L"Scene1", L"Wall.obj", { -200,0,0 }, { 0,DirectX::XM_PI * 1.5f,0 });
	
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
	std::function<void(void*)> refunc = [](void* s) { ((Scenes*)s)->ResetActiveEntity(); ((Scenes*)s)->RemoveActiveKeyGui(); };
	gui->AddButton(refunc, (Scenes*)s, L"Pause", L"Data\\Menu\\Main\\Restart.png", L"Data\\Menu\\Main\\Restart_Selected.png",
		{ -stBWit, -stBHei + 50 }, { stBWit, stBHei + 50 });
	gui->AddExistingButton(L"Pause", L"Settings_Selected.png"/* L"settings.png"*/);
	gui->AddExistingButton(L"Pause", L"quit.png");


	gui->SetMainMenu(L"Main");

}

//bool LoadModel(const std::wstring& filename, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//{
//
//	FILE* file;
//	_wfopen_s(&file, filename.c_str(), L"r");
//	if (file == NULL)
//		return false;
//	std::wstring line;
//	const std::wstring directory = helper::strings::GetDirectoryFromPath(filename);
//	float x, y, z;
//	int i, t, n;
//	std::wstring mtlfile = L"";
//
//	struct wavefront
//	{
//		std::wstring name;
//		std::wstring material;
//		std::vector<DirectX::XMFLOAT3> vertices;
//		std::vector<DirectX::XMFLOAT2> textureCoords;
//		std::vector<DirectX::XMFLOAT3> normals;
//		std::vector<DWORD> indecies;
//		std::vector<DWORD> tindecies;
//		std::vector<DWORD> nindecies;
//		wavefront(std::wstring n) { name = n; }
//	};
//	std::vector<wavefront> wf;
//	
//
//	int wfsize;
//	line.resize(100);
//	while(!feof(file))
//	//while (fgetws(line.data(), line.size(), file))
//	{
//		std::wstring substr;
//
//		
//		wchar_t str[100];
//		int size = fwscanf_s(file, L"%s", str, 100);
//		if(size)
//			substr.resize(size);
//
//
//		wfsize = wf.size() - 1;
//		if (str[0] == L'#')
//		{
//			fgetws(line.data(), line.size(), file);
//			continue;
//		}
//			
//		if (str[0] == L's')
//		{
//			fgetws(line.data(), line.size(), file);
//			continue;
//		}
//		if (str[0] == L'm')
//		{
//			fwscanf_s(file, L"%s\n", line.data(), 100);
//			mtlfile = line;
//			continue;
//		}
//		if (str[0] == L'u')
//		{
//			fwscanf_s(file, L"%s\n", line.data(), 100);
//			wf[wfsize].material = line;
//			continue;
//		}
//		if(str[0] == L'v')
//		{
//			fwscanf_s(file, L"%f %f %f\n", &x, &y, &z);
//			if(str[1] == L'n')
//				wf[wfsize].normals.push_back({ x, y, z });
//			else if(str[1] == L't')
//				wf[wfsize].textureCoords.push_back({ x, y });
//			else
//				wf[wfsize].vertices.push_back({ x, y, z });
//			continue;
//		}
//		
//		if (str[0] == L'f')
//		{
//			for (int j = 0; j < 2; j++) {
//				fwscanf_s(file, L"%d/%d/%d ", &i, &t, &n);
//				i -= 1;
//				t -= 1;
//				n -= 1;
//				wf[wfsize].indecies.push_back(i);
//				wf[wfsize].tindecies.push_back(t);
//				wf[wfsize].nindecies.push_back(n);
//			}
//			fwscanf_s(file, L"%d/%d/%d\n", &i, &t, &n);
//			i -= 1;
//			t -= 1;
//			n -= 1;
//			wf[wfsize].indecies.push_back(i);
//			wf[wfsize].tindecies.push_back(t);
//			wf[wfsize].nindecies.push_back(n);
//			continue;
//		}
//		if (str[0] == L'o')
//		{
//			fwscanf_s(file, L"%s\n", line.data(), 100);
//			wf.push_back(line);
//			continue;
//		}
//		
//	}
//	fclose(file);
//	std::vector<Vertex> v;
//	std::vector<texture> vTexture;
//	std::vector<Mesh> meshes;
//	vTexture.push_back(texture(pDevice, colors::UnhandledTexturecolor, aiTextureType::aiTextureType_DIFFUSE));
//	for (auto& w : wf)
//	{
//
//		
//
//		for (auto& index : w.tindecies)
//		{
//			if (index >= w.vertices.size() || index >= w.textureCoords.size())
//			{
//				int poo = 0;
//			}
//			v.emplace_back(w.vertices[index], w.textureCoords[index]);
//			
//		}
//		meshes.push_back(Mesh(pDevice, pContext, v, w.indecies, vTexture));
//		v.clear();
//	}
//	int p = 8;
//}
