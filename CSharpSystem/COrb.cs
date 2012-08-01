using UnityEngine;
using System.Collections;

public class COrb : MonoBehaviour {

	// Use this for initialization
	CSkeleton[] m_pRegisteredObj;
    public MySocket m_pSocket = null;

    void Init(MySocket pSocket)
    {
        m_pSocket = pSocket;
    }
	// Update is called once per frame
	void Update () 
    {
        if (null == m_pSocket)
        {
            return;
        }
        int nNr = 128;
        while (nNr > 0)
        {
            CMessage pMsg = m_pSocket.GetMsg();
            if (null != pMsg)
            {
                --nNr;
                for (int i=0;i<m_pRegisteredObj.Length;++i)
                {
                    int nRet = m_pRegisteredObj[i].HandleMsg(pMsg);
                    if (nRet != MacrosAndDef.MSG_NOT_HANDLED)
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }
        }
	}
    void RegisterService(CSkeleton pSkeleton)
    {
        if (null == m_pRegisteredObj)
        {
            m_pRegisteredObj = new CSkeleton[1];
        }
        else
        {
            CSkeleton[] pNew = new CSkeleton[m_pRegisteredObj.Length + 1];
            int i=0;
            for (;i<m_pRegisteredObj.Length;++i)
            {
                pNew[i] = m_pRegisteredObj[i];
            }
            pNew[i] = pSkeleton;
            m_pRegisteredObj = pNew;
        }
    }
}
