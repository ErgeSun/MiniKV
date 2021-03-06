/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_RPC_SERVER_H
#define FLYINGKV_RPC_SERVER_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "../common/iservice.h"

#include "../net/notify-message.h"
#include "../common/blocking-queue.h"
#include "../sys/thread-pool.h"

#include "imessage-handler.h"

// TODO(sunchao): fix rpc to use protobuf service. Now we just use protobuf data serialize feature.
namespace flyingkv {
namespace sys {
class MemPool;
}
namespace net {
class ISocketService;
}
namespace rpc {
class IRpcHandler;
/**
 * [Message format]
 *    client -> server :  |net common header(Message::HeaderSize() bytes)|[handler id(2bytes)|protobuf msg(n bytes)]|
 *    server -> client :  |net common header(Message::HeaderSize() bytes)|[rpc code(2bytes)|[handler id(2bytes)|protobuf msg(n bytes or 0 if no return value)]|
 */
class RpcServerSync : public common::IService, public IMessageHandler {
PUBLIC
    /**
     *
     * @param workThreadsCnt
     * @param ss
     * @param memPool 如果为空，则内部自己构造。
     */
    RpcServerSync(uint16_t workThreadsCnt, net::ISocketService *ss, sys::MemPool *memPool = nullptr);
    ~RpcServerSync() override;

    bool Start() override;
    bool Stop() override;

    bool RegisterRpc(uint16_t id, IRpcHandler *handler);
    void FinishRegisterRpc();

    void HandleMessage(std::shared_ptr<net::NotifyMessage> sspNM) override;

PRIVATE
    void proc_msg(std::shared_ptr<net::NotifyMessage> sspNM);

PRIVATE
    bool                                                        m_bStopped              = true;
    uint16_t                                                    m_iWorkThreadsCnt       = 0;
    // 关联指针，无需本类释放。
    net::ISocketService                                        *m_pSocketService        = nullptr;
    bool                                                        m_bOwnMemPool           = false;
    sys::MemPool                                               *m_pRpcMemPool           = nullptr;
    sys::ThreadPool<std::shared_ptr<net::NotifyMessage>>       *m_pWorkThreadPool       = nullptr;
    std::unordered_map<uint16_t, IRpcHandler*>                  m_hmHandlers;
    uint16_t                                                    m_iPort;
    bool                                                        m_bRegistered           = false;
};
}
}

#endif //FLYINGKV_RPC_SERVER_H
