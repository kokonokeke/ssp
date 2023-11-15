#ifndef _MESSAGEINFO_H
#define _MESSAGEINFO_H

#include "common/macro.h"
#include <string.h>

class MessageInfo {
    GETSETVAR(int, user_id)
    GETSETVAR(int, message_id)
    GETSETVAR(int, publish_time)
    GETSETSTR(1024, content)
};

#endif // !__MESSAGE_INFO_H__