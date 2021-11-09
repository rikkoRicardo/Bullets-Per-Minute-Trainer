#pragma once
#include "stdafx.h"

namespace mem {
	void PatchEx(HANDLE hProc, BYTE* dst, BYTE* src, unsigned int size);

	void NopEx(HANDLE hProc, BYTE* dst, unsigned int size);
}