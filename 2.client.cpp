#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "Socket.h"
#include "common/proto.h"
#include "proto/message_define.pb.h"
#define GOTO_NEXT if (ret == 0) {phase++;}
#define GOTO_LOGIN if (ret == 0) {phase = 5;}
#define GET_OUT if (ret == 0) {phase = 999;}
#define WAIT_RSP_GO_NEXT ret = RecvRsp(); GOTO_NEXT
#define WAIT_RSP_GO_LOGIN ret = RecvRsp(); GOTO_LOGIN
#define WAIT_RSP_GET_OUT ret = RecvRsp(); GET_OUT
#define CHECK_SERVER_ON if (ret < 0) {\
                            printf("Server Not Ready\n");\
                            return 0;\
                        }
                        
#define SEND common_req.SerializeToArray(sock.send_buffer, common_req.ByteSize());\
            int ret = sock.SocketSend(common_req.ByteSize());\
            send_count++;\
            return ret;
#define CHECK_SEND need_send = check_need_send(time0_now, time0_last); \
                    if (need_send == 0) {break;}\
                    update(time0_last);

int send_count = 0;
int recv_count = 0;

ssp::CommonReq common_req;
ssp::CommonRsp common_rsp;

SspSocket sock;

int Register(const char *user_name, const char *password) {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(REG_REQ);
    common_req.mutable_reg_req()->set_user_name(user_name);
    common_req.mutable_reg_req()->set_password(password);
    common_req.mutable_reg_req()->set_from(1);
    SEND
}

int Login(const char *user_name, const char *password) {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(LOGIN_REQ);
    common_req.mutable_login_req()->set_user_name(user_name);
    common_req.mutable_login_req()->set_password(password); 
    SEND
}

int AddFriend(int user_id, int other_id) {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(ADD_FRIEND_REQ);
    common_req.mutable_add_friend_req()->set_user_id(user_id);
    common_req.mutable_add_friend_req()->set_other_id(other_id);
    SEND
}

int DelFriend(int user_id, int other_id) {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(DEL_FRIEND_REQ);
    common_req.mutable_add_friend_req()->set_user_id(user_id);
    common_req.mutable_del_friend_req()->set_other_id(other_id);
    SEND
}

int PublishMessage(int user_id, const char *content) {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(PUBLISH_MESSAGE_REQ);
    common_req.mutable_publish_message_req()->set_user_id(user_id);
    common_req.mutable_publish_message_req()->set_content(content);
    SEND
}

int GetPhoto(int user_id) {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(GET_PHOTO_REQ);
    common_req.mutable_get_photo_req()->set_user_id(user_id);
    SEND
}

int GetMessageList(int user_id) {   
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(GET_MESSAGE_LIST_REQ); 
    common_req.mutable_get_message_list_req()->set_user_id(user_id);
    SEND
}

int ShoutDown() {
    common_req.Clear();
    common_req.mutable_header()->set_ver(1);
    common_req.mutable_header()->set_cmd_type(SERVER_SHUTDOWN);
    common_req.SerializeToArray(sock.send_buffer, common_req.ByteSize());
    int ret = sock.SocketSend(common_req.ByteSize());
    return ret;
}

int saved_user_id1 = 0;
int saved_user_id2 = 0;

int RecvRsp() {
    int message_count = 0;
    int ret = sock.SocketCheckRecv();
    if (ret > 0) {
        common_rsp.ParseFromArray(sock.recv_buffer, 10240);
        int cmd_type = common_rsp.header().cmd_type();
        recv_count++;
        if (recv_count > 0) return 0;
        switch (cmd_type) {
            case REG_RSP:
                printf("Recvive Reg Rsp\n");
                printf("ret     : %d\n", common_rsp.reg_rsp().ret());
                printf("user_id : %d\n", common_rsp.reg_rsp().user_id());
                break;

            case LOGIN_RSP:
                printf("Recvive Login Rsp\n");
                printf("ret     : %d\n", common_rsp.login_rsp().ret());
                printf("user_id: %d\n", common_rsp.login_rsp().user_id());
                if (saved_user_id1 == 0) {
                    saved_user_id1 = common_rsp.login_rsp().user_id();
                } else {
                    if (saved_user_id2 == 0) {
                        saved_user_id2 = common_rsp.login_rsp().user_id();
                    }
                }
                break;

            case ADD_FRIEND_RSP:
                printf("Recvive AddFriend Rsp\n");
                printf("ret     :%d\n", common_rsp.add_friend_rsp().ret());
                break;

            case DEL_FRIEND_RSP:
                printf("Receive DelFriend Rsp\n");
                printf("ret     :%d\n", common_rsp.del_friend_rsp().ret());
                break;

            case PUBLISH_MESSAGE_RSP:
                printf("Receive PublishMessage Rsp\n");
                printf("ret     :%d\n", common_rsp.publish_message_rsp().ret());
                break;

            case GET_PHOTO_RSP:
                printf("Recvive GetPhoto Rsp\n");
                printf("ret    :%d\n", common_rsp.get_photo_rsp().ret());
                printf("updata :%d\n", common_rsp.get_photo_rsp().last_publisher_id());
                break;

            case GET_MESSAGE_LIST_RSP:
                printf("Recvive GetMessageList Rsp\n");
                printf("ret     :%d\n", common_rsp.get_message_list_rsp().ret());
                message_count = common_rsp.get_message_list_rsp().message_list_size();
                printf("count   :%d\n", message_count);
                for (int i = 0; i < message_count; i++) {
                    printf("%6d:%s\n", common_rsp.get_message_list_rsp().message_list(i).publisher_id(),
                                        common_rsp.get_message_list_rsp().message_list(i).content().c_str());
                }
                break;

            default:
                break;
        }
    } else {
        return -1;  
    }
    return 0;
}

