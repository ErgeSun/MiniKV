/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_TEST_ENTRY_HANDLER_H
#define FLYINGKV_TEST_ENTRY_HANDLER_H

#include "../../../common/ientry.h"
#include "../../../wal/iwal.h"

namespace flyingkv {
namespace waltest {
class TestEntryHandler {
PUBLIC
    common::IEntry* CreateNewEntry();
    common::IEntry* CreateNewEntryWithContent(std::string &&content);
    void OnLoad(std::vector<wal::WalEntry>);

PRIVATE
    std::vector<wal::WalEntry>  m_vLoadEntries;
};
}
}

#endif //FLYINGKV_TEST_ENTRY_HANDLER_H