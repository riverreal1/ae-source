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

/*void ErrorMSG(LPCSTR msg)
{
	MessageBox(0, TEXT(msg), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
}*/
#endif

