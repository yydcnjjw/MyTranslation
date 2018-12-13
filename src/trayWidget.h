#ifndef WIDGET_H
#define WIDGET_H

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QUrlQuery>
#include <QWidget>
#include <iostream>

namespace Translation {

class TrayWidget : public QWidget {
  Q_OBJECT

 public:
  explicit TrayWidget();
  ~TrayWidget() = default;

 private:
  void createActions();
  void createTrayIcon();

  QLabel *fillLabel;
  QLabel *translationLabel;
  QDialog *translationDialog;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

  void initTranslationDialog();
 private slots:
  void requestTranslate(QString &text);
  void screenTranslate();
  void clipboardTranslate();
};
    

}  // Translation

#endif
#endif  // WIDGET_H
