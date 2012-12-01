using System.Collections;
using System.Collections.Generic;
using System;
public class TJobInfos
{
    uint m_uMenuId;
    uint m_uLvl;
    uint m_uFatherMenuId;
    string m_pszNames;
    public TJobInfos()
    {
    }
    public static int Unmarshall(byte[] pBuffers,int nBufferLen,int nUsed, out TJobInfos rValue)
    {
        rValue = new TJobInfos();
        int nLen=0;
        nLen = TypeUnmarshaller.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.m_uMenuId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        nLen = TypeUnmarshaller.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.m_uLvl);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        nLen = TypeUnmarshaller.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.m_uFatherMenuId);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        nLen = TypeUnmarshaller.Unmarshall(pBuffers, nBufferLen, nUsed, out rValue.m_pszNames);
        if (nLen < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nUsed += nLen;
        return nUsed;
    }
    public static int GetLength(TJobInfos rValue)
    {
        return TypeMarshaller.GetLength(rValue.m_uMenuId)+TypeMarshaller.GetLength(rValue.m_uLvl)+TypeMarshaller.GetLength(rValue.m_uFatherMenuId)+TypeMarshaller.GetLength(rValue.m_pszNames);
    }
    public static int Marshall(byte[] pBuffers,int nBufferLen,int nUsed, TJobInfos rValue)
    {
        int nLen=0;
        int n;
        n = TypeMarshaller.Marshall(pBuffers, nBufferLen, nUsed, rValue.m_uMenuId);
        if (n < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nLen += n;
        nUsed += n;
        n = TypeMarshaller.Marshall(pBuffers, nBufferLen, nUsed, rValue.m_uLvl);
        if (n < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nLen += n;
        nUsed += n;
        n = TypeMarshaller.Marshall(pBuffers, nBufferLen, nUsed, rValue.m_uFatherMenuId);
        if (n < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nLen += n;
        nUsed += n;
        n = TypeMarshaller.Marshall(pBuffers, nBufferLen, nUsed, rValue.m_pszNames);
        if (n < MacrosAndDef.SUCCESS)
        {
            return MacrosAndDef.OUT_OF_MEM;
        }
        nLen += n;
        nUsed += n;
        return nUsed;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<TJobInfos> tValue)
    {
        if ((nUsed + GetLength(tValue)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nMarshalled = nRet;
        foreach(TJobInfos item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed+nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<TJobInfos> tValue)
    {
    int nLen = sizeof(int);
    foreach (TJobInfos item in tValue)
    {
        nLen += GetLength(item);
    }
    return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint,TJobInfos> tValue)
    {
        if ((nUsed + GetLength(tValue)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nMarshalled = nRet;
        foreach (KeyValuePair<uint, TJobInfos> item in tValue)
        {
            nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Key);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Value);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(Dictionary<uint,TJobInfos> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint,TJobInfos> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int,TJobInfos> tValue)
    {
        if ((nUsed + GetLength(tValue)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nMarshalled = nRet;
        foreach (KeyValuePair<int, TJobInfos> item in tValue)
        {
            nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Key);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Value);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(Dictionary<int,TJobInfos> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int,TJobInfos> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong,TJobInfos> tValue)
    {
        if ((nUsed + GetLength(tValue)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nMarshalled = nRet;
        foreach (KeyValuePair<ulong, TJobInfos> item in tValue)
        {
            nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Key);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Value);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(Dictionary<ulong,TJobInfos> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong,TJobInfos> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long,TJobInfos> tValue)
    {
        if ((nUsed + GetLength(tValue)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed, (int)tValue.Count);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nMarshalled = nRet;
        foreach (KeyValuePair<long, TJobInfos> item in tValue)
        {
            nRet = TypeMarshaller.Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Key);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item.Value);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(Dictionary<long,TJobInfos> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long,TJobInfos> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<TJobInfos> tValue)
    {
        tValue = new List<TJobInfos>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            TJobInfos tOut;
            nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nUnmarshalled += nRet;
            tValue.Add(tOut);
        }
        return nUnmarshalled;
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint,TJobInfos> tValue)
    {
        tValue = new Dictionary<uint, TJobInfos>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            uint uKey;
            nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out uKey);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            TJobInfos tOut;
            nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nUnmarshalled += nRet;
            tValue.Add(uKey, tOut);
        }
        return nUnmarshalled;
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int,TJobInfos> tValue)
    {
        tValue = new Dictionary<int, TJobInfos>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            int uKey;
            nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out uKey);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            TJobInfos tOut;
            nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nUnmarshalled += nRet;
            tValue.Add(uKey, tOut);
        }
        return nUnmarshalled;
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong,TJobInfos> tValue)
    {
        tValue = new Dictionary<ulong, TJobInfos>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            ulong uKey;
            nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out uKey);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            TJobInfos tOut;
            nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nUnmarshalled += nRet;
            tValue.Add(uKey, tOut);
        }
        return nUnmarshalled;
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long,TJobInfos> tValue)
    {
        tValue = new Dictionary<long, TJobInfos>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            long uKey;
            nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out uKey);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            TJobInfos tOut;
            nRet = Unmarshall(pBuffers, nBuferLength, nUsed + nUnmarshalled, out tOut);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nUnmarshalled += nRet;
            tValue.Add(uKey, tOut);
        }
        return nUnmarshalled;
    }
}
