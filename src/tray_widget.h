#ifndef WIDGET_H
#define WIDGET_H

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QUrlQuery>
#include <QWidget>
#include <map>
namespace Translation {

class TrayWidget : public QWidget {
    Q_OBJECT

  public:
    explicit TrayWidget();
    ~TrayWidget() = default;

  private:
    void initShortcut();

    QSystemTrayIcon *trayIcon;
    void initOSTray();
    
    QLabel *translContentLabel_;
    QDialog *translDialog_;

    void initTranslationDialog();
                                
  private slots:
    void requestTranslate(QString &text);
    void screenTranslate();
    void clipboardTranslate();
};

} // namespace Translation

#endif
#endif // WIDGET_H
