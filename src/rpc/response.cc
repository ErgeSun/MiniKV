/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#include <google/protobuf/message.h>

#include "response.h"
#include "../utils/protobuf-utils.h"

namespace flyingkv {
namespace rpc {
uint32_t RpcResponse::getDerivePayloadLength() {
    auto baseSize = RpcResponseBase::getDerivePayloadLength();
    if (m_pMsg) {
        return baseSize + m_pMsg->ByteSize();
    }

    return baseSize;
}

void RpcResponse::encodeDerive(common::Buffer *b) {
    ByteOrderUtils::WriteUInt16(b->GetPos(), (RpcCodeType)m_code);
    b->MoveHeadBack(sizeof(RpcCodeType));
    ByteOrderUtils::WriteUInt16(b->GetPos(), m_ht);
    b->MoveHeadBack(sizeof(HandlerType));
    if (m_pMsg) {
        utils::ProtoBufUtils::Serialize(m_pMsg.get(), b);
    }
}

uint32_t RpcResponseBase::getDerivePayloadLength() {
    return sizeof(RpcCodeType) + sizeof(HandlerType);
}

void RpcResponseBase::encodeDerive(common::Buffer *b) {
    ByteOrderUtils::WriteUInt16(b->GetPos(), (RpcCodeType)m_code);
    b->MoveHeadBack(sizeof(RpcCodeType));
    ByteOrderUtils::WriteUInt16(b->GetPos(), m_ht);
}
} // namespace rpc
} // namespace flyingkv
