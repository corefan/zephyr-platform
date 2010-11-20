/*************************************************
  Copyright (C), 1988-1999, Zephyr Platform
  File name:      tplMap.h
  Author:         Zephyr Shannon
  Version:        V1.2
  Date:           12-22-2009
  Description:    Map template
  Others:         none
  Function List:  
      It looks like std::map, but more efficient,
      I take consider of the worst condition.
      And for the most important thing: U can read and debug it!
  History:
*************************************************/
#ifndef _S_MAP_TEMPLATE_H_
#define _S_MAP_TEMPLATE_H_


#include "SysMacros.h"
#include "TypeDef.h"
#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif


namespace Zephyr
{

template<class CItem,class CKey>
class TplNode : public CItem
{
public:
    //CKey                       m_key;
    TUInt32               m_nodeSize:31;
    TUInt32               m_isActive:1;

    TplNode<CItem,CKey>*    m_pLeftNode;
    TplNode<CItem,CKey>*    m_pRightNode;
    TplNode<CItem,CKey>*    m_pParent;
public:
    void     CheckTree()
    {
        if (m_pLeftNode)
        {
            m_pLeftNode->CheckTree();
        }
        CheckNode();
        if (m_pRightNode)
        {
            m_pRightNode->CheckTree();
        }
    }


private:
    enum EnActive
    {
        active_mask     = 0x80000000,
        deactive_mask   = 0x7FFFFFFF,
    };
    enum EnTreeSize
    {
        tree_size_mask  = 0x7FFFFFFF,
        balance_mask    = 0x80000000,
    };
    enum EnNextAddNode
    {
        left_node       = 0,
        right_node      = 1,
    };
public:

    TplNode<CItem,CKey> *GetNextSmallFirst(TplNode<CItem,CKey> *pNow)
    {
        if (!pNow)
        {
            return NULL;
        }
        TplNode<CItem,CKey> *pResult = NULL;
        if (CItem::m_key > pNow->m_key)
        {
            if (m_pLeftNode)
            {
                pResult = m_pLeftNode->GetNextSmallFirst(pNow);
                if (!pResult)
                {
                    pResult = this;
                }
                return pResult;
            }
            else
            {
                return NULL;
            }
        }
        if (CItem::m_key == pNow->m_key)
        {
            if (m_pRightNode)
            {
                return m_pRightNode->GetSmallest();
            }
            return NULL;
        }
        //else 
        if (!m_pRightNode)
        {
            return NULL;
        }
        else
        {
            return m_pRightNode->GetNextSmallFirst(pNow);
        }
    }

    TplNode<CItem,CKey> *FindNode(CKey& key);
    TplNode<CItem,CKey> *SafeAddNode(TplNode *pNode,TInt32&result)
    {
            TplNode<CItem,CKey> *pResult = this;
            if (FindNode(pNode->m_key))
            {
                result = FAIL;
            }
            else
            {
                result = SUCCESS;
                pResult = AddNode(pNode);
            }
            return pResult;
    }

    //if return NULL, mean this node had already existed! U can't add again!
    TplNode<CItem,CKey> *AddNode(TplNode *pNode);
public:
    //high efficiency also use this in release.
    TplNode<CItem,CKey> *SafeReleaseNode(CKey& key,TInt32& result)
        {
            TplNode<CItem,CKey> *pResult = this;
            if (FindNode(key))
            {
                result = SUCCESS;
                pResult = ReleaseNode(key);
            }
            else
            {
                result = FAIL;
            }
            return pResult;
        }
    //if the key does not exist, U wouldn't know about it.
    TplNode<CItem,CKey> *ReleaseNode(CKey& key);
    void        Init()
                {
                    m_nodeSize = 0;
                    m_isActive = 1;
                    m_pLeftNode = NULL;
                    m_pRightNode = NULL;
                    m_pParent    = NULL;
                }
    void        UnInit()
                {
                    m_nodeSize = 0;
                    m_isActive = 0;
                    m_pLeftNode = NULL;
                    m_pRightNode = NULL;
                    m_pParent    = NULL;
                }
    bool        IsActived()
                {
                    return m_isActive;
                }
    void        Active()
                {
                    m_isActive = 1;
                }
    void        Deactive()
                {
                    m_isActive = 0;
                }
    
    void        Print(TInt32 etch,TInt32& count);
    typedef TInt32 (CItem::*_PFMSG)(TInt32); 
	typedef TInt32 (*_PFV)(CItem *pThis,void *);

	void		RunOnTreeMiddleFirst(_PFV ptr,void *pArg)
				{
					ptr(this,pArg);
					if (m_pLeftNode)
					{
						m_pLeftNode->RunOnTreeMiddleFirst(ptr,pArg);
					}
					if (m_pRightNode)
					{
						m_pRightNode->RunOnTreeMiddleFirst(ptr,pArg);
					}
				}

    void        RunOnTree(_PFMSG ptr,TInt32 arg)
                {
                    if (m_pLeftNode)
                    {
                        m_pLeftNode->RunOnTree(ptr,arg);
                    }
					(this->*ptr)(arg);
                    if (m_pRightNode)
                    {
                        m_pRightNode->RunOnTree(ptr,arg);
                    }
                }
    

