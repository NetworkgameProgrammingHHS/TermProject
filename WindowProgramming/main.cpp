#include "pch.h"
#include "CFramework.h"


int main()
{
	CFramework* Framework = new CFramework();
	
	Framework->Process();

	delete Framework;

	return 0;
}