#include "app.h"
#include <time.h>
#include <DirectXCollision.h>

app::app(HINSTANCE hinst, const std::wstring windowName, POINT p) :
	window(hinst, windowName, p)
{
	hInst = hinst;
	
	timer.Start();

}

app::~app()
{

}
void setRandomFloat3(float&r, float&g, float&b, int d, float x)
{
	srand(static_cast <unsigned> (time(0)));
	
	r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/d) * x;
	g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/d) * x;
	b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/d) * x;
}
int app::begin()
{
	RECT rect = g.GetRect(getHWND());

	if(!pInput.initialize(hInst, getHWND(), rect.right, rect.bottom))
		return false;
	float r, gr, b;
	setRandomFloat3(r, gr, b, 255, .005f);
	float xPos = 0.f, yPos = 0.f, zPos = -5.f;
	float zRot = -5.f;
	float mX, mY;
	POINT p;


	while (true) {
		float elapsedTime = timer.GetMillisecondsElapsed();
		timer.restart();

		if (const auto optional = processMessages())
			return *optional;

		//place per frame code here
		if (!pInput.frame())
			return false;
		pInput.getMouseLocation(mX, mY);
		if(mX)
			mX = mX * 0.001f;
		if(mY)
			mY = mY * 0.001f;

		const float moveSpeed = .01f * elapsedTime;

	
		if (pInput.isLeftClick())
		{
			GetCursorPos(&p);
			//ScreenToClient(getHWND(), &p);
			DirectX::XMVECTOR orig = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 0.f, 0.f),
				0, 0,
				getWidth(), getHeight(),
				0, 1,
				g.getProjectionMatrix(),
				g.getViewMatrix(),
				g.getWorldMatrix());

			DirectX::XMVECTOR dest = DirectX::XMVector3Unproject(DirectX::XMVectorSet(p.x, p.y, 1.f, 0.f),
				0, 0,
				getWidth(), getHeight(),
				0, 1,
				g.getProjectionMatrix(),
				g.getViewMatrix(),
				g.getWorldMatrix());

			DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(dest, orig);

			direction = DirectX::XMVector3Normalize(direction);
			
			//std::vector<std::vector<Vertex>> modelBounds = g.pModel.GetBounds();

			/*for (int i = 0; i < modelBounds.size(); i++)
			{
				for (int j = 0; j < modelBounds[i].size(); j++)
				{
					float f = .0f;
					if (DirectX::TriangleTests::Intersects(orig, direction, DirectX::XMLoadFloat3(&modelBounds[i][j].position),
						DirectX::XMLoadFloat3(&modelBounds[i][j].position), DirectX::XMLoadFloat3(&modelBounds[i][j].position), f))
					{
						
					}
				}
			}*/
			//DirectX::XMFLOAT3 tmpv;
			//g.pModel.bBox.GetCorners(&tmpv);
			//DirectX::PlaneIntersectionType pType = g.pModel.bBox.Intersects(direction);

			//if (pType == DirectX::PlaneIntersectionType::INTERSECTING)
			//{
			//	std::cout << "we did it!";
			//}

			
		}


		



		//g.SetCamera(xPos, yPos, zPos);
		g.SetCameraRotation(mY, mX, zRot);
		g.BeginScene(r, gr, b, 1.0f);


		g.endFrame();

	}
}
