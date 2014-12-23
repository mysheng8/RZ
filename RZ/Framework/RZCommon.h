#pragma once
#ifndef _RZCOMMON_H_  
#define _RZCOMMON_H_

#include <D3D11.h>
#include <d3dx11.h>
#include <DxErr.h>
#include <DXGI.h>
#include <D3DCompiler.h>
#include <windows.h>
#include <xnamath.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "RZResource.h"
#define RZRELEASE(val) {if(val){ val->Release(); val = 0;};}

#endif