    TInt32         GetTreeSize()
                {
                    return (m_nodeSize + 1);
                }
private:
    bool        NeedRearrange(EnNextAddNode nextAddNode)
                {
                    //return false;
                    TInt32 leftNodeSize = 0;
                    TInt32 rightNodeSize = 0;
                    if (m_pLeftNode)
                    {
                        leftNodeSize = m_pLeftNode->GetTreeSize();
                    }
                    if (m_pRightNode)
                    {
                        rightNodeSize = m_pRightNode->GetTreeSize();
                    }
                    TInt32 balance = 0;
                    TInt32 totalNum = GetTreeSize();
                    totalNum = totalNum>>2;
                    totalNum += 4;
                    if (left_node == nextAddNode)
                    {
                        balance = leftNodeSize - rightNodeSize;
                    }
                    else // the right node case
                    {
                        balance = rightNodeSize - leftNodeSize;
                    }
                    /*
                    if (balance > 64)
                    {
                        return true;
                    }
                    */
                    if (balance > totalNum)
                    {
                        return true;
                    }
                    return false;
                }

    void        AddLeftNodeNum()
                {
                    m_nodeSize ++;
                }

    void        DelLeftNodeNum()
                {
                    m_nodeSize --;
                }

    void        AddRightNodeNum()
                {
                    m_nodeSize ++;
                }

    void        DelRightNodeNum()
                {
                    m_nodeSize --;
                }
public:
    TInt32         GetMaxHeight()
                {
                    TInt32 lHt = 0;
                    TInt32 rHt = 0;
                    if (m_pLeftNode)
                    {
                        lHt = m_pLeftNode->GetMaxHeight();
                    }
                    if (m_pRightNode)
                    {
                        rHt = m_pRightNode->GetMaxHeight();
                    }
                    if (lHt > rHt)
                    {
                        return (++lHt);
                    }
                    else
                    {
                        return (++rHt);
                    }
                }
private:
    bool IsLeftChild()
    {
        if (m_pParent)
        {
            if (m_pParent->m_pLeftNode == this)
            {
                return true;
            }
        }
        return false;
    }
    bool IsRoot()
    {
        if (m_pParent)
        {
            return false;
        }
        return true;
    }
    bool IsRightChild()
    {
        if (m_pParent)
        {
            if (m_pParent->m_pRightNode == this)
            {
                return true;
            }
        }
        return false;
    }
    TInt32        CheckNode()
    {
        TInt32 totalNode = 0;
        if (m_pLeftNode)
        {
            if(m_pLeftNode->FindNode(CItem::m_key))
            {
                printf("check tree failed, find self in the m_pLeftNode \n");
                TInt32 count = 0;
                Print(1,count);
                return FAIL;
            }
            if (!(CItem::m_key > m_pLeftNode->m_key))
            {
                printf("check tree failed, left node key is not smaller than root!\n");
                TInt32 count = 0;
                Print(1,count);
                return FAIL;
            }
            totalNode += (m_pLeftNode->GetTreeSize());
        }
        if (m_pRightNode)
        {
            if (m_pRightNode->FindNode(CItem::m_key))
            {
                printf("check tree failed, find self in the m_pRightNode \n");
                TInt32 count = 0;
                Print(1,count);
                return FAIL;
            }
            if (!(CItem::m_key < m_pRightNode->m_key))
            {
                printf("check tree failed, right node key is not bigger than root!\n");
                TInt32 count = 0;
                Print(1,count);
                return FAIL;
            }
            totalNode += (m_pRightNode->GetTreeSize());
        }
        if (totalNode != (GetTreeSize()-1))
        {
            printf("check tree failed, tree size incorrect!\n");
            TInt32 count = 0;
            Print(1,count);
            return FAIL;
        }
        if (m_pParent)
        {
            if (m_pParent->m_key == m_key)
            {
                printf("Check tree failed ,parent's key is the same to this!\n");
                return FAIL;
            }
            if (m_pParent->m_key > m_key)
            {
                if (m_pParent->m_pLeftNode != this)
                {
                    printf("check tree failed ,the tree parent is incorrect!\n");
                    return FAIL;
                }
            }
            else
            {
                if (m_pParent->m_pRightNode != this)
                {
                    printf("check tree failed, the tree parent is incorrect!\n");
                    return FAIL;
                }
            }
        }
        return SUCCESS;
    }
public:
    TplNode<CItem,CKey>* GetBigest()
                            {
                            #ifdef _DEBUG
                                if (this == m_pRightNode)
                                {
                                    return NULL;
                                }
                            #endif
                                if (m_pRightNode)
                                {
                                    return m_pRightNode->GetBigest();
                                }
                                return this;
                            }

    TplNode<CItem,CKey>* GetSmallest()
                            {
                            #ifdef _DEBUG
                                if (this == m_pLeftNode)
                                {
                                    return NULL;
                                }
                            #endif
                                if (m_pLeftNode)
                                {
                                    return m_pLeftNode->GetSmallest();
                                }
                                return this;
                            }

