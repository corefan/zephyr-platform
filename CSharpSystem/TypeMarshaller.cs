using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;


public class TypeMarshaller
{
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, uint tValue)
    {
        if ((nUsed + sizeof(uint)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(uint);
    }



    static public int GetLength(uint tValue)
    {
        return sizeof(uint);
    }



    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, int tValue)
    {
        if ((nUsed + sizeof(int)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(int);
    }
    static public int GetLength(int tValue)
    {
        return sizeof(int);
    }
    
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, short tValue)
    {
        if ((nUsed + sizeof(short)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(short);
    }
    static public int GetLength(short tValue)
    {
        return sizeof(short);
    }
   
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, ushort tValue)
    {
        if ((nUsed + sizeof(ushort)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(ushort);
    }
    static public int GetLength(ushort tValue)
    {
        return sizeof(ushort);
    }
   
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, float tValue)
    {
        if ((nUsed + sizeof(float)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(float);
    }
    static public int GetLength(float tValue)
    {
        return sizeof(float);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, double tValue)
    {
        if ((nUsed + sizeof(double)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(double);
    }
    static public int GetLength(double tValue)
    {
        return sizeof(double);
    }
    

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, byte tValue)
    {
        if ((nUsed + sizeof(byte)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        pBuffers[nUsed] = tValue;
        return sizeof(byte);
    }
    static public int GetLength(byte tValue)
    {
        return sizeof(byte);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, char tValue)
    {
        if ((nUsed + sizeof(sbyte)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(sbyte);
    }
    static public int GetLength(char tValue)
    {
        return sizeof(char);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, sbyte tValue)
    {
        if ((nUsed + sizeof(sbyte)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        pBuffers[nUsed] = (byte)tValue;
        return sizeof(sbyte);
    }
    static public int GetLength(sbyte tValue)
    {
        return sizeof(sbyte);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, long tValue)
    {
        if ((nUsed + sizeof(long)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(long);
    }
    static public int GetLength(long tValue)
    {
        return sizeof(long);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, ulong tValue)
    {
        if ((nUsed + sizeof(ulong)) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        byte[] pConvert2 = BitConverter.GetBytes(tValue);
        pConvert2.CopyTo(pBuffers, nUsed);
        return sizeof(ulong);
    }
    static public int GetLength(ulong tValue)
    {
        return sizeof(ulong);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, string tValue)
    {
        int nLength = (tValue.Length) + sizeof(int) + sizeof(char);
        if ((nUsed + nLength) > nBuferLength)
        {
            return MacrosAndDef.NOT_ENOUGH_BUFFER;
        }
        int nLen = Marshall(pBuffers,nBuferLength,nUsed, tValue.Length);
        if (nLen < 0)
        {
            return nLen;
        }
        nUsed += nLen;
        if (tValue.Length > 0)
        {
            byte[] pConvert2 = System.Text.Encoding.UTF8.GetBytes(tValue);
            pConvert2.CopyTo(pBuffers, nUsed);
            nUsed += pConvert2.Length;
        }
        char cNull = '\0';
        nLen = Marshall(pBuffers, nBuferLength, nUsed, cNull);
        return nLength;
    }
    static public int GetLength(string tValue)
    {
        return tValue.Length+sizeof(int)+sizeof(char);
    }

    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<byte> tValue)
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
        foreach (byte item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<byte> tValue)
    {
        int nLen = sizeof(int);
        foreach (byte item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, byte> tValue)
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
        foreach (KeyValuePair<uint, byte> item in tValue)
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
    static public int GetLength(Dictionary<uint, byte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, byte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, byte> tValue)
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
        foreach (KeyValuePair<int, byte> item in tValue)
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
    static public int GetLength(Dictionary<int, byte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, byte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, byte> tValue)
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
        foreach (KeyValuePair<ulong, byte> item in tValue)
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
    static public int GetLength(Dictionary<ulong, byte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, byte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, byte> tValue)
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
        foreach (KeyValuePair<long, byte> item in tValue)
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
    static public int GetLength(Dictionary<long, byte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, byte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<sbyte> tValue)
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
        foreach (sbyte item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<sbyte> tValue)
    {
        int nLen = sizeof(int);
        foreach (sbyte item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, sbyte> tValue)
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
        foreach (KeyValuePair<uint, sbyte> item in tValue)
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
    static public int GetLength(Dictionary<uint, sbyte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, sbyte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, sbyte> tValue)
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
        foreach (KeyValuePair<int, sbyte> item in tValue)
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
    static public int GetLength(Dictionary<int, sbyte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, sbyte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, sbyte> tValue)
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
        foreach (KeyValuePair<ulong, sbyte> item in tValue)
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
    static public int GetLength(Dictionary<ulong, sbyte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, sbyte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, sbyte> tValue)
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
        foreach (KeyValuePair<long, sbyte> item in tValue)
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
    static public int GetLength(Dictionary<long, sbyte> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, sbyte> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<char> tValue)
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
        foreach (char item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<char> tValue)
    {
        int nLen = sizeof(int);
        foreach (char item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, char> tValue)
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
        foreach (KeyValuePair<uint, char> item in tValue)
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
    static public int GetLength(Dictionary<uint, char> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, char> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, char> tValue)
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
        foreach (KeyValuePair<int, char> item in tValue)
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
    static public int GetLength(Dictionary<int, char> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, char> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, char> tValue)
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
        foreach (KeyValuePair<ulong, char> item in tValue)
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
    static public int GetLength(Dictionary<ulong, char> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, char> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, char> tValue)
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
        foreach (KeyValuePair<long, char> item in tValue)
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
    static public int GetLength(Dictionary<long, char> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, char> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<short> tValue)
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
        foreach (short item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<short> tValue)
    {
        int nLen = sizeof(int);
        foreach (short item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, short> tValue)
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
        foreach (KeyValuePair<uint, short> item in tValue)
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
    static public int GetLength(Dictionary<uint, short> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, short> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, short> tValue)
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
        foreach (KeyValuePair<int, short> item in tValue)
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
    static public int GetLength(Dictionary<int, short> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, short> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, short> tValue)
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
        foreach (KeyValuePair<ulong, short> item in tValue)
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
    static public int GetLength(Dictionary<ulong, short> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, short> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, short> tValue)
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
        foreach (KeyValuePair<long, short> item in tValue)
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
    static public int GetLength(Dictionary<long, short> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, short> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<ushort> tValue)
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
        foreach (ushort item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<ushort> tValue)
    {
        int nLen = sizeof(int);
        foreach (ushort item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, ushort> tValue)
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
        foreach (KeyValuePair<uint, ushort> item in tValue)
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
    static public int GetLength(Dictionary<uint, ushort> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, ushort> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, ushort> tValue)
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
        foreach (KeyValuePair<int, ushort> item in tValue)
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
    static public int GetLength(Dictionary<int, ushort> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, ushort> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, ushort> tValue)
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
        foreach (KeyValuePair<ulong, ushort> item in tValue)
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
    static public int GetLength(Dictionary<ulong, ushort> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, ushort> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, ushort> tValue)
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
        foreach (KeyValuePair<long, ushort> item in tValue)
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
    static public int GetLength(Dictionary<long, ushort> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, ushort> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<int> tValue)
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
        foreach (int item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<int> tValue)
    {
        int nLen = sizeof(int);
        foreach (int item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, int> tValue)
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
        foreach (KeyValuePair<uint, int> item in tValue)
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
    static public int GetLength(Dictionary<uint, int> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, int> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, int> tValue)
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
        foreach (KeyValuePair<int, int> item in tValue)
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
    static public int GetLength(Dictionary<int, int> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, int> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, int> tValue)
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
        foreach (KeyValuePair<ulong, int> item in tValue)
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
    static public int GetLength(Dictionary<ulong, int> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, int> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, int> tValue)
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
        foreach (KeyValuePair<long, int> item in tValue)
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
    static public int GetLength(Dictionary<long, int> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, int> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<uint> tValue)
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
        foreach (uint item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<uint> tValue)
    {
        int nLen = sizeof(int);
        foreach (uint item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, uint> tValue)
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
        foreach (KeyValuePair<uint, uint> item in tValue)
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
    static public int GetLength(Dictionary<uint, uint> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, uint> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, uint> tValue)
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
        foreach (KeyValuePair<int, uint> item in tValue)
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
    static public int GetLength(Dictionary<int, uint> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, uint> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, uint> tValue)
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
        foreach (KeyValuePair<ulong, uint> item in tValue)
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
    static public int GetLength(Dictionary<ulong, uint> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, uint> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, uint> tValue)
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
        foreach (KeyValuePair<long, uint> item in tValue)
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
    static public int GetLength(Dictionary<long, uint> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, uint> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<long> tValue)
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
        foreach (long item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<long> tValue)
    {
        int nLen = sizeof(int);
        foreach (long item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, long> tValue)
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
        foreach (KeyValuePair<uint, long> item in tValue)
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
    static public int GetLength(Dictionary<uint, long> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, long> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, long> tValue)
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
        foreach (KeyValuePair<int, long> item in tValue)
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
    static public int GetLength(Dictionary<int, long> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, long> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, long> tValue)
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
        foreach (KeyValuePair<ulong, long> item in tValue)
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
    static public int GetLength(Dictionary<ulong, long> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, long> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, long> tValue)
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
        foreach (KeyValuePair<long, long> item in tValue)
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
    static public int GetLength(Dictionary<long, long> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, long> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<ulong> tValue)
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
        foreach (ulong item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<ulong> tValue)
    {
        int nLen = sizeof(int);
        foreach (ulong item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, ulong> tValue)
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
        foreach (KeyValuePair<uint, ulong> item in tValue)
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
    static public int GetLength(Dictionary<uint, ulong> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, ulong> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, ulong> tValue)
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
        foreach (KeyValuePair<int, ulong> item in tValue)
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
    static public int GetLength(Dictionary<int, ulong> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, ulong> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, ulong> tValue)
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
        foreach (KeyValuePair<ulong, ulong> item in tValue)
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
    static public int GetLength(Dictionary<ulong, ulong> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, ulong> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, ulong> tValue)
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
        foreach (KeyValuePair<long, ulong> item in tValue)
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
    static public int GetLength(Dictionary<long, ulong> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, ulong> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<float> tValue)
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
        foreach (float item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<float> tValue)
    {
        int nLen = sizeof(int);
        foreach (float item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, float> tValue)
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
        foreach (KeyValuePair<uint, float> item in tValue)
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
    static public int GetLength(Dictionary<uint, float> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, float> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, float> tValue)
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
        foreach (KeyValuePair<int, float> item in tValue)
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
    static public int GetLength(Dictionary<int, float> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, float> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, float> tValue)
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
        foreach (KeyValuePair<ulong, float> item in tValue)
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
    static public int GetLength(Dictionary<ulong, float> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, float> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, float> tValue)
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
        foreach (KeyValuePair<long, float> item in tValue)
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
    static public int GetLength(Dictionary<long, float> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, float> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<double> tValue)
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
        foreach (double item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<double> tValue)
    {
        int nLen = sizeof(int);
        foreach (double item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, double> tValue)
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
        foreach (KeyValuePair<uint, double> item in tValue)
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
    static public int GetLength(Dictionary<uint, double> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, double> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, double> tValue)
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
        foreach (KeyValuePair<int, double> item in tValue)
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
    static public int GetLength(Dictionary<int, double> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, double> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, double> tValue)
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
        foreach (KeyValuePair<ulong, double> item in tValue)
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
    static public int GetLength(Dictionary<ulong, double> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, double> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, double> tValue)
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
        foreach (KeyValuePair<long, double> item in tValue)
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
    static public int GetLength(Dictionary<long, double> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, double> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, List<string> tValue)
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
        foreach (string item in tValue)
        {
            nRet = Marshall(pBuffers, nBuferLength, nUsed + nMarshalled, item);
            if (nRet < MacrosAndDef.SUCCESS)
            {
                return nRet;
            }
            nMarshalled += nRet;
        }
        return nMarshalled;
    }
    static public int GetLength(List<string> tValue)
    {
        int nLen = sizeof(int);
        foreach (string item in tValue)
        {
            nLen += GetLength(item);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<uint, string> tValue)
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
        foreach (KeyValuePair<uint, string> item in tValue)
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
    static public int GetLength(Dictionary<uint, string> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<uint, string> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<int, string> tValue)
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
        foreach (KeyValuePair<int, string> item in tValue)
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
    static public int GetLength(Dictionary<int, string> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<int, string> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<ulong, string> tValue)
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
        foreach (KeyValuePair<ulong, string> item in tValue)
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
    static public int GetLength(Dictionary<ulong, string> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<ulong, string> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }
    static public int Marshall(byte[] pBuffers, int nBuferLength, int nUsed, Dictionary<long, string> tValue)
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
        foreach (KeyValuePair<long, string> item in tValue)
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
    static public int GetLength(Dictionary<long, string> tValue)
    {
        int nLen = sizeof(int);
        foreach (KeyValuePair<long, string> item in tValue)
        {
            nLen += TypeMarshaller.GetLength(item.Key);
            nLen += GetLength(item.Value);
        }
        return nLen;
    }

}
