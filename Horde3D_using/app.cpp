// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
//
// Sample Application
// --------------------------------------
// Copyright (C) 2006-2016 Nicolas Schulz and Horde3D team
//
//
// This sample source file is not covered by the EPL as the rest of the SDK
// and may be used without any restrictions. However, the EPL's disclaimer of
// warranty and liability shall be in effect for this file.
//
// *************************************************************************************************

#include "app.h"
#include "Horde3D.h"
#include "Horde3DUtils.h"
#include "utMath.h"
#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;


Horde3DUsingApp::Horde3DUsingApp(int argc, char** argv) :
	SampleApplication(argc, argv, "Horde3D using Sample", H3DRenderDevice::OpenGL4)
{
	_x = 15; _y = 3; _z = 20;
	_rx = -10; _ry = 60;
}


bool Horde3DUsingApp::initResources()
{
	if (!SampleApplication::initResources())
		return false;

	// 1. Add resources

	//_peopleRes = h3dAddResource(H3DResTypes::Material, "horde3d_usings/people.material.xml", 0);

	_peopleRes = h3dAddResource(H3DResTypes::Material, "horde3d_usings/people1_origin.material.xml", 0);
	_peopleResOutline = h3dAddResource(H3DResTypes::Material, "horde3d_usings/people1_outline.material.xml", 0);
	

	//_outlinePipline = h3dAddResource(H3DResTypes::Pipeline, "horde3d_usings/outline.pipeline.xml",0);

	// 2. Load resources

	if (!h3dutLoadResourcesFromDisk(getResourcePath()))
	{
		h3dutDumpMessages();
		return false;
	}

	// 3. Add scene nodes

	// Add camera
	_cam = h3dAddCameraNode(H3DRootNode, "Camera", getPipelineRes());
	h3dSetNodeParamI( _cam, H3DCamera::OccCullingI, 1 );

	//_people = h3dAddNodes(H3DRootNode, _peopleRes);
	return true;
}


void Horde3DUsingApp::releaseResources()
{

	SampleApplication::releaseResources();
}


void Horde3DUsingApp::update()
{
	SampleApplication::update();

	if (!checkFlag(SampleApplication::FreezeMode))
	{
		
	}
}

void Horde3DUsingApp::render() {


	const float ww = (float)h3dGetNodeParamI(_cam, H3DCamera::ViewportWidthI) /
		(float)h3dGetNodeParamI(_cam, H3DCamera::ViewportHeightI);

	const float verts[] = {
		0.25f, 0.25f, 0, 1,
		0.25f, 0.75f, 0, 0,
		0.75f, 0.75f, 1, 0,
		0.75f, 0.25f, 1, 1
	};
	const float verts_bigger[] = {
		0.20f, 0.20f, 0, 1,
		0.20f, 0.80f, 0, 0,
		0.80f, 0.80f, 1, 0,
		0.80f, 0.20f, 1, 1
	};
	

	h3dShowOverlays(verts, 4, 1.f, 1.f, 1.f, 1.f, _peopleRes, 0);
	h3dShowOverlays(verts_bigger, 4, 1.f, 1.f, 1.f, 1.f, _peopleResOutline, 0);

	SampleApplication::render();
	
}