    TplNode<CItem,CKey>* GetRoot()
                            {
                                if (m_pParent)
                                {
                                    return m_pParent->GetRoot();
                                }
                                return this;
                            }

public:
    class Iterator
    {
    private:
        TplNode<CItem,CKey>* m_pNow;
		//TplNode<CItem,CKey>* m_pRoot;
    public:
        Iterator()
        {
            m_pNow = NULL;
            //m_pRoot = NULL;
        }
        Iterator(TplNode<CItem,CKey>* pNow)
        {
            m_pNow = pNow;
        }
        TInt32 Init(TplNode<CItem,CKey>* pNow)
        {
            if(!pNow)
            {
                return NULL_POINTER;
            }
            //m_pRoot = pNow;
			m_pNow = pNow;
            return SUCCESS;
        }
        //前缀++，没有后缀++
        Iterator &operator ++()
        {
            //m_pNow = m_pRoot->GetNextSmallFirst(m_pNow);
            TplNode<CItem,CKey>* pTmp = m_pNow;
            TplNode<CItem,CKey>* pLast = NULL;
            //m_pNow = NULL;
            while (pTmp)
            {
                if ((pTmp->m_pRightNode) && (pTmp->m_pRightNode != pLast))
                {
                    m_pNow = pTmp->m_pRightNode->GetSmallest();
                    return *this;
                }
                else
                {
                    if (pTmp->IsLeftChild())
                    {
                        m_pNow = pTmp->m_pParent;
                        return *this;
                    }
                    else
                    {
                        pLast = pTmp;
                        pTmp = pTmp->m_pParent;
                    }
                }
            }
            m_pNow = NULL;
            return *this;
        }
        Iterator &operator --()
        {
            TplNode<CItem,CKey>* pTmp = m_pNow;
            TplNode<CItem,CKey>* pLast = NULL;
            m_pNow = NULL;
            while (pTmp)
            {
                if ((pTmp->m_pLeftNode) && (pTmp->m_pLeftNode != pLast))
                {
                    m_pNow = pTmp->m_pLeftNode->GetBigest();
                    return *this;
                }
                else
                {
                    if (pTmp->IsRightChild())
                    {
                        m_pNow = pTmp->m_pParent;
                        return *this;
                    }
                    else
                    {
                        pLast = pTmp;
                        pTmp = pTmp->m_pParent;
                    }
                }
            }
            m_pNow = NULL;
            return *this;
        }
        CItem*      operator ->()
        {
            return m_pNow;
        }
        bool        operator == (Iterator& itor)
        {
            if (itor.m_pNow == m_pNow)
            {
                return true;
            }
            return false;
        }
		TplNode<CItem,CKey>* GetItem()
		{
			return m_pNow;
		}
		TInt32 Size()
		{
		    if (m_pNow)
		    {
		        return (m_pNow->GetRoot()->GetTreeSize());
		    }
		    return 0;
		}
		bool IsNull()
		{
		    return (m_pNow == NULL); 
		}
    };

};


template<class CItem,class CKey>
void TplNode<CItem,CKey>::Print(TInt32 etch,TInt32& count)
{
    TInt32 needPlusSign = 0;
    if (m_pLeftNode)
    {
        m_pLeftNode->Print(etch+1,count);
    }
    else
    {
        if (m_pRightNode)
        {
            needPlusSign = 1;
           /* for(TInt32 i=0;i<etch+1;i++)
            {
                cout<<"    ";
            }
            cout<<"-["<<etch + 1<<"]"<<"NULL"<<endl;*/
        }
    }

    for(TInt32 i=0;i<etch;i++)
    {
        cout<<"    ";
    }

    cout<<"-["<<etch<<"]";
    if (needPlusSign)
    {
        cout<<"+  ";
    }
    cout<<CItem::m_key<<"("<<count++<<") nodeSize :"<<m_nodeSize<<endl;

    if (m_pRightNode)
    {
        m_pRightNode->Print(etch+1,count);
    }
    else
    {
        if (m_pLeftNode)
        {
            /*for(TInt32 i=0;i<etch+1;i++)
            {
                cout<<"    ";
            }
            cout<<"-["<<etch + 1<<"]"<<"NULL"<<endl;*/

        }
    }
}



template<class CItem,class CKey>
TplNode<CItem,CKey> *TplNode<CItem,CKey>::FindNode(CKey& key)
{
    if (CItem::m_key == key)
    {
        return this;
    }
    if (CItem::m_key > key)
    {
        if (m_pLeftNode)
        {
            return m_pLeftNode->FindNode(key);
        }
        return NULL;
    }
    if (m_pRightNode)
    {
        return m_pRightNode->FindNode(key);
    }
    return NULL;
}

template<class CItem,class CKey>
TplNode<CItem,CKey> *TplNode<CItem,CKey>::AddNode(TplNode *pNode)
{
//#ifdef _DEBUG
    if (this == pNode)
    {
        return this;
    }
//#endif
#ifdef _DEBUG
    if (NULL == pNode)
    {
        
        CheckNode();

        return NULL;
    }
    pNode->Init();
    if (CItem::m_key == pNode->m_key)
    {
        //almost never happened!
        #ifdef _NEED_TREE_CHECK
        CheckNode();
        #endif
        return this;
    }
#endif
    if (CItem::m_key > pNode->m_key)
    {
        if (NeedRearrange(left_node))
        {
            if (NULL == m_pLeftNode)
            {
                //sth wrong!! this should never happened!
                m_pLeftNode = pNode;
                pNode->m_pParent = this;
                AddLeftNodeNum();
                #ifdef _NEED_TREE_CHECK
                CheckNode();
                #endif
                return this;
            }
            //重新修整树
            TplNode<CItem,CKey>* pNewRoot = m_pLeftNode;
            m_pLeftNode    = m_pLeftNode->m_pRightNode;
            pNewRoot->m_pRightNode = this;
            m_nodeSize = 0;
            
            //修整m_pParent
            pNewRoot->m_pParent = m_pParent;
            this->m_pParent = pNewRoot;
            if (m_pLeftNode)
            {
                m_pLeftNode->m_pParent = this;
            }
            //修整结束
            
            if(m_pLeftNode)
            {
                m_nodeSize += m_pLeftNode->GetTreeSize();
            }
            if (m_pRightNode)
            {
                m_nodeSize += m_pRightNode->GetTreeSize();
            }
            pNewRoot->m_nodeSize = 0;
            if (pNewRoot->m_pLeftNode)
            {
                pNewRoot->m_nodeSize += pNewRoot->m_pLeftNode->GetTreeSize();
            }
            if (pNewRoot->m_pRightNode)
            {
                pNewRoot->m_nodeSize += pNewRoot->m_pRightNode->GetTreeSize();
            }
            //结束
            
            if (pNewRoot)
            {
                if (pNewRoot->m_key > pNode->m_key)
                {
                    if (pNewRoot->m_pLeftNode)
                    {
                        pNewRoot->m_pLeftNode = pNewRoot->m_pLeftNode->AddNode(pNode);
                    }
                    else
                    {
                        pNode->m_pParent = pNewRoot;
                        pNewRoot->m_pLeftNode = pNode;
                    }
                    pNewRoot->AddLeftNodeNum();
                }
                else
                {
                    pNewRoot->m_pRightNode = pNewRoot->m_pRightNode->AddNode(pNode);
                    pNewRoot->AddRightNodeNum();
                }
            }
            #ifdef _NEED_TREE_CHECK
            pNewRoot->CheckTree();
            #endif
            return pNewRoot;
            
            /*
            TplNode<CItem,CKey>* pNewRoot = m_pLeftNode->GetBigest();

            if ((pNode->m_key) > (pNewRoot->m_key))
            {
                //the new added node would be the root!
                pNewRoot = pNode;
            }
            else
            {
                m_pLeftNode = m_pLeftNode->ReleaseNode(pNewRoot->m_key);
                #ifdef _DEBUG
                if (m_pLeftNode)
                {
                    m_pLeftNode->CheckTree();
                }
                #endif
                //BEGIN ADD 01-04-2009 S0032 TDS00034
                if (m_pLeftNode)
                {
                //END ADD 01-04-2009 S0032 TDS00034
                    m_pLeftNode = m_pLeftNode->AddNode(pNode);
                    #ifdef _DEBUG
                    m_pLeftNode->CheckTree();
                    #endif
                //BEGIN ADD 01-04-2009 S0032 TDS00034
                }
                else
                {
                    m_pLeftNode = pNode;

                }
                //END ADD 01-04-2009 S0032 TDS00034

            }
            pNewRoot->m_pLeftNode = m_pLeftNode;
            pNewRoot->m_pRightNode = m_pRightNode;
            pNewRoot->m_nodeSize = m_nodeSize;


            if (pNewRoot->m_pRightNode)
            {

                pNewRoot->m_pRightNode = pNewRoot->m_pRightNode->AddNode(this);

            }
            else
            {
                

                //BEGIN ADD 01-04-2009 S0032 TDS00036
                this->Init();
                //END ADD 01-04-2009 S0032 TDS00036
                pNewRoot->m_pRightNode = this;
            }

            pNewRoot->AddRightNodeNum();
            */
#ifdef _NEED_TREE_CHECK
            pNewRoot->CheckTree();
#endif
            return pNewRoot;

        }
        else
        {
            if (m_pLeftNode)
            {
                m_pLeftNode = m_pLeftNode->AddNode(pNode);
#ifdef _NEED_TREE_CHECK
                m_pLeftNode->CheckTree();
#endif
            }
            //BEGIN ADD 01-04-2009 S0032 TDS00035
            else
            {
            //END ADD 01-04-2009 S0032 TDS00034
                pNode->m_pParent = this;
                m_pLeftNode = pNode;
            }
            AddLeftNodeNum();
#ifdef _NEED_TREE_CHECK
            CheckTree();
#endif
            return this;
        }
    }

    //add to right node.
    if (NeedRearrange(right_node))
    {
        if (NULL == m_pRightNode)
        {

            m_pRightNode = pNode;
            AddRightNodeNum();
#ifdef _NEED_TREE_CHECK
            CheckNode();
#endif
            return this;
        }
        //重新修正树
        TplNode<CItem,CKey>* pNewRoot = m_pRightNode;
        m_pRightNode    = m_pRightNode->m_pLeftNode;
        pNewRoot->m_pLeftNode = this;
        m_nodeSize = 0;
        
        //修整m_pParent
        pNewRoot->m_pParent = m_pParent;
        this->m_pParent = pNewRoot;
        if (m_pRightNode)
        {
            m_pRightNode->m_pParent = this;
        }
        //修整结束
        
        if(m_pLeftNode)
        {
            m_nodeSize += m_pLeftNode->GetTreeSize();
        }
        if (m_pRightNode)
        {
            m_nodeSize += m_pRightNode->GetTreeSize();
        }
        pNewRoot->m_nodeSize = 0;
        if (pNewRoot->m_pLeftNode)
        {
            pNewRoot->m_nodeSize += pNewRoot->m_pLeftNode->GetTreeSize();
        }
        if (pNewRoot->m_pRightNode)
        {
            pNewRoot->m_nodeSize += pNewRoot->m_pRightNode->GetTreeSize();
        }
        if (pNewRoot)
        {
            if (pNewRoot->m_key > pNode->m_key)
            {
                pNewRoot->m_pLeftNode = pNewRoot->m_pLeftNode->AddNode(pNode);
                pNewRoot->AddLeftNodeNum();
            }
            else
            {
                if (pNewRoot->m_pRightNode)
                {
                    pNewRoot->m_pRightNode = pNewRoot->m_pRightNode->AddNode(pNode);
                }
                else
                {
                    pNode->m_pParent = pNewRoot;
                    pNewRoot->m_pRightNode = pNode;
                }
                pNewRoot->AddRightNodeNum();
            }
        }
        #ifdef _NEED_TREE_CHECK
        pNewRoot->CheckTree();
        #endif
        return pNewRoot;
        /*
        TplNode<CItem,CKey>* pNewRoot = m_pRightNode->GetSmallest();
        if ((pNode->m_key) < (pNewRoot->m_key))
        {
            pNewRoot = pNode;
        }
        else
        {
            //BEGIN ADD 01-04-2009 s0032 TDS00037
            m_pRightNode = m_pRightNode->ReleaseNode(pNewRoot->m_key);

#ifdef _DEBUG
            if (m_pRightNode)
            {
                m_pRightNode->CheckTree();
            }
#endif
            if (m_pRightNode)
            {
            //END ADD 01-04-2009 s0032 TDS00037
                m_pRightNode = m_pRightNode->AddNode(pNode);

#ifdef _DEBUG
                m_pRightNode->CheckTree();
#endif
            //BEGIN ADD 01-04-2009 s0032 TDS00037
            }
            else
            {
                m_pRightNode = pNode;
            }
            //END ADD 01-04-2009 s0032 TDS00037
        }

        //BEGIN ADD 01-04-2009 s0032 TDS00038
        //pNode->m_info = m_info;
        pNewRoot->m_pLeftNode = m_pLeftNode;
        pNewRoot->m_pRightNode = m_pRightNode;
        pNewRoot->m_nodeSize = m_nodeSize;
        //END ADD 01-04-2009 s0032 TDS00038
        if (pNewRoot->m_pLeftNode)
        {
            pNewRoot->m_pLeftNode = pNewRoot->m_pLeftNode->AddNode(this);
#ifdef _DEBUG
            pNewRoot->m_pLeftNode->CheckTree();
#endif
        }
        else
        {
            pNewRoot->m_pLeftNode = this;
            //BEGIN ADD 01-04-2009 S0032 TDS00036
            Init();
            //END ADD 01-04-2009 S0032 TDS00036
        }
        pNewRoot->AddLeftNodeNum();
#ifdef _DEBUG
        pNewRoot->CheckTree();
#endif
        return pNewRoot;
        */
    }
    else
    {
        if (m_pRightNode)
        {
            m_pRightNode = m_pRightNode->AddNode(pNode);
        }
        //BEGIN ADD 01-04-2009 S0032 TDS00035
        else
        {
        //END ADD 01-04-2009 S0032 TDS00035
            pNode->m_pParent = this;
            m_pRightNode = pNode;
        }
        AddRightNodeNum();
#ifdef _NEED_TREE_CHECK
        CheckNode();
#endif
        return this;
    }
}

template<class CItem,class CKey>
TplNode<CItem,CKey> *TplNode<CItem,CKey>::ReleaseNode(CKey& key)
{
    //make sure the key does existed!
    //TplNode *pResult = FindNode(key);
    if (CItem::m_key == key)
    {
        // release this node;
        //if this node is the leaf, than just release this node;
        if ((NULL == m_pLeftNode) && (NULL == m_pRightNode))
        {
            UnInit();
            return NULL;
        }
        if (NULL == m_pLeftNode)
        {
            TplNode<CItem,CKey> *pResult = m_pRightNode;
            m_pRightNode->m_pParent = m_pParent;
            //this cann't be null;
            UnInit();
            #ifdef _NEED_TREE_CHECK
            pResult->CheckTree();
            #endif
            return pResult;
        }
        if (NULL == m_pRightNode)
        {
            TplNode<CItem,CKey> *pResult = m_pLeftNode;
            m_pLeftNode->m_pParent = m_pParent;
            UnInit();
            #ifdef _NEED_TREE_CHECK
            pResult->CheckTree();
            #endif
            return pResult;
        }
        //other cases!
        TplNode<CItem,CKey>* pNewRoot = NULL;

        TInt32 balanceNum = m_pLeftNode->GetTreeSize();
        balanceNum -= m_pRightNode->GetTreeSize();

        if (balanceNum < 0)
        {
            pNewRoot = m_pRightNode->GetSmallest();
            m_pRightNode = m_pRightNode->ReleaseNode(pNewRoot->m_key);
#ifdef _NEED_TREE_CHECK
            if (m_pRightNode)
            {
                m_pRightNode->CheckTree();
            }
#endif
            DelRightNodeNum();
        }
        else
        {
            pNewRoot = m_pLeftNode->GetBigest();
            m_pLeftNode = m_pLeftNode->ReleaseNode(pNewRoot->m_key);
#ifdef _NEED_TREE_CHECK
            if (m_pLeftNode)
            {
                m_pLeftNode->CheckTree();
            }
#endif
            DelLeftNodeNum();
        }
        pNewRoot->m_nodeSize = m_nodeSize;
        pNewRoot->m_pParent = m_pParent;
        pNewRoot->m_pLeftNode  = m_pLeftNode;
        pNewRoot->m_pRightNode = m_pRightNode;
        if (m_pLeftNode)
        {
            m_pLeftNode->m_pParent = pNewRoot;
        }
        if (m_pRightNode)
        {
            m_pRightNode->m_pParent = pNewRoot;
        }
        UnInit();
        #ifdef _NEED_TREE_CHECK
        pNewRoot->CheckTree();
        #endif
        return pNewRoot;

        //return the new root of this subtree.
    }
    //otherwist just release the node in the tree and return this itself, because the root is not changed.
    if (CItem::m_key > key)
    {
        //equals to minus left node num.

        // m_pLeftNode must be existed!
        if (NULL == m_pLeftNode)
        {
            //sth wrong!!!
            //cout<<"(TplMap::ReleaseItem)can not find the node!"<<endl;
            #ifdef _NEED_TREE_CHECK
            CheckTree();
        #endif
            return this;
        }
        TplNode<CItem,CKey>*   pNewRoot = this;
        //minus a left node equals to add a right node
        if (NeedRearrange(right_node))
        {
            /*
            #ifdef _DEBUG
            if (m_pRightNode)
            {
                #endif
                pNewRoot = m_pRightNode->GetSmallest();
                m_pRightNode = m_pRightNode->ReleaseNode(pNewRoot->m_key);
                #ifdef _DEBUG
                if (m_pRightNode)
                {
                    m_pRightNode->CheckTree();
                }
                #endif
                DelRightNodeNum();
                pNewRoot->m_pRightNode = m_pRightNode;
                pNewRoot->m_pLeftNode  = m_pLeftNode;
                pNewRoot->m_nodeSize   = m_nodeSize;
                #ifdef _DEBUG
                if (!pNewRoot->m_pLeftNode)
                {
                    TInt32 *p = NULL;
                    *p = 1;
                    cout<<"pNewRoot->m_pLeftNode should not be null!"<<endl;
                }
                else
                {
                #endif
                pNewRoot->m_pLeftNode  = pNewRoot->m_pLeftNode->AddNode(this);
#ifdef _DEBUG
                pNewRoot->m_pLeftNode->CheckTree();
#endif
                pNewRoot->AddLeftNodeNum();
                #ifdef _DEBUG
                }
                #endif


                #ifdef _DEBUG
            }
            else
            {
                TInt32 *p = NULL;
                *p = 1;
                cout<<"release node , something is wrong, the right node should not be null!"<<endl;
            }
            #endif
            */
            if (NULL == m_pRightNode)
            {
                return this;
            }
            //重整树
            TplNode<CItem,CKey>* pNewRoot = m_pRightNode;
            m_pRightNode    = m_pRightNode->m_pLeftNode;
            pNewRoot->m_pLeftNode = this;
            m_nodeSize = 0;
            
            //重整m_pParent
            pNewRoot->m_pParent = m_pParent;
            this->m_pParent = pNewRoot;
            if (m_pRightNode)
            {
                m_pRightNode->m_pParent = this;
            }
            
            if(m_pLeftNode)
            {
                m_nodeSize += m_pLeftNode->GetTreeSize();
            }
            if (m_pRightNode)
            {
                m_nodeSize += m_pRightNode->GetTreeSize();
            }
            pNewRoot->m_nodeSize = 0;
            if (pNewRoot->m_pLeftNode)
            {
                pNewRoot->m_nodeSize += pNewRoot->m_pLeftNode->GetTreeSize();
            }
            if (pNewRoot->m_pRightNode)
            {
                pNewRoot->m_nodeSize += pNewRoot->m_pRightNode->GetTreeSize();
            }
            pNewRoot = pNewRoot->ReleaseNode(key);
            #ifdef _NEED_TREE_CHECK
            pNewRoot->CheckTree();
            #endif
            return pNewRoot;

        }
        pNewRoot->m_pLeftNode = pNewRoot->m_pLeftNode->ReleaseNode(key);
        
        pNewRoot->DelLeftNodeNum();
        #ifdef _NEED_TREE_CHECK
        pNewRoot->CheckTree();
        #endif
        return pNewRoot;
    }
    else
    {
        if (NULL == m_pRightNode)
        {
            //sth wrong!!!
            #ifdef _NEED_TREE_CHECK
            CheckTree();
            #endif
            return this;
        }
        TplNode<CItem,CKey>*   pNewRoot = this;
        //minus a left node equals to add a right node
        if (NeedRearrange(left_node))
        {
            if (NULL == m_pLeftNode)
            {
                //sth wrong!! this should never happened!
                //m_pLeftNode = pNode;
                //DelLeftNodeNum();
                #ifdef _NEED_TREE_CHECK
                CheckNode();
                #endif
                return this;
            }
            //重整树
            TplNode<CItem,CKey>* pNewRoot = m_pLeftNode;
            m_pLeftNode    = m_pLeftNode->m_pRightNode;
            pNewRoot->m_pRightNode = this;
            m_nodeSize = 0;
            //重整m_pParent
            pNewRoot->m_pParent = m_pParent;
            this->m_pParent = pNewRoot;
            if (m_pLeftNode)
            {
                m_pLeftNode->m_pParent = this;
            }
            
            if(m_pLeftNode)
            {
                m_nodeSize += m_pLeftNode->GetTreeSize();
            }
            if (m_pRightNode)
            {
                m_nodeSize += m_pRightNode->GetTreeSize();
            }
            pNewRoot->m_nodeSize = 0;
            if (pNewRoot->m_pLeftNode)
            {
                pNewRoot->m_nodeSize += pNewRoot->m_pLeftNode->GetTreeSize();
            }
            if (pNewRoot->m_pRightNode)
            {
                pNewRoot->m_nodeSize += pNewRoot->m_pRightNode->GetTreeSize();
            }
            pNewRoot = pNewRoot->ReleaseNode(key);
            #ifdef _NEED_TREE_CHECK
            pNewRoot->CheckTree();
            #endif
            return pNewRoot;
        }
        pNewRoot->m_pRightNode = pNewRoot->m_pRightNode->ReleaseNode(key);
        pNewRoot->DelRightNodeNum();
        #ifdef _NEED_TREE_CHECK
        pNewRoot->CheckTree();
        #endif
        return pNewRoot;
    }
    #ifdef _NEED_TREE_CHECK
        CheckTree();
    #endif
    //cout<<"TplMap::ReleaseItem:774 this should never happened!"<<endl;
    return this;
}


//make sure class CKey reload ">" ,"<" and "==";
template<class CItem, class CKey>
class TplMap
{
private:
    TInt32                         m_size;
    TplNode<CItem,CKey>*     m_pItem;
    TplNode<CItem,CKey>*     m_pRear;
    TplNode<CItem,CKey>*     m_pHead;
    TplNode<CItem,CKey>*     m_pTree;
public:
	TplNode<CItem,CKey>*     GetRoot()
	{
		return m_pTree;
	}
public:
    void CheckTree()
    {
        if(m_pTree)
        {
            m_pTree->CheckTree();
        }
    }
	TInt32 GetActivedSize()
	{
		if (m_pTree)
		{
			return (m_pTree->m_nodeSize + 1);
		}
		return 0;
	}
    class Iterator
    {
    private:
        TplNode<CItem,CKey> *m_pNode;
        TplMap              *m_pMap;
    public:
        void        operator ++()
                    {
                        while ((m_pNode + 1) < (m_pMap->m_pItem + m_pMap->m_size))
                        {
                            if ((++m_pNode)->m_pLeftNode != m_pNode)
                            {
                                return;
                            }
                        }
                    }
        CItem*      operator ->()
                    {
                        return m_pNode;
                    }
        bool        operator == (Iterator& itor)
                    {
                        if (itor.m_pNode == m_pNode)
                        {
                            return true;
                        }
                        return false;
                    }
    private:
        Iterator(){};
        ~Iterator(){};
        friend class TplMap<CItem,CKey>;
    };
    friend class Iterator;
private:
public:
    typedef TInt32 (CItem::*STD_PFMSG)(TInt32);
    //Begin是有效的
    TplNode<CItem,CKey> *Begin()
    {
        if (m_pTree)
        {
             return m_pTree->GetSmallest();
        }
        return NULL;
    }
    //end也是有效的
    TplNode<CItem,CKey> *End()
    {
        if (m_pTree)
        {
            return m_pTree->GetBigest();
        }
        return NULL;
    }
    
