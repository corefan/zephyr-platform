#ifndef __ERP_PLATFORM_JOB_INFO_H__
#define __ERP_PLATFORM_JOB_INFO_H__

#include "Public/include/TypeDef.h"

namespace erp_platform
{

    enum EnJobMenuId
    {
        en_enum2_root_menu                  = 0,     //有更改配置的权限
        en_enum3_setting_job_info           = 1,     //第一个工作就是配置流程名字.
    };
    

    struct TJobInfos
    {
        TUInt32 m_uMenuId; //平台名
        TUInt32 m_uLvl;
        TUInt32 m_uFatherMenuId;
        TChar   *m_pszNames; //名字
        //         bool  operator=(const TJobInfos &rRValue)
        //         {
        //             m_uPlatform = rRValue.m_uPlatform;
        //             m_uJobId = rRValue.m_uJobId;
        //             m_pszNames = rRValue.m_pszNames;
        //         }
        //         bool  operator==(const TJobInfos &rRvalue) const
        //         {
        //             return ((m_uPlatform == rRvalue.m_uPlatform)&&(m_uJobId == rRvalue.m_uJobId));
        //         }
        //         bool  operator>(const TJobInfos &rRvalue) const 
        //         {
        //             if (m_uPlatform == rRvalue.m_uPlatform)
        //             {
        //                 return (m_uJobId > rRvalue.m_uJobId);
        //             }
        //             return (m_uPlatform>rRvalue.m_uPlatform);
        //         }
        //         bool  operator<(const TJobInfos &rRvalue) const 
        //         {
        //             if (m_uPlatform == rRvalue.m_uPlatform)
        //             {
        //                 return (m_uJobId < rRvalue.m_uJobId);
        //             }
        //             return (m_uPlatform<rRvalue.m_uPlatform);
        //         }
    };
}

#endif
