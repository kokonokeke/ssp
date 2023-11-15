#include "MessageManager.h"
#include "common/ret_value.h"
#include <vector>
#include <map>

void MessageManager::Start() {
    //todo
    //get message from db
    set_cur_message_id(100001);
}

void MessageManager::Proc() {
    
}

void MessageManager::Shutdown() {
    
}

void MessageManager::Restart() {
    
}

MessageInfo *MessageManager::GetMessage(int user_id, int message_id) {
    for (int i = 0; i < message_count_; i++) {
        if (messages_[i].user_id() == user_id && messages_[i].message_id() == message_id) {
            return &messages_[i];
        }
    }
    return NULL;
}

MessageInfo *MessageManager::GetMessage(int message_id) {
    for (int i = 0; i < message_count_; i++) {
        if (messages_[i].message_id() == message_id) {
            return &messages_[i];
        }
    }
    return NULL;
}

int MessageManager::PublishMessage(MessageInfo message) {
       // 检查是否达到消息数组的最大容量
    if (message_count_ == 10239) {
        return MESSAGE_TOO_MUCH; // 消息数组已满，无法添加新消息
    }
    int ret_mess_id = cur_message_id();
    messages_[message_count_].set_message_id(cur_message_id());
    messages_[message_count_].set_user_id(message.user_id());
    messages_[message_count_].set_publish_time(message.publish_time());
    cur_message_id_++;
    message_count_++;
    return ret_mess_id; // 成功添加消息
}


int MessageManager::DeleteMessage(int message_id) {
    for (int i = 0; i < message_count_; i++) {
        if (messages_[i].message_id() == message_id) {
            messages_[i].set_message_id(messages_[message_count_ - 1].message_id());
            messages_[i].set_user_id(messages_[message_count_ - 1].user_id());
            messages_[i].set_publish_time(messages_[message_count_ - 1].publish_time());
            messages_[i].set_content(messages_[message_count_ - 1].content());
            message_count_--;
            return SUCCESS;
        }
    }
    return MESSAGE_NOT_EXIST; // 未找到匹配的消息
}

std::vector<int> MessageManager::GetMessageIds(int user_id) {
   std::map<int, std::vector<int>> ::iterator iter;
   iter = user_messages_.find(user_id);
   if (iter != user_messages_.end()) {
    return iter->second;
   }
   return std::vector<int>();
}

int MessageManager::PushUserMessageId(int user_id, int message_id) {
    std::map<int, std::vector<int>> ::iterator iter;
    iter = user_messages_.find(user_id);
    if (iter == user_messages_.end()) {
        std::vector<int> mess_ids;
        mess_ids.push_back(message_id);
        user_messages_.insert(std::pair<int, std::vector<int>> (user_id, mess_ids));
        return 1;
    }
    iter->second.push_back(message_id);
    return 0;
}