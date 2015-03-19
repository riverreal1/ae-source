#ifndef _IRENDERMANAGER_H_
#define _IRENDERMANAGER_H_

class IRenderManager
{
public:
	virtual bool VInit() = 0;
	virtual void VStartRendering() = 0;
	virtual void VEndRendering() = 0;
	virtual void VShutDown() = 0;
};

#endif