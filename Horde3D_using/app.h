#pragma once

#include "sampleapp.h"


class Horde3DUsingApp : public SampleApplication
{
public:
	Horde3DUsingApp(int argc, char** argv);

protected:
	bool initResources();
	void releaseResources();

	virtual void update() override;
	virtual void render() override;
private:
	H3DRes _peopleRes;
	H3DRes _peopleResOutline;


	H3DNode _people;
	H3DRes _outlinePipline;
};