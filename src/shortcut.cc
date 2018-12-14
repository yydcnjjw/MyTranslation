#include "shortcut.h"

namespace Translation {

Shortcut *Shortcut::instance_ = nullptr;

Shortcut *Shortcut::instance() {
    if (instance_ == nullptr) {
        instance_ = new Shortcut;
    }
    return instance_;
}

bool Shortcut::registerShortcut(ShortcutAction action, std::string &&key) {
    QxtGlobalShortcut *shortcut = new QxtGlobalShortcut;
    bool ret = shortcut->setShortcut(QKeySequence(key.c_str()));
    if (ret == true) {
        shortcutMap_[action] = shortcut;
    } else {
        free(shortcut);
    }
    return ret;
}

QxtGlobalShortcut *Shortcut::getShortcut(ShortcutAction action) {
    return shortcutMap_[action];
}

} // namespace Translation