    void RunOnTree(STD_PFMSG ptr,TInt32 arg)
    {
		if (m_pTree)
		{
			m_pTree->RunOnTree(ptr, arg);
		}
    }
    TplMap(){};
    ~TplMap()
    {
        UnInit();
    }
    TInt32 Init(TInt32 size);
    void UnInit()
    {
        if (m_pItem)
        {
            delete [] m_pItem;
            m_pItem = NULL;
        }

    }
    //first get the Item
    CItem        *GetItem(TInt32& result,CKey* pKey=NULL);

    /*
    Iterator     Begin()
                 {
                     TplMap<CItem,CKey>Iterator itor;
                     itor.m_pMap   = this;
                     itor.m_pNode  = m_pItem;
                     return itor;
                 }

    Iterator     End()
                 {
                     TplMap<CItem,CKey>Iterator itor;
                     itor.m_pMap   = this;
                     itor.m_pNode  = m_pItem + m_size;
                     return itor;
                 }
    */

    CItem        *FindItem(TInt32 idx);
    TInt32          ReleaseItem(CItem * pItem);
    CKey         *GetItemKey(TInt32 idx);

    TInt32      AddInTree(CItem* pItem, CKey* pKey);

    CItem   *GetItemByKey(CKey* pKey);
    TInt32         GetSize()
                {
                    return m_size;
                }

