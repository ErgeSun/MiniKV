/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_NET_CORE_SOCKETAPI_SOCKET_EVENT_HANDLER_H
#define FLYINGKV_NET_CORE_SOCKETAPI_SOCKET_EVENT_HANDLER_H

#include "../../../../../sys/gcc-buildin.h"
#include "../../../../../common/common-def.h"
#include "../../../../../common/reference-counter.h"

#include "../../ievent-handler.h"
#include "file-descriptor.h"
#include "../event-drivers/event-worker.h"

namespace flyingkv {
namespace net {
class ANetStackMessageWorker;
class AFileEventHandler : public common::ReferenceCounter, public IEventHandler {
PUBLIC
    AFileEventHandler() : common::ReferenceCounter(1) {}
    explicit AFileEventHandler(FileDescriptor *socketDesc) : common::ReferenceCounter(1), m_socketDesc(socketDesc) {}

    virtual ~AFileEventHandler() = default;
    /**
     * 必须先调用此函数进行初始化。
     * @return
     */
    virtual bool Initialize() = 0;

    inline FileDescriptor* GetSocketDescriptor() {
        return m_socketDesc;
    }

    inline void SetOwnWorker(EventWorker* ew) {
        m_pOwnEvWorker = ew;
    }

    inline EventWorker* GetOwnWorker() {
        return m_pOwnEvWorker;
    }

    void Release() final override {
        common::ReferenceCounter::Release();
        if (0 == common::ReferenceCounter::GetRef()) {
            if (LIKELY(m_pOwnEvWorker)) {
                m_pOwnEvWorker->AddExternalEpDelEvent(this);
                m_pOwnEvWorker->Wakeup();
            }
        }
    }

    virtual ANetStackMessageWorker* GetStackMsgWorker() = 0;

PROTECTED
    inline void setSocketDescriptor(FileDescriptor *psd) {
        m_socketDesc = psd;
    }

PRIVATE
    FileDescriptor *m_socketDesc = nullptr;
    EventWorker    *m_pOwnEvWorker = nullptr;
};
}
}

#endif //FLYINGKV_NET_CORE_SOCKETAPI_SOCKET_EVENT_HANDLER_H
