#ifndef _GLKEYS_H
#define _GLKEYS_H

class GLKeys
{
public:
	GLKeys();
	//������а�����Ϣ
	void Clear();
	//�ж�ĳ�����Ƿ񱻰���
	bool IsPressed(unsigned int nKey);
	//����ĳ����������
	void SetPressed(unsigned int nKey);
	//����ĳ�����ͷ�
	void SetReleased(unsigned int nKey);

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_bKeyDown[MAX_KEYS];
};
#endif