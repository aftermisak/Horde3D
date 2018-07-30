// Horde3D_using.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "app.h"
int main(int argc, char** argv)
{
	std::cout << "hello horde 3d!" << std::endl;
	Horde3DUsingApp app(argc, argv);
	return app.run();
}

