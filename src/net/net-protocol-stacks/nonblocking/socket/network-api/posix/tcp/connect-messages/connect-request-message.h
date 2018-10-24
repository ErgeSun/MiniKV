/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef MINIKV_CONNECT_REQUEST_MESSAGE_H
#define MINIKV_CONNECT_REQUEST_MESSAGE_H

#include "../../../../../../../snd-message.h"
#include "../../../../../../../../utils/codec-utils.h"
#include "../../../../../../../../common/buffer.h"

namespace minikv {
namespace net {
class ConnectRequestMessage : public SndMessage {
public:
    ConnectRequestMessage(sys::MemPool *mp, int16_t logicPort) :
        SndMessage(mp, net_peer_info_s()), m_logicPort(std::move(logicPort)) {}

protected:
    uint32_t getDerivePayloadLength() override {
        return sizeof(uint16_t)/*logic port*/;
    }

    void encodeDerive(common::Buffer *b) override {
        ByteOrderUtils::WriteUInt16(b->GetPos(), (uint16_t)m_logicPort);
    }

private:
    int16_t          m_logicPort;
};
}
}

#endif //MINIKV_CONNECT_REQUEST_MESSAGE_H
