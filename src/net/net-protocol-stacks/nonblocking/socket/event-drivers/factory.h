/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_NET_CORE_NB_SOCKET_ED_FACTORY_H
#define FLYINGKV_NET_CORE_NB_SOCKET_ED_FACTORY_H

#include "ievent-driver.h"
#include "epoll/epoll-event-driver.h"
#include "../../../../common-def.h"

namespace flyingkv {
namespace net {
class EventDriverFactory {
PUBLIC
    static IEventDriver* GetNewDriver(NonBlockingEventModel m) {
        IEventDriver *eventDriver = nullptr;
        if (NonBlockingEventModel::DPDK == m) {
#ifdef HAVE_DPDK
            // eventDriver = new DPDKDriver(cct);
#endif
        } else {
#ifdef __linux__ // HAVE_EPOLL
            eventDriver = new EpollEventDriver();
#else
            #ifdef (defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6)) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__) // HAVE_KQUEUE
        // eventDriver = new KqueueDriver();
#else
        // eventDriver = new SelectDriver();
#endif
#endif
        }

        return eventDriver;
    }
};
}  // namespace net
}  // namespace flyingkv

#endif //FLYINGKV_NET_CORE_NB_SOCKET_ED_FACTORY_H
