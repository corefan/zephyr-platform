#ifndef __TEST_LOGIC_TEST_LOGIC_LOGGER_DEF_H__
#define __TEST_LOGIC_TEST_LOGIC_LOGGER_DEF_H__

namespace test_logic
{

enum EnTestLogicLogger
{
    en_service_start = 3 * 1000,
    en_service_start_rslt,
    en_service_create_session,
    en_service_create_session_failed,
    en_service_release_session,

    en_service_stoping,
};

}

#endif