#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../sources/Device/Core/CCore.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace NativeTests
{		
	TEST_CLASS(CCore)
	{
	public:
		
		TEST_METHOD(RangeTest)
		{
			CCore *core = new CCore();
			Assert::AreEqual(1.1, 1.1, 0.01, L"Basic Test Test", LINE_INFO());
		}

	};
}
