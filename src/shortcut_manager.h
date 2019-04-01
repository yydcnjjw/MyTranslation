#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <map>
#include <string>

#include <qxtglobalshortcut.h>

#include "status.h"

namespace MyTranslation {
    
enum ShortcutAction {
    SELECT,
    SCREEN,
    WINDOW_HIDE
};
    
class ShortcutManager {
  public:
    ShortcutManager();

    ShortcutManager(const ShortcutManager &) = delete;
    ShortcutManager &operator=(const ShortcutManager &) = delete;

    QxtGlobalShortcut *getShortcut(ShortcutAction);

    Status registerShortcut(ShortcutAction, std::string &);

  private:
    void loadOrigShortcut();
    std::map<ShortcutAction, QxtGlobalShortcut *> shortcutMap_;
};

extern ShortcutManager *getShortcutManager();

} // namespace MyTranslation

#endif /* SHORTCUT_H */
