#include "Framework\RZCommon.h"
#include "Entity\FbxImporter\RZFbxImporter.h"
#include "Framework\SystemClass.h"

using namespace RZ;


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pScmdlne,int iCmdshc)
{
	char lFilename[]="D:\\\workspace\\RZ\\RZ\\test2.fbx";
	char szFileNameASCII[]="D:\\\workspace\\RZ\\RZ\\Resource\\model\\test.rza";
	RZFbxImporter *importer=new RZFbxImporter();
	importer->init();
	importer->LoadMeshFromFile(lFilename);
	RZIntermediateMesh* pRZIntermediateMesh=importer->GetMesh();
	pRZIntermediateMesh->SaveAsRZMesh(model);
	pRZIntermediateMesh->SaveAsRZMeshASCII(szFileNameASCII);
	SystemClass* System;
	bool result; 

	// Create the system object. 
	System = new SystemClass;  
	if(!System)  
	{
		return 0;
	}

	// Initialize and run the system object.  
	result = System->Initialize();  
	if(result)
	{
		System->Run();	
	}

	// Shutdown and release the system object. 
	System->Shutdown();
	delete System;
	System = 0; 

	return 0;
}