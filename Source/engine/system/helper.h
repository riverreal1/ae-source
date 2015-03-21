#ifndef _HELPER_H_
#define _HELPER_H_

template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#endif

