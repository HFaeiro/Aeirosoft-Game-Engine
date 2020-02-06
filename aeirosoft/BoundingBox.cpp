#include "BoundingBox.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::GetBounds(ID3D11Device* pDevice,std::vector<Vertex>& v)
{

    
    // ....

if (v.size() > 1) {
    BoxMin.position.x = (*std::min_element(v.begin(), v.end(), &BoundingBox::_compare_min_x)).position.x;
    BoxMin.position.y = (*std::min_element(v.begin(), v.end(), &BoundingBox::_compare_min_y)).position.y;
    BoxMin.position.z = (*std::min_element(v.begin(), v.end(), &BoundingBox::_compare_min_z)).position.z;

    BoxMax.position.x = (*std::min_element(v.begin(), v.end(), &BoundingBox::_compare_max_x)).position.x;
    BoxMax.position.y = (*std::min_element(v.begin(), v.end(), &BoundingBox::_compare_max_y)).position.y;
    BoxMax.position.z = (*std::min_element(v.begin(), v.end(), &BoundingBox::_compare_max_z)).position.z;

}



//topLeftfar.position.x = BoxMin.position.x;
//topLeftfar.position.y = BoxMax.position.y;
//topLeftfar.position.y = BoxMax.position.z;
//
//
//topLeftclose.position.x = BoxMin.position.x;
//topLeftclose.position.y = BoxMax.position.y;
//topLeftclose.position.y = BoxMin.position.z;
//
//trf.position.x = BoxMax.position.x;
//trf.position.y = BoxMax.position.y;
//trf.position.y = BoxMin.position.z;
//
//
//bottomLeftclose.position.x = BoxMin.position.x;
//bottomLeftclose.position.y = BoxMin.position.y;
//bottomLeftclose.position.y = BoxMax.position.z;
//
//
//bottomRightfar.position.x = BoxMax.position.x;
//bottomRightfar.position.y = BoxMin.position.y;
//bottomRightfar.position.y = BoxMin.position.z;
//
//bottomRightclose.position.x = BoxMax.position.x;
//bottomRightclose.position.y = BoxMin.position.y;
//bottomRightclose.position.y = BoxMax.position.z;
//
//std::vector<Vertex> vertices;
//vertices.push_back(topLeftclose); 
//vertices.push_back(topLeftfar);
//vertices.push_back(trf);
//vertices.push_back(BoxMax);
//
//vertices.push_back(bottomRightclose);
//vertices.push_back(bottomRightfar);
//vertices.push_back(BoxMin);
//vertices.push_back(bottomLeftclose);

//vBuff.Init(pDevice, vertices.data(), vertices.size());
//
//std::vector<DWORD> indices;
//
//
//DWORD ind[] =
//{
//    0,1,2,3,0,
//    7,6,5,4,7 
//
//
//};
//iBuff.Init(pDevice, indices.data(), indices.size());

}

void BoundingBox::Draw(ID3D11DeviceContext* pContext, DirectX::XMMATRIX world)
{

    //pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //UINT offset = 0;
    //pContext->IASetVertexBuffers(0, 1, vBuff.GetAddressOf(), vBuff.GetStridePtr(), &offset);
    //pContext->IASetIndexBuffer(iBuff.Get(), DXGI_FORMAT_R32_UINT, 0);
    //pContext->DrawIndexed(this->iBuff.Size(), 0, 0);


}





