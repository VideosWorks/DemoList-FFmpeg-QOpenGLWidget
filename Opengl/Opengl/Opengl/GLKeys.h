#ifndef _GLKEYS_H
#define _GLKEYS_H

class GLKeys
{
public:
	GLKeys();
	//清空所有按键信息
	void Clear();
	//判断某个键是否被按下
	bool IsPressed(unsigned int nKey);
	//设置某个键被按下
	void SetPressed(unsigned int nKey);
	//设置某个键释放
	void SetReleased(unsigned int nKey);

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_bKeyDown[MAX_KEYS];
};
#endif