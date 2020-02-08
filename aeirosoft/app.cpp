#include "app.h"
#include "Inc/DirectX/SimpleMath.h"
#include "Player.h"
#include "MovingAimBox.h"

#include <fstream>
#include <sstream>
app::app(HINSTANCE hinst, const std::wstring windowName, POINT p) :
	window(hinst, windowName, p)
{
	hInst = hinst;
	m_Window = getHWND();
	m_Timer.Start();
	m_ClickTimer.Start();
	m_KeyTimer.Start();
}

app::~app()
{
	DestroyWindow(getHWND());
}

//template <typename ...args>
//std::function<void(Gui*, args...)> function;

//void setRandomFloat3(float& r, float& g, float& b, int d, float x)
//{
//	srand(static_cast <unsigned> (time(0)));
//
//	r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / d)* x;
//	g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / d)* x;
//	b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / d)* x;
//}




int app::begin()
{
	graphics m_Graphics(m_Window, false, false);
	if (!m_Graphics.Initialize())
		return false;

	std::vector<Events*> events;
	input i(&m_Graphics, static_cast<window*>(this), &m_Graphics.m_Camera, &m_Timer);

	//Gui gui(&m_Graphics, &i);
	//StartupGui(gui);

	//Player p(&m_Graphics, &i, L"Data\\Guns\\DesertEagle.obj", { 2.5f, -2.50f, 3.f }, { 0.f, 4.5f, 0.f },
	//					L"");

	//Player p(&m_Graphics, &i, L"Data\\Guns\\AK47\\AK47.obj", { 3.93f, -4.05f, 12.8215f }, { 0.f, 4.7f, 0.0017f },
	//	L"");

		//Player p(&m_Graphics, &i, L"Data\\Guns\\AK47\\AK47.obj", { .063f, -4.12517f, 12.8215f }, { 0.00518461f, 4.70958f, 0.0017f },
		//L"");


	Player p(&m_Graphics, &i, L"Data\\Guns\\AK47\\AK47.obj",{ { 3.93f, -4.05f, 12.8215f }, { 0.f, 4.7f, 0.0017f } },
															{ { .0115558f, -2.87861f, 5.8215f }, { 0.f, 4.71489f, -.0250774f } },
															L"");

	Collision C;
	Scenes s(&m_Graphics, &C);
	CreateScenes(s);
	s.AddPlayerToScene(L"Scene1", &p);

	MovingAimBox M(&m_Graphics);
	MovingAimBox M1(&m_Graphics);
	MovingAimBox M2(&m_Graphics);
	C.AddCollidable(&M);
	C.AddCollidable(&M1);
	C.AddCollidable(&M2);
	events.push_back(&i);
	events.push_back(&s);
	events.push_back(&C);
	events.push_back(&M);
	events.push_back(&M1);
	events.push_back(&M2);
	//LoadModel(L"Data\\Objects\\nanosuit\\nanosuit.obj", m_Graphics.GetDevice(), m_Graphics.GetDeviceContext());
	texture t(m_Graphics.GetDevice().Get(), L"Data\\Textures\\RustyPaint.dds", aiTextureType::aiTextureType_DIFFUSE);

	for (const auto& E : events)
		if (!E->Initialize())
			return false;

	while (true) 
	{		
		if (const auto optional = processMessages())
				return *optional;

		m_Graphics.BeginScene(0.f, 0.f, 0.f, 1.f);
		m_Graphics.CreateFloor({ -200,-200 }, { 200, 200 }, t);
		// start of event que
		std::vector<Events*> queued;

		for (const auto& E : events)
			if (const auto optional = E->Queue())
				queued.push_back(*optional);

		for (const auto& E : queued)
			E->Update();
		
		queued.clear();
		//end of event que

		std::wstringstream wss;
		std::wstringstream wsshots;
		std::wstringstream wssAcc;
		int hits = M.GetHits() + M1.GetHits() + M2.GetHits();
		int shots = p.GetShots();
		float Accuracy = (float)((float)hits / (shots > 0 ? (float)shots : (float)1));
		wss << L"Hits: " << hits;
		wsshots << L"Shots: " << shots;
		wssAcc << L"Accuracy: " << Accuracy;
		m_Graphics.pSpriteBatch->Begin();
		m_Graphics.pSpriteFont->DrawString(m_Graphics.pSpriteBatch.get(), wss.str().c_str(), DirectX::XMFLOAT2(0, 20));
		m_Graphics.pSpriteFont->DrawString(m_Graphics.pSpriteBatch.get(), wsshots.str().c_str(), DirectX::XMFLOAT2(0, 0));
		m_Graphics.pSpriteFont->DrawString(m_Graphics.pSpriteBatch.get(), wssAcc.str().c_str(), DirectX::XMFLOAT2(0, 40));
		m_Graphics.pSpriteBatch->End();



		m_Graphics.EndScene();

	}
}



