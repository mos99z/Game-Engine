// CSharpWarpper.h

#pragma once
#using "CSharpTestClass.dll"

using namespace CSharpTestClass;

namespace CSharpWarpper {

	__declspec(dllexport) int GetWrapper()
	{
		Test^ pInstance = gcnew Test();
		return pInstance->Get();
	}
}
