/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef MINIKV_MINI_KV_H
#define MINIKV_MINI_KV_H

#include "../../common/iservice.h"

#include "../ikv-handler.h"

namespace minikv {
namespace kv {
class MiniKV : public common::IService, public IKVHandler {
public:
    MiniKV();
    ~MiniKV();

    bool Start() override;
    bool Stop() override;

    rpc::SP_PB_MSG OnPut(rpc::SP_PB_MSG sspMsg)    override;
    rpc::SP_PB_MSG OnGet(rpc::SP_PB_MSG sspMsg)    override;
    rpc::SP_PB_MSG OnDelete(rpc::SP_PB_MSG sspMsg) override;
    rpc::SP_PB_MSG OnScan(rpc::SP_PB_MSG sspMsg)   override;
private:
};
}
}

#endif //MINIKV_MINI_KV_H