void app::CreateScenes(Scenes& s)
{
	s.CreateScene(L"Scene1");
	//s.AddModel(L"Data\\Objects\\Target\\target.obj");
	//s.AddModelToScene(L"Scene1", L"target.obj", { 0,10,10 }, { 0,0,0 });
	s.SetActiveScene(L"Scene1");
}

void app::StartupGui(Gui& gui)
{
	RECT r;

	GetWindowRect(m_Window, &r);
	r.right = r.right - r.left;
	r.bottom = r.bottom - r.top;

	r.top = r.bottom * .5;
	r.left = -r.right * .5;

	r.bottom = -r.top;
	r.right = -r.left;

	gui.AddImage(L"Main", L"Data\\Menu\\Backgrounds\\4.png", { (float)r.left, (float)r.bottom }, { (float)r.right, (float)r.top });

	gui.AddImage(L"GirlMenu", L"Data\\Menu\\Backgrounds\\4.png", { (float)r.left, (float)r.bottom }, { (float)r.right, (float)r.top });
	gui.AddImage(L"GuyMenu", L"Data\\Menu\\Backgrounds\\4.png", { (float)r.left, (float)r.bottom }, { (float)r.right, (float)r.top });
	gui.AddImage(L"Main", L"Data\\Menu\\Menu\\Elites.png", { -300.f, 200.f }, { 300.f, 300.f });

	std::function<void(Gui*)> funcGuy = [](Gui* g) { g->ActivateMenu(L"GuyMenu"); };

	gui.AddButton(funcGuy, L"Main", L"Data\\Menu\\Characters\\Guy.png", L"Data\\Menu\\Characters\\Male Glow.png",
		{ 200.f, -350.f }, { 450.f, 200.f });

	std::function<void(Gui*)> funcGirl = [](Gui* g) { g->ActivateMenu(L"GirlMenu"); };

	gui.AddButton(funcGirl, L"Main", L"Data\\Menu\\Characters\\Girl.png", L"Data\\Menu\\Characters\\Female Glow.png",
		{ -470.f, -325.f }, { -110.f, 25.f });


	gui.AddButton(funcGirl, L"GirlMenu", L"Data\\Menu\\Menu\\World.png", L"Data\\Menu\\Menu\\World Glow.png",
		{ -80.f, 0.f }, { 188.f, 68.f });

	gui.AddButton(funcGirl, L"GirlMenu", L"Data\\Menu\\Menu\\Invite.png", L"Data\\Menu\\Menu\\Invite Glow.png",
		{ -80.f, -86.f }, { 188.f, -16.f });

	gui.AddButton(funcGirl, L"GirlMenu", L"Data\\Menu\\Menu\\Stash.png", L"Data\\Menu\\Menu\\Stash Glow.png",
		{ -80.f, -170.f }, { 188.f, -102.f });

	gui.AddImage(L"GirlMenu", L"Data\\Menu\\Characters\\Girl.png",
		{ 200.f, -325.f }, { 450.f, 25.f });

	gui.AddButton(funcGuy, L"GuyMenu", L"Data\\Menu\\Menu\\World.png", L"Data\\Menu\\Menu\\World Glow.png",
		{ -80.f, 0.f }, { 188.f, 68.f });

	gui.AddButton(funcGuy, L"GuyMenu", L"Data\\Menu\\Menu\\Invite.png", L"Data\\Menu\\Menu\\Invite Glow.png",
		{ -80.f, -86.f }, { 188.f, -16.f });

	gui.AddButton(funcGuy, L"GuyMenu", L"Data\\Menu\\Menu\\Stash.png", L"Data\\Menu\\Menu\\Stash Glow.png",
		{ -80.f, -170.f }, { 188.f, -102.f });

	gui.AddImage(L"GuyMenu", L"Data\\Menu\\Characters\\Guy.png",
		{ 200.f, -350.f }, { 450.f, 200.f });


	gui.SetMainMenu(L"Main");

}

