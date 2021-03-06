/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_RPC_ABSTRACT_RPC_SYNC_CLIENT_H
#define FLYINGKV_RPC_ABSTRACT_RPC_SYNC_CLIENT_H

#include <memory>

#include "../sys/timer.h"
#include "../sys/spin-lock.h"
#include "../common/common-def.h"
#include "../common/resource-pool.h"
#include "../utils/protobuf-utils.h"
#include "exceptions.h"

#include "abstract-rpc-client.h"

#define DefineStandardSyncRpcWithNoMsgId(RpcName)                                                                   \
    std::shared_ptr<protocol::RpcName##Response> RpcName(common::SP_PB_MSG req, net::net_peer_info_t &&peer)

#define ImplStandardSyncRpcWithNoMsgId(ClassName, RpcName)                                                         \
std::shared_ptr<protocol::RpcName##Response>                                                                       \
    ClassName::RpcName(common::SP_PB_MSG req, net::net_peer_info_t &&peer) {                                          \
        auto sspNM = sendMessage(#RpcName, std::move(req), std::move(peer));                                       \
        if (!sspNM) {                                                                                              \
            throw rpc::RpcClientIsBusyException();                                                                      \
        }                                                                                                          \
                                                                                                                   \
        auto *mnm = dynamic_cast<net::MessageNotifyMessage*>(sspNM.get());                                         \
        auto rm = mnm->GetContent();                                                                               \
        auto RpcName##Resp__Impl_DEF_TMP = new protocol::RpcName##Response();                                      \
        auto buf = rm->GetDataBuffer();                                                                            \
        buf->MoveHeadBack(sizeof(rpc::HandlerType));                                                               \
        utils::ProtoBufUtils::Deserialize(buf, RpcName##Resp__Impl_DEF_TMP);                                      \
        return std::shared_ptr<protocol::RpcName##Response>(RpcName##Resp__Impl_DEF_TMP);                          \
    }

#define DefineStandardSyncRpcWithMsgId(RpcName)                                                                                 \
    std::shared_ptr<protocol::RpcName##Response> RpcName(net::Message::Id id, common::SP_PB_MSG req, net::net_peer_info_t &&peer)

#define ImplStandardSyncRpcWithMsgId(ClassName, RpcName)                                                           \
std::shared_ptr<protocol::RpcName##Response>                                                                       \
    ClassName::RpcName(net::Message::Id id, common::SP_PB_MSG req, net::net_peer_info_t &&peer) {                     \
        auto sspNM = sendMessage(#RpcName, id, std::move(req), std::move(peer));                                   \
        if (!sspNM) {                                                                                              \
            throw rpc::RpcClientIsBusyException();                                                                      \
        }                                                                                                          \
                                                                                                                   \
        auto *mnm = dynamic_cast<net::MessageNotifyMessage*>(sspNM.get());                                         \
        auto rm = mnm->GetContent();                                                                               \
        auto RpcName##Resp__Impl_DEF_TMP = new protocol::RpcName##Response();                                      \
        auto buf = rm->GetDataBuffer();                                                                            \
        buf->MoveHeadBack(sizeof(rpc::HandlerType));                                                               \
        utils::ProtoBufUtils::Deserialize(buf, RpcName##Resp__Impl_DEF_TMP);                                      \
        return std::shared_ptr<protocol::RpcName##Response>(RpcName##Resp__Impl_DEF_TMP);                          \
    }


namespace flyingkv {
namespace net {
class NotifyMessage;
}
namespace rpc {
class ARpcSyncClient : public ARpcClient {
PRIVATE
    class RpcCtx {
    PUBLIC
        enum class State {
            Ok          = 0,
            Timeout     = 1,
            BrokenPipe  = 2
        };
    PUBLIC
        RpcCtx() {
            cv = new std::condition_variable();
            mtx = new std::mutex();
        }

        ~RpcCtx() {
            DELETE_PTR(mtx);
            DELETE_PTR(cv);
        }

        void Release() {
            ssp_nm.reset();
        }

    PUBLIC
        std::condition_variable             *cv        = nullptr;
        std::mutex                          *mtx       = nullptr;
        net::net_peer_info_t                 peer;
        uint64_t                             msgId     = 0;
        bool                                 complete  = false;
        State                                state     = State::Ok;
        sys::Timer::EventId                  timer_ev;
        std::shared_ptr<net::NotifyMessage>  ssp_nm;
    };

PUBLIC
    ARpcSyncClient(net::ISocketService *ss, const sys::cctime &timeout, sys::MemPool *memPool = nullptr) :
                    rpc::ARpcClient(ss, memPool), m_timeout(timeout) {}

PROTECTED
    std::shared_ptr<net::NotifyMessage> recvMessage(RpcCtx *rc);
    void onRecvMessage(std::shared_ptr<net::NotifyMessage> sspNM) override;
    /**
     * @param rpcName
     * @param msg
     * @return 如果get()为null则失败，否则成功。
     */
    std::shared_ptr<net::NotifyMessage> sendMessage(std::string &&rpcName, common::SP_PB_MSG msg, net::net_peer_info_t &&peer);

    /**
     * @param rpcName
     * @param msg
     * @return 如果get()为null则失败，否则成功。
     */
    std::shared_ptr<net::NotifyMessage> sendMessage(std::string &&rpcName, net::Message::Id id, common::SP_PB_MSG msg, net::net_peer_info_t &&peer);

PRIVATE
    common::ResourcePool<RpcCtx>                                   m_rpcCtxPool = common::ResourcePool<RpcCtx>(200);
    std::unordered_map<uint64_t, RpcCtx*>                          m_hmapRpcCtxs;
    std::unordered_map<net::net_peer_info_t, std::set<RpcCtx*>>    m_hmapPeerRpcs;
    sys::spin_lock_t                                               m_slRpcCtxs = UNLOCKED;
    sys::cctime                                                    m_timeout;
};
}
}

#endif //FLYINGKV_RPC_ABSTRACT_RPC_SYNC_CLIENT_H
