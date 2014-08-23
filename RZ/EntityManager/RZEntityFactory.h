#ifndef _RZENTITYFACTORY_H_
#define _RZENTITYFACTORY_H_

class RZEntityFactory
{
public:
	RZEntityFactory();
	RZEntityFactory(const RZEntityFactory& other);
	virtual ~RZEntityFactory();
	bool Initialize(ID3D11Device*);
	bool Render(ID3D11DeviceContext* pd3dDeviceContext);

private:
	vector<RZEntity> RZEntityPool;
	

};

#endif