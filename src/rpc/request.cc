/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#include <google/protobuf/message.h>

#include "../common/buffer.h"
#include "../utils/codec-utils.h"
#include "../utils/protobuf-utils.h"

#include "request.h"
#include "common-def.h"

namespace flyingkv {
namespace rpc {
uint32_t RpcRequest::getDerivePayloadLength() {
    if (m_sspMsg) {
        return sizeof(uint16_t) + m_sspMsg->ByteSize();
    } else {
        return sizeof(uint16_t);
    }
}

void RpcRequest::encodeDerive(common::Buffer *b) {
    ByteOrderUtils::WriteUInt16(b->GetPos(), (uint16_t)m_iHandlerId);
    b->MoveHeadBack(sizeof(uint16_t));
    if (m_sspMsg) {
        utils::ProtoBufUtils::Serialize(m_sspMsg.get(), b);
    }
}
} // namespace rpc
} // namespace flyingkv
