#ifndef __ZEPHYR_GATEWAY_SERVICE_H__
#define __ZEPHYR_GATEWAY_SERVICE_H__

#include "Public/include/TypeDef.h"
#include "Public/include/SysMacros.h"
#include "Public/Orb/include/Service.h"
#include "../../Interface/Interface/IfGateway.h"
#include "Public/include/IdlHeaderFile.h"
#include "Public/Interface/Platform/include/IfNet.h"
#include "Public/tpl/include/TplMultiMap.h"
#include "Public/tpl/include/TplPool.h"
#include "Route.h"
namespace Zephyr
{
class CGatewayService : public CService,
                        public IfGatewaySvc
{
private:
    IfNet   *m_pNet;
    TUInt32 m_uIp;
    TUInt16 m_uListeningPort;
    TUInt16 m_nMaxConnections;

    CPool<TplMultiKeyMapNode<CRoute,TUInt32> > m_tListeningPool;

public:
    CPool<TplMultiKeyMapNode<CRoute,TUInt32> > *GetRoutePool()
    {
        return &m_tListeningPool;
    }
    DECALRE_HANDLE_INTERFCE;

    //ͬ����Ϣ.
    virtual TInt32 Syn2Map(TUInt32 uFrom,TLV<TUInt8,TUInt16>& tTLV);
    //ע�����
    virtual TInt32 RegisterService(TUInt32 uServiceId,OctSeq<TUInt16>& tServiceName);
    //ע������
    virtual TInt32 UnRegisterService(TUInt32 uServiceId);
    //���͹㲥������Ϣ
    virtual TInt32 BroadcastTeamMsg(TUInt32 uTeam,OctSeq<TUInt16>& tServiceName);


    //������Serviceר�е�.
    virtual TInt32      OnInit();
    //�����ǻص�.
    virtual TInt32      OnFinal();


    //��ʱ  ����ʱ��
    virtual TInt32  OnTimer(TInt32 nTimerIdx,void *pData,TInt32 nTimeGap,TUInt32 nTimeNow);

    //�����Ǹ�Serviceʹ�õ�
    //���ڵĻص�������ע��ѭ��ʱ�䣬��ֻ����һ��
    virtual TInt32  OnRoutine(TUInt32 nRunCnt);
    //����ʱ��
    virtual TInt32  OnNetEvent(CConnectionEvent *pEvent);

    //��ʼ����
    void StartLogin(TChar *pIp,TUInt16 nListeningPort,TUInt16 nMaxConnection);
    //ֹͣ����
    void StopLogin();
};



#ifdef WIN32

extern "C" __declspec( dllexport ) CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr);

extern "C" __declspec( dllexport ) TInt32 ReleaseService(CService *);

#else

extern "C" CService *InitService(IfOrb* pStubCenter,IfTaskMgr *pIfTaskMgr,IfLoggerManager *pIfLoggerMgr);
extern "C" TInt32 ReleaseService(CService *);

#endif



}



#endif
