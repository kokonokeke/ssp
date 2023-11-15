#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BusManager.h"
#include "proto/message_define.pb.h"
#include "UserManager.h"
#include "common/proto.h"
#include "common/ret_value.h"
#include "DbManager.h"  

BusManager bus;
UserManager user_svr;
DbManager db_svr;
int user_svr_id = 0;//1 or 2
ssp::ServerToUserReq inner_req;
ssp::UserToServerRsp inner_rsp;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: user_svr <svr_id>");
        return 0;
    }
    sscanf(argv[1], "%d", &user_svr_id);
    db_svr.Init();
    bus.Init(); 
    user_svr.Start(&db_svr);
    int server_on = 1;
    while (server_on) {
        int check_bus = bus.CheckRecv(user_svr_id);
        if (check_bus != -1) {
            inner_req.ParseFromArray(bus.Recv(check_bus, user_svr_id), bus.RecvSize(check_bus, user_svr_id));
            int cmd_id = inner_req.header().cmd_type();
            switch (cmd_id) {
                case GET_USER_ID_REQ:
                {
                    int user_id = user_svr.GetUserIdByUserName(inner_req.get_user_id().user_name().c_str());
                    inner_rsp.mutable_header()->set_ver(1);
                    inner_rsp.mutable_header()->set_cmd_type(cmd_id + 1);
                    if (user_id == USER_EXIST) {
                        inner_rsp.mutable_get_user_id()->set_ret(USER_EXIST);
                    } else {
                        inner_rsp.mutable_get_user_id()->set_ret(USER_NOT_EXIST);
                        inner_rsp.mutable_get_user_id()->set_user_id(user_svr.cur_user_id());                   
                    }
                    char data[10240];
                    inner_rsp.SerializeToArray(data, inner_rsp.ByteSize());
                    bus.Send(user_svr_id, check_bus, data, inner_rsp.ByteSize());
                }
                break;
                default:
                break;
            }

        }
    }
    return 0;
}
