/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_SERVER_RF_NODE_H
#define FLYINGKV_SERVER_RF_NODE_H

#include "../../common/iservice.h"
#include "../../rpc/rpc-server-sync.h"
#include "../../rpc/common-def.h"

namespace flyingkv {
namespace common {
class IKVOperator;
}

namespace server {
class KVRpcServerSync : public common::IService {
PUBLIC
    KVRpcServerSync(common::IKVOperator *handler, uint16_t workThreadsCnt, uint16_t netIOThreadCnt,
                    uint16_t port, sys::MemPool *memPool = nullptr);
    ~KVRpcServerSync() override;

    bool Start() override;
    bool Stop() override;

PRIVATE
    void register_rpc_handlers();
    common::SP_PB_MSG on_put(common::SP_PB_MSG sspMsg);
    /**
     * for rpc lib to deserialize client request.
     * @return
     */
    common::SP_PB_MSG create_put_request();
    common::SP_PB_MSG on_get(common::SP_PB_MSG sspMsg);
    common::SP_PB_MSG create_get_request();
    common::SP_PB_MSG on_delete(common::SP_PB_MSG sspMsg);
    common::SP_PB_MSG create_delete_request();
    common::SP_PB_MSG on_scan(common::SP_PB_MSG sspMsg);
    common::SP_PB_MSG create_scan_request();

    void on_recv_net_message(std::shared_ptr<net::NotifyMessage> sspNM);

PRIVATE
    bool                      m_bStopped        = true;
    uint16_t                  m_iIOThreadsCnt   = 0;
    rpc::RpcServerSync       *m_pRpcServer      = nullptr;
    common::IKVOperator      *m_pHandler        = nullptr;/*关联关系，无需释放*/
    net::ISocketService      *m_pSocketService  = nullptr;
    bool                      m_bOwnMemPool     = false;
    sys::MemPool             *m_pMemPool        = nullptr;
};
} // namespace server
} // namespace flyingkv

#endif //FLYINGKV_SERVER_RF_NODE_H
