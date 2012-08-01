using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class TypeUnmarshaller
{
    int[,] m_pBuffers = new int[12,12];

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out byte tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(byte)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = pBuffers[nUsed];
        return sizeof(byte);
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out char tValue)
    {
        tValue = '\0';
        if ((nUsed + sizeof(byte)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToChar(pBuffers, nUsed);
        return sizeof(byte);
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out float tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(float)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToSingle(pBuffers, nUsed);
        return sizeof(float);
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out uint tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(uint)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToUInt32(pBuffers, nUsed);
        return sizeof(uint);
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out int tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(uint)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToInt32(pBuffers, nUsed);
        return sizeof(uint);
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out short tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(short)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToInt16(pBuffers, nUsed);
        return sizeof(short);
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out ushort tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(ushort)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToUInt16(pBuffers, nUsed);
        return sizeof(ushort);
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out double tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(double)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToDouble(pBuffers, nUsed);
        return sizeof(double);
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out sbyte tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(byte)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = (sbyte)pBuffers[nUsed];
        return sizeof(byte);
    }
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out long tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(long)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToInt64(pBuffers, nUsed);
        return sizeof(long);
    }


    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out ulong tValue)
    {
        tValue = 0;
        if ((nUsed + sizeof(ulong)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        tValue = BitConverter.ToUInt64(pBuffers, nUsed);
        return sizeof(ulong);
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out string tValue)
    {
        tValue = string.Empty;
        if (nUsed + sizeof(int)+sizeof(char)>nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nStrLen = 0;
        int nLen = Unmarshall(pBuffers, nBuferLength, nUsed,out nStrLen);
        if (nLen < 0)
        {
            return nLen;
        }
        int nNeed = nStrLen + sizeof(char)+sizeof(int);
        if ((nNeed + nUsed) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        nUsed += nLen;
        if (nStrLen > 0)
        {
            byte[] pNew = new byte[nStrLen];
            Array.Copy(pBuffers, nUsed, pNew, 0, nStrLen);
            tValue = System.Text.Encoding.UTF8.GetString(pNew);
        }
        return nNeed;
    }

    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<byte> tValue)
    {
        tValue = new List<byte>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            byte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, byte> tValue)
    {
        tValue = new Dictionary<uint, byte>();
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
            byte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, byte> tValue)
    {
        tValue = new Dictionary<int, byte>();
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
            byte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, byte> tValue)
    {
        tValue = new Dictionary<ulong, byte>();
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
            byte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, byte> tValue)
    {
        tValue = new Dictionary<long, byte>();
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
            byte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<sbyte> tValue)
    {
        tValue = new List<sbyte>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            sbyte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, sbyte> tValue)
    {
        tValue = new Dictionary<uint, sbyte>();
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
            sbyte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, sbyte> tValue)
    {
        tValue = new Dictionary<int, sbyte>();
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
            sbyte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, sbyte> tValue)
    {
        tValue = new Dictionary<ulong, sbyte>();
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
            sbyte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, sbyte> tValue)
    {
        tValue = new Dictionary<long, sbyte>();
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
            sbyte tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<char> tValue)
    {
        tValue = new List<char>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            char tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, char> tValue)
    {
        tValue = new Dictionary<uint, char>();
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
            char tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, char> tValue)
    {
        tValue = new Dictionary<int, char>();
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
            char tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, char> tValue)
    {
        tValue = new Dictionary<ulong, char>();
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
            char tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, char> tValue)
    {
        tValue = new Dictionary<long, char>();
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
            char tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<short> tValue)
    {
        tValue = new List<short>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            short tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, short> tValue)
    {
        tValue = new Dictionary<uint, short>();
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
            short tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, short> tValue)
    {
        tValue = new Dictionary<int, short>();
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
            short tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, short> tValue)
    {
        tValue = new Dictionary<ulong, short>();
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
            short tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, short> tValue)
    {
        tValue = new Dictionary<long, short>();
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
            short tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<ushort> tValue)
    {
        tValue = new List<ushort>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            ushort tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, ushort> tValue)
    {
        tValue = new Dictionary<uint, ushort>();
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
            ushort tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, ushort> tValue)
    {
        tValue = new Dictionary<int, ushort>();
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
            ushort tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, ushort> tValue)
    {
        tValue = new Dictionary<ulong, ushort>();
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
            ushort tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, ushort> tValue)
    {
        tValue = new Dictionary<long, ushort>();
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
            ushort tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<int> tValue)
    {
        tValue = new List<int>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            int tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, int> tValue)
    {
        tValue = new Dictionary<uint, int>();
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
            int tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, int> tValue)
    {
        tValue = new Dictionary<int, int>();
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
            int tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, int> tValue)
    {
        tValue = new Dictionary<ulong, int>();
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
            int tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, int> tValue)
    {
        tValue = new Dictionary<long, int>();
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
            int tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<uint> tValue)
    {
        tValue = new List<uint>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            uint tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, uint> tValue)
    {
        tValue = new Dictionary<uint, uint>();
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
            uint tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, uint> tValue)
    {
        tValue = new Dictionary<int, uint>();
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
            uint tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, uint> tValue)
    {
        tValue = new Dictionary<ulong, uint>();
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
            uint tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, uint> tValue)
    {
        tValue = new Dictionary<long, uint>();
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
            uint tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<long> tValue)
    {
        tValue = new List<long>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            long tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, long> tValue)
    {
        tValue = new Dictionary<uint, long>();
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
            long tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, long> tValue)
    {
        tValue = new Dictionary<int, long>();
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
            long tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, long> tValue)
    {
        tValue = new Dictionary<ulong, long>();
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
            long tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, long> tValue)
    {
        tValue = new Dictionary<long, long>();
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
            long tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<ulong> tValue)
    {
        tValue = new List<ulong>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            ulong tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, ulong> tValue)
    {
        tValue = new Dictionary<uint, ulong>();
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
            ulong tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, ulong> tValue)
    {
        tValue = new Dictionary<int, ulong>();
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
            ulong tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, ulong> tValue)
    {
        tValue = new Dictionary<ulong, ulong>();
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
            ulong tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, ulong> tValue)
    {
        tValue = new Dictionary<long, ulong>();
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
            ulong tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<float> tValue)
    {
        tValue = new List<float>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            float tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, float> tValue)
    {
        tValue = new Dictionary<uint, float>();
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
            float tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, float> tValue)
    {
        tValue = new Dictionary<int, float>();
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
            float tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, float> tValue)
    {
        tValue = new Dictionary<ulong, float>();
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
            float tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, float> tValue)
    {
        tValue = new Dictionary<long, float>();
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
            float tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<double> tValue)
    {
        tValue = new List<double>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            double tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, double> tValue)
    {
        tValue = new Dictionary<uint, double>();
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
            double tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, double> tValue)
    {
        tValue = new Dictionary<int, double>();
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
            double tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, double> tValue)
    {
        tValue = new Dictionary<ulong, double>();
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
            double tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, double> tValue)
    {
        tValue = new Dictionary<long, double>();
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
            double tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out List<string> tValue)
    {
        tValue = new List<string>();
        int nLen = 0;
        int nRet = TypeUnmarshaller.Unmarshall(pBuffers, nBuferLength, nUsed, out nLen);
        if (nRet < MacrosAndDef.SUCCESS)
        {
            return nRet;
        }
        int nUnmarshalled = nRet;
        for (int i = 0; i < nLen; ++i)
        {
            string tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<uint, string> tValue)
    {
        tValue = new Dictionary<uint, string>();
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
            string tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<int, string> tValue)
    {
        tValue = new Dictionary<int, string>();
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
            string tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<ulong, string> tValue)
    {
        tValue = new Dictionary<ulong, string>();
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
            string tOut;
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
    static public int Unmarshall(byte[] pBuffers, int nBuferLength, int nUsed, out Dictionary<long, string> tValue)
    {
        tValue = new Dictionary<long, string>();
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
            string tOut;
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
