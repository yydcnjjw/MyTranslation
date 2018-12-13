#ifndef SCREEN_SHOOT_H
#define SCREEN_SHOOT_H
#include <qfile.h>
#include <qpoint.h>
#include <qtemporaryfile.h>
#include <qwidget.h>
#include <qrubberband.h>
#include <QMouseEvent>

class ScreenShoot : public QWidget{
  Q_OBJECT

public:
  static ScreenShoot *Instance() {
    if (!instance) {
      instance = new ScreenShoot();
    }
    return instance;
  }

  // QString getScreentShootImage();
  
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  
  ~ScreenShoot() override;

private:
  void grabScreen();
  
  static ScreenShoot *instance;
  QPoint mousePressPos;
  QPoint mouseReleasePos;
  QRubberBand *rubberBand;

  ScreenShoot();
signals:
  bool hided();
};

#endif /* SCREEN_SHOOT_H */
