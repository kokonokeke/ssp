#include "PhotoManager.h"

int PhotoManager::Start()
{
    // printf("PhotoManager Start\n");
    return SUCCESS;
}

int PhotoManager::Proc()
{
    // printf("PhotoManager Proc\n");
    return SUCCESS;
}

int PhotoManager::Shutdown()
{
    // printf("PhotoManager Shutdown\n");
    return SUCCESS;
}

int PhotoManager::Restart()
{
    // printf("PhotoManager Restart\n");
    return SUCCESS;
}

int PhotoManager::CreatePhoto(int user_id) {
    for (int i = 0; i < photo_count_; i++) {
        if (photos_[i].user_id() == user_id) {
            return PHOTO_EXIST;
        }
    }
    if (photo_count_ == 10239) {
        return PHOTO_TOO_MUCH;
    }
    photos_[photo_count_].set_user_id(user_id);
    photos_[photo_count_].set_last_publisher_id(0);
    photo_count_++;
    return SUCCESS;
}



PhotoInfo *PhotoManager::GetPhoto(int user_id)
{
    for (int i = 0; i < photo_count_; i++)
    {
        if (photos_[i].user_id() == user_id)
        {
            return &photos_[i];
        }
    }
    return nullptr;
}

int PhotoManager::UpdatePhoto(int user_id, int publisher_id, int publish_time, int publish_message_id)
{
    PhotoInfo *photo = GetPhoto(user_id);
    if (photo == nullptr)
    {
        printf("photo is null\n");
        return PHOTO_NOT_EXIST;
    }
    // photo->set_user_id(user_id); // not need
    printf("update %d publisher %d\n", user_id, publisher_id);
    photo->set_last_publisher_id(publisher_id);
    // photo->set_last_publish_time(publish_time);
    // photo->set_last_publish_message_id(publish_message_id);
    return SUCCESS;
}