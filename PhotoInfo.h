#ifndef _PHOTOINFO_H

#define _PHOTOINFO_H

#include "common/macro.h"

class PhotoInfo {
private:
    GETSETVAR(int, user_id)
    GETSETVAR(int, last_publisher_id)   
    GETSETVAR(int, last_publish_time)
    GETSETVAR(int, last_publish_message_id)
};

#endif // !__PHOTO_INFO_H__