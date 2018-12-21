#include "shortcut_manager.h"

#include <QObject>

#include "no_destructor.h"
#include "translation_widget.h"

namespace MyTranslation {

namespace {
class ShortcutFunction : public QObject {
  public slots:
    void clipboard() { getSelectTranslationWidget()->Translation(); }
    void screen() { getClipScreenTranslationWidget()->Translation(); }
};

struct Shortcut {
    ShortcutAction action;
    std::string key;
    ShortcutFunction *instance;
    void (ShortcutFunction::*func)(void);
};

ShortcutFunction shortcutFunction;

std::vector<struct Shortcut> origShorcutSet = {
    {ShortcutAction::CLIPBOARD, "Alt+t", &shortcutFunction,
     &ShortcutFunction::clipboard},
    {ShortcutAction::SCREEN, "Ctrl+t", &shortcutFunction,
     &ShortcutFunction::screen}};
} // namespace

ShortcutManager::ShortcutManager() { loadOrigShortcut(); }

void ShortcutManager::loadOrigShortcut() {
    for (auto shortcut : origShorcutSet) {
        if (shortcut.key.length() != 0) {
            Status s = registerShortcut(shortcut.action, shortcut.key);
            if (s.ok()) {
                QObject::connect(getShortcut(shortcut.action),
                                 &QxtGlobalShortcut::activated,
                                 shortcut.instance, shortcut.func);
                // TODO: log
            }
        }
    }
}

Status ShortcutManager::registerShortcut(ShortcutAction action,
                                         std::string &key) {
    QxtGlobalShortcut *shortcut = new QxtGlobalShortcut;
    bool ret = shortcut->setShortcut(QKeySequence(key.c_str()));
    if (ret == true) {
        shortcutMap_[action] = shortcut;
    } else {
        free(shortcut);
        return Status::ShortcutError("register shortcut error");
    }
    return Status::OK();
}

QxtGlobalShortcut *ShortcutManager::getShortcut(ShortcutAction action) {
    return shortcutMap_[action];
}

ShortcutManager *getShortcutManager() {
    static NoDestructor<ShortcutManager> singleton;
    return singleton.get();
}

} // namespace MyTranslation
