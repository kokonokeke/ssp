#ifndef _PHOTOMANAGER_H

#define _PHOTOMANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PhotoInfo.h"
#include "common/ret_value.h"

class PhotoManager {
private:
    PhotoInfo photos_[10240];
    GETSETVAR(int, photo_count)
public:
    int Start();
    int Proc();
    int Shutdown();
    int Restart();

public:
    int CreatePhoto(int user_id);
    PhotoInfo *GetPhoto(int user_id);
    int UpdatePhoto(int user_id, int publisher_id, int publish_time, int publish_message_id);
};

#endif // !__PHOTO_MANAGER_H__