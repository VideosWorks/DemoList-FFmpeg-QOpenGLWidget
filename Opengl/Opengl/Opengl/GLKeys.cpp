#include "GLKeys.h"
#include <windows.h>

GLKeys::GLKeys()
{
	Clear();
}

void GLKeys::Clear()
{
	ZeroMemory(&m_bKeyDown, sizeof(m_bKeyDown));
}

bool GLKeys::IsPressed(unsigned int nKey)
{
	if (nKey < MAX_KEYS)
	{
		return (m_bKeyDown[nKey] == true);
	}
	return false;
}

void GLKeys::SetPressed(unsigned int nKey)
{
	if (nKey < MAX_KEYS)
	{
		m_bKeyDown[nKey] = true;
	}
}

void GLKeys::SetReleased(unsigned int nKey)
{
	if (nKey < MAX_KEYS)
	{
		m_bKeyDown[nKey] = false;
	}
}

