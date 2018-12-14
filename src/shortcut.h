#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <map>
#include <qxtglobalshortcut.h>
#include <string>

namespace Translation {

enum ShortcutAction { CLIPBOARD, SCREEN };

class Shortcut {

public:
  static Shortcut *instance();

  Shortcut() = default;
  ~Shortcut() = default;

  Shortcut(const Shortcut &) = delete;
  Shortcut &operator=(const Shortcut &) = delete;

  QxtGlobalShortcut *getShortcut(ShortcutAction);

  bool registerShortcut(ShortcutAction, std::string &&);

private:
  std::map<ShortcutAction, QxtGlobalShortcut *> shortcutMap_;
  static Shortcut *instance_;
};
} // namespace Translation

#endif /* SHORTCUT_H */