//void app::CheckBounds(model& m)
//{
//
//	for (DirectX::BoundingBox i : m.vbBox)
//	{
//		
//		DirectX::BoundingBox box;
//		float f = 0.f;
//		if (i.Intersects(clickOrigin, clickDestination, f))
//		{
//			intersecting = helper::strings::wsinttows(L"Intersecting: TRUE: Distance From: ", f);
//			//collision = true;
//			//m.Select();
//			break;
//		}
//		else
//		{
//			//collision = false;
//			
//		}
//	}
//}

/*
	//void app::DrawClickRays()
	//{
	//
	//	m_Graphics.m_batch->Begin();
	//
	//	for (int i = 0; i < vOrig.size() && i < vDest.size(); i++)
	//	{
	//		DirectX::VertexPositionColor v1(vOrig[i], DirectX::Colors::Red);
	//		DirectX::VertexPositionColor v2(vDest[i], DirectX::Colors::Red);
	//		m_Graphics.m_batch->DrawLine(v1, v2);
	//	}
	//	m_Graphics.m_batch->End();
	//
	//}


	//
	//DirectX::XMVECTOR app::GetMouseWorldDestination(bool storeOriginAndDestination)
	//{
	//	POINT p;
	//	GetCursorPos(&p);
	//	RECT r = helper::window::GetRect(m_Window);
	//	//float x, y;
	//
	//	ScreenToClient(m_Window, &p);
	//
	//
	//
	//	DirectX::XMMATRIX projection = m_Graphics.GetProjectionMatrix();
	//	DirectX::XMMATRIX view = m_Graphics.GetViewMatrix();
	//	DirectX::XMMATRIX world = m_Graphics.GetWorldMatrix();
	//
	//	//
	//	clickOrigin = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 0.f, 0.f),
	//		0, 0,
	//		r.right, r.bottom,
	//		0, 1.f,
	//		projection,
	//		view,
	//		world);
	//
	//	DirectX::XMVECTOR dest = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 1.f, 0.f),
	//		0, 0,
	//		r.right, r.bottom,
	//		0, 1.f,
	//		projection,
	//		view,
	//		world);
	//
	//
	//	DirectX::XMVECTOR clickDestination = DirectX::XMVectorSubtract(dest, clickOrigin);
	//
	//	if (storeOriginAndDestination)
	//	{
	//		vOrig.push_back(clickOrigin);
	//		vDest.push_back(clickDestination);
	//		
	//
	//	}
	//
	//	return clickDestination;
	//}
}
void app::CreateClickRay()
{
	
	clickDestination = GetMouseWorldDestination(true);
	DirectX::XMFLOAT3 fl3;
	
	clickDestination = DirectX::XMVector3Normalize(clickDestination);

	DirectX::XMStoreFloat3(&fl3, clickDestination);

	float fx = fl3.x;




}
*/

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
