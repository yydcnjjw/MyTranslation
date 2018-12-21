#ifndef TRAY_WIDGET_H
#define TRAY_WIDGET_H

#ifndef QT_NO_SYSTEMTRAYICON

#include <QSystemTrayIcon>
#include <QWidget>

#include "status.h"

namespace MyTranslation {

class TrayWidget : public QWidget {
    Q_OBJECT

  public:
    explicit TrayWidget();
    ~TrayWidget() = default;

  private:
    QSystemTrayIcon *trayIcon;
    void initOSTray();
};

} // namespace Translation

#endif
#endif /* TRAY_WIDGET_H */
