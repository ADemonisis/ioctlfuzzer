/*

	(c) eSage lab
	http://www.esagelab.ru

*/
#include "stdafx.h"
//--------------------------------------------------------------------------------------
/** 
* ����� ����� � ���������� �����
* @param file ��� �����
* @param line ����� ������
* @param msg �����
* � �������� �������� ���������� file � line ������ ���������� ��������� __FILE__ � __LINE__ �������������� 
*/
void DbgMsg(char *file, int line, char *msg, ...)
{
    char buff[1024], obuff[1024]="\0";
    va_list mylist;

    va_start(mylist, msg);
    wvsprintf(buff, msg, mylist);	
    va_end(mylist);

    wsprintf(obuff, "%s(%d) : %s", file, line, buff);	

#ifdef DBG
    OutputDebugString(obuff);
#endif

    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStd != INVALID_HANDLE_VALUE)
    {
        DWORD dwWritten;
        WriteFile(hStd, buff, lstrlen(buff), &dwWritten, NULL);
    }
}
//--------------------------------------------------------------------------------------
// EoF
