#pragma once
#include "framework.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

    void GetBounds(ID3D11Device* pDevice, std::vector<Vertex>& v);
    void Draw(ID3D11DeviceContext*, DirectX::XMMATRIX);


    static bool _compare_min_x(Vertex const& p1, Vertex const& p2) { return p1.position.x < p2.position.x; }
    static  bool _compare_min_y(Vertex const& p1, Vertex const& p2) { return p1.position.y < p2.position.y; }
    static bool _compare_min_z(Vertex const& p1, Vertex const& p2) { return p1.position.z < p2.position.z; }

    static bool _compare_max_x(Vertex const& p1, Vertex const& p2) { return p1.position.x > p2.position.x; }
    static  bool _compare_max_y(Vertex const& p1, Vertex const& p2) { return p1.position.y > p2.position.y; }
    static bool _compare_max_z(Vertex const& p1, Vertex const& p2) { return p1.position.z > p2.position.z; }
private:

    VertexBuffer<Vertex> vBuff;
    IndexBuffer iBuff;
    Vertex BoxMin, BoxMax;
   

};

