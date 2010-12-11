/*

	(c) eSage lab
	http://www.esagelab.ru

*/
#include "stdafx.h"
//--------------------------------------------------------------------------------------
/** 
 * ��������� xml-���� �� ������ �� ��� �����
 * @param NodeName ��� �������� ����
 * @param pIDOMNodeList ���������� ������
 * @return ���������� ������� ����, ��� NULL � ������ �������
 * @see ConfGetNodeByName() 
 * @see ConfGetNodeText() 
 * @see ConfGetTextByName()
 */
IXMLDOMNode * __stdcall ConfGetListNodeByName(BSTR NodeName, IXMLDOMNodeList *pIDOMNodeList)
{
	IXMLDOMNode *pIDOMChildNode = NULL;
	LONG len;
	BSTR ChildNodeName;

	if (!pIDOMNodeList)
		return NULL;

	HRESULT hr = pIDOMNodeList->get_length(&len);
	if (SUCCEEDED(hr))
	{
		pIDOMNodeList->reset();
		for (int i = 0; i < len; i++)
		{
			hr = pIDOMNodeList->get_item(i, &pIDOMChildNode);
			if (SUCCEEDED(hr))
			{
				pIDOMChildNode->get_nodeName(&ChildNodeName);

				if (!wcscmp(NodeName, ChildNodeName))
					return pIDOMChildNode;

				pIDOMChildNode->Release();
				pIDOMChildNode = NULL;
			} 
            else 
            {
				DbgMsg(__FILE__, __LINE__, "pIDOMNodeList->get_item() ERROR 0x%.8x\n", hr);
			}
		}
	} 
    else 
    {
		DbgMsg(__FILE__, __LINE__, "pIDOMNodeList->get_length() ERROR 0x%.8x\n", hr);
	}

	return NULL;
}
//--------------------------------------------------------------------------------------
/** 
 * ��������� ������� �� ��� �����
 * @param NodeName ��� �������� ����
 * @param pIDOMNode ���������� ������������� ����
 * @return ���������� ������� ����, ��� NULL � ������ �������
 * @see ConfGetListNodeByName()  
 * @see ConfGetNodeText() 
 * @see ConfGetTextByName()
 */
IXMLDOMNode * __stdcall ConfGetNodeByName(BSTR NodeName, IXMLDOMNode *pIDOMNode)
{
	IXMLDOMNode *pIDOMRetNode = NULL;
	IXMLDOMNodeList *pIDOMNodeList = NULL;

	if (pIDOMNode == NULL)
		return NULL;

	HRESULT hr = pIDOMNode->get_childNodes(&pIDOMNodeList);
	if (SUCCEEDED(hr) && pIDOMNodeList)
	{
        pIDOMRetNode = ConfGetListNodeByName(NodeName, pIDOMNodeList);
        pIDOMNodeList->Release();		
	} 
    else 
    {
		DbgMsg(__FILE__, __LINE__, "pIDOMNodeList->get_length() ERROR 0x%.8x\n", hr);
	}

	return pIDOMRetNode;
} 
//--------------------------------------------------------------------------------------
/** 
 * ��������� �������� ����
 * @param pIDOMNode ���������� ����
 * @param str ������ unicode-������, � ������� ����� �������� ��������
 * @return TRUE ���� �� ��, FALSE � ������ ������
 * @see ConfGetListNodeByName() 
 * @see ConfGetNodeByName() 
 * @see ConfGetTextByName()
 */
BOOL __stdcall ConfGetNodeTextW(IXMLDOMNode *pIDOMNode, PWSTR *str)
{
    BOOL bRet = FALSE;
	BSTR val;

	if (!pIDOMNode)
		return FALSE;

	HRESULT hr = pIDOMNode->get_text(&val);
	if (FAILED(hr))
	{
		DbgMsg(__FILE__, __LINE__, "pIDOMNode->get_text() ERROR 0x%.8x\n", hr);
		return FALSE;
	}

	if (*str = (PWSTR)M_ALLOC((wcslen((PWSTR)val) + 1) * sizeof(WCHAR)))
    {
        wcscpy(*str, (PWSTR)val);
        bRet = TRUE;
    }
    else
    {
		DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
    }

	SysFreeString(val);

	return bRet;
}
//--------------------------------------------------------------------------------------
/** 
 * ��������� �������� ����
 * @param pIDOMNode ���������� ����
 * @param str ������ unicode-������, � ������� ����� �������� ��������
 * @return TRUE ���� �� ��, FALSE � ������ ������
 * @see ConfGetListNodeByName() 
 * @see ConfGetNodeByName() 
 * @see ConfGetTextByName()
 */
BOOL __stdcall ConfGetNodeTextA(IXMLDOMNode *pIDOMNode, PCHAR *str)
{
    BOOL bRet = FALSE;
    PWSTR str_w;

    if (ConfGetNodeTextW(pIDOMNode, &str_w))
    {
        int len = wcslen(str_w);
        if (*str = (PCHAR)M_ALLOC(len + 1))
        {
            WideCharToMultiByte(CP_ACP, 0, str_w, -1, *str, len, NULL, NULL);    
            bRet = TRUE;
        }
        else
        {
		    DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        }

        M_FREE(str_w);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
/** 
 * ��������� �������� ������� �� ��� �����
 * @param pIDOMNode ���������� ������������� ����
 * @param name ��� ��������� ����, �������� �������� ���������� ��������
 * @param val ������ ��������� �� unicode-������, � ������� ����� �������� ��������
 * @return TRUE ���� �� ��, FALSE � ������ ������
 * @see ConfGetListNodeByNameA() 
 * @see ConfGetListNodeByName() 
 * @see ConfGetNodeByName() 
 * @see ConfGetNodeText() 
 * @see ConfGetTextByName()
 */
BOOL __stdcall ConfAllocGetTextByNameW(IXMLDOMNode *pIDOMNode, PWSTR name, PWSTR *value)
{
	BOOL bRet = FALSE;
    
	IXMLDOMNode *pIDOMChildNode = ConfGetNodeByName(name, pIDOMNode);
	if (pIDOMChildNode)
	{
        bRet = ConfGetNodeTextW(pIDOMChildNode, value);		
	
		pIDOMChildNode->Release();
	}

	return bRet;
}
//--------------------------------------------------------------------------------------
/** 
 * ��������� �������� ������� �� ��� �����
 * @param pIDOMNode ���������� ������������� ����
 * @param name ��� ��������� ����, �������� �������� ���������� ��������
 * @param val ������ ��������� �� unicode-������, � ������� ����� �������� ��������
 * @return TRUE ���� �� ��, FALSE � ������ ������
 * @see ConfGetListNodeByNameW() 
 * @see ConfGetListNodeByName() 
 * @see ConfGetNodeByName() 
 * @see ConfGetNodeText() 
 * @see ConfGetTextByName()
 */
BOOL __stdcall ConfAllocGetTextByNameA(IXMLDOMNode *pIDOMNode, PWSTR name, PCHAR *value)
{
    BOOL bRet = FALSE;
    PWSTR value_w;

    if (ConfAllocGetTextByNameW(pIDOMNode, name, &value_w))
    {
        int len = wcslen(value_w);
        if (*value = (PCHAR)M_ALLOC(len + 1))
        {
            WideCharToMultiByte(CP_ACP, 0, value_w, -1, *value, len, NULL, NULL);    
            bRet = TRUE;
        }
        else
        {
		    DbgMsg(__FILE__, __LINE__, "M_ALLOC() ERROR %d\n", GetLastError());
        }

        M_FREE(value_w);
    }

    return bRet;
}
//--------------------------------------------------------------------------------------
// EoF