    TInt32         PrintTree()
                {
                    TInt32 count = 0;
                    if (m_pTree)
                    {
                        m_pTree->Print(0,count);
                    }
                    else
                    {
                        cout<<"The tree is empty!"<<endl;
                    }
                    return SUCCESS;
                }
    TBool   IsFull()
            {
                if (m_pHead->m_pRightNode == m_pRear)
                {
                    return TRUE;
                }
                return FALSE;
            }



};

template<class CItem, class CKey>
TInt32 TplMap<CItem,CKey>::Init(TInt32 size)
{

    if (size < 0)
    {
        return INPUT_PARA_ERROR;
    }
    // 0 ~ 1G memory was used as system area.
    //may have warnings, but it's ok.


    m_pItem = new TplNode<CItem,CKey>[size];
    if (NULL == m_pItem)
    {
        return OUT_OF_MEM;
    }
    m_size = size;
    for (TInt32 i=0;i< size;i++)
    {
        //pointer to itself
        (m_pItem + i)->m_pLeftNode  = (m_pItem + i);
        (m_pItem + i)->m_pRightNode = (m_pItem + i + 1);
    }
    (m_pItem + size -1)->m_pRightNode = NULL;
    m_pRear = (m_pItem + size -1);
    m_pHead = m_pItem;
    m_pTree = NULL;
    return SUCCESS;
}

template<class CItem, class CKey>
CItem *TplMap<CItem,CKey>::GetItem(TInt32& result,CKey* pKey)
{
    if (m_pHead->m_pRightNode == m_pRear)
    {
       //out of memory!
       result = OUT_OF_MEM;
       return NULL;
    }
    if (pKey)
    {
        if (GetItemByKey(pKey))
        {
            result = KEY_ALREADY_EXIST;
            return NULL;
        }
    }
    //get a new node;
    TplNode<CItem,CKey>* pResult = m_pHead;
    m_pHead = m_pHead->m_pRightNode;
    pResult->Init();

    if (pKey)
    {
        pResult->m_key = *pKey;
        if (m_pTree)
        {
            m_pTree = m_pTree->AddNode(pResult);
        }
        else
        {
            m_pTree = pResult;
        }
    }
    result = SUCCESS;
    return pResult;
}





template<class CItem, class CKey>
CItem *TplMap<CItem,CKey>::FindItem(TInt32 idx)
{
    if ((idx > (m_size - 1))||(idx < 0))
    {
        return NULL;
    }
    return m_pItem + idx;
}

template<class CItem, class CKey>
TInt32 TplMap<CItem,CKey>::ReleaseItem(CItem * pItem)
{
    if (NULL == pItem)
    {
        return NULL_POINTER;
    }
    //we do not use dynamic_cast to check if pItem belongs to this list, because the CItem has virtual func, otherwise it wouldn't has v_table.
    TplNode<CItem,CKey> *pNew = static_cast< TplNode<CItem,CKey>* >(pItem);

    TInt32 offset = ((char*)pNew - (char*)m_pItem);


    if ((pNew >= (m_pItem + m_size))||(offset < 0)||(0 != (offset % sizeof( TplNode<CItem,CKey>* ))))
    {
        return NOT_BELONG_TO_THIS_CAPSULA;
    }
    //delete the pItem in the tree;
    if (m_pTree)
    {
        TplNode<CItem,CKey> *pInTree = m_pTree->FindNode(pNew->m_key);
        //make sure it is the one!
        if(pInTree == pNew)
        {
            //BEGIN ADD 01-04-2009 S0032 TDS00035
            //m_pTree->ReleaseNode(pNew->m_key);
            m_pTree = m_pTree->ReleaseNode(pNew->m_key);
            //END ADD 01-04-2009 S0032 TDS00035
        }
        else
        {
            return NOT_BELONG_TO_THIS_CAPSULA;
        }
    }
    //force upper casting, because we know it's ok.
    m_pRear->m_pRightNode= pNew;
    pNew->m_pRightNode = NULL;
    pNew->m_pLeftNode  = pNew;
    m_pRear = pNew;
    return SUCCESS;
}

template<class CItem, class CKey>
TInt32 TplMap<CItem,CKey>::AddInTree(CItem* pItem, CKey* pKey)
{
    if ((NULL == pItem) || (NULL == pKey))
    {
        return NULL_POINTER;
    }

    TplNode<CItem,CKey> *pNew = static_cast< TplNode<CItem,CKey>* >(pItem);
    TInt32 offset = ((char*)pNew - (char*)m_pItem);


    if ((offset < 0)||(0 != (offset % sizeof( TplNode<CItem,CKey>* ))))
    {
        return NOT_BELONG_TO_THIS_CAPSULA;
    }
    CItem *pExist = GetItemByKey(pKey);
    if (pExist)
    {
        if (pExist = pNew)
        {
            return ITEM_ALREADY_IN_THE_TREE;
        }
        return KEY_ALREADY_EXIST;
    }
    pNew->m_key = *pKey;
    if (m_pTree)
    {
        m_pTree =  m_pTree->AddNode(pNew);
    }
    else
    {
        m_pTree = pNew;
    }
    return SUCCESS;
}

template<class CItem, class CKey>
CItem *TplMap<CItem,CKey>::GetItemByKey(CKey* pKey)
{
    if ((NULL == pKey) || (NULL == m_pTree))
    {
        return NULL;
    }
    return m_pTree->FindNode(*pKey);
}

}
#endif