int delay = 0;

int update(struct timeval &tim) {
    tim.tv_usec = tim.tv_usec + delay;
    if (tim.tv_usec > 1000000) {
        tim.tv_usec = tim.tv_usec - 1000000;
        tim.tv_sec = tim.tv_sec + 1;
    }
    return 0;
}

int check_need_send(struct timeval t0, struct timeval t1) {
    int sec = t0.tv_sec - t1.tv_sec;
    int usec = t0.tv_usec - t1.tv_usec;
    int delta = sec * 1000 + usec / 1000;
    if (delta > delay) {
        return 1;
    }
    return 0;
}

int Reg10000() {
    int ret = 0;
    for (int i = 10000; i < 20000; i++) {
        char user_name[16];
        sprintf(user_name, "hank%d", i);
        ret = Register(user_name, "1234567");
        if (ret == 0) {
            do {
                ret = RecvRsp();
            } while (ret == -1);
        }
    }
    return 0;
}

int main() {
    int ret;
    ret = sock.Init();
    ret = sock.ClientSockInit();
    CHECK_SERVER_ON
    int client_on = 1;
    int phase = 1;
    int count_down = 40;
    int time_begin = time(NULL);
    int send_per_second = 10000;
    delay = 1000000 / send_per_second;
    struct timeval time0_last; 
    gettimeofday(&time0_last, NULL);
    // Reg10000();
    // return 0; 
    struct timeval time0_now;
    while (client_on) {
        gettimeofday(&time0_now, NULL);
        int time_now = time(NULL);
        if (time_now > time_begin) {
            time_begin = time_now;
            count_down--;
            if (count_down <= 0) {
                phase = 999;
            }
            printf("--- send %d recv %d\n",send_count, recv_count);
            send_count = 0;
            recv_count = 0;
        }
        // printf("----[debug]phase=%d\n", phase);
        int need_send = 0;
        switch (phase) {
            case 1:
                CHECK_SEND
                ret = Register("hank1234", "12345678");
                GOTO_NEXT           
                break;
            case 2:
                WAIT_RSP_GO_NEXT
                break;
            case 3:
                CHECK_SEND
                ret = Register("hank5678", "88888888");
                GOTO_NEXT
                break;
            case 4:
                // WAIT_RSP_GET_OUT
                // GET_OUT 
                WAIT_RSP_GO_NEXT
                break;
            case 5:
                CHECK_SEND
                ret = Login("hank14567", "1234567");
                GOTO_NEXT
                break;
            case 6:
                WAIT_RSP_GO_NEXT
                break;
            case 7:
                CHECK_SEND
                ret = Login("hank15678", "1234567");
                GOTO_NEXT
                break;
            case 8:
                WAIT_RSP_GO_NEXT
                break;
            case 9:
                CHECK_SEND
                ret = AddFriend(saved_user_id1, saved_user_id2);
                GOTO_NEXT
                break;
            case 10:
                WAIT_RSP_GO_NEXT
                break;
            case 11:
                CHECK_SEND
                ret = PublishMessage(saved_user_id1, "Hank Is Here");
                GOTO_NEXT
                break;
            case 12:
                WAIT_RSP_GO_NEXT
                break;
            case 13:
                CHECK_SEND
                ret = GetPhoto(saved_user_id2);
                GOTO_NEXT
                break;
            case 14:
                WAIT_RSP_GO_NEXT
                break;
            case 15:
                CHECK_SEND
                ret = GetMessageList(saved_user_id2);
                GOTO_NEXT
                break;
            case 16:
                WAIT_RSP_GO_NEXT
                break;
            case 17:
                CHECK_SEND
                ret = DelFriend(saved_user_id1, saved_user_id2);
                GOTO_NEXT
                break;
            case 18:
                WAIT_RSP_GO_LOGIN
                // WAIT_RSP_GO_NEXT
                break;
            default:
                ret = ShoutDown();
                GOTO_NEXT
                client_on = 0;
                break;
        }
    }
    sock.ClientClose();
    return 0;
}