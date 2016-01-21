#pragma once
#ifndef _RZLINE_H_
#define _RZLINE_H_

#include "..\Framework\RZCommon.h"

namespace RZ
{
	class RZLine
	{
	public:
		XMFLOAT3 position;
		XMFLOAT3 tangent;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};
};
#endif