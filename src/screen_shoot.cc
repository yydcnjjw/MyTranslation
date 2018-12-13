#include "screen_shoot.h"
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qscreen.h>
#include <qdebug.h>

ScreenShoot *ScreenShoot::instance = nullptr;

ScreenShoot::ScreenShoot() {
  QDesktopWidget *desktopWidget = QApplication::desktop();
  QRect deskRect = desktopWidget->screenGeometry();

  this->resize(deskRect.width(), deskRect.height());
  this->setWindowOpacity(0);
  this->setWindowFlag(Qt::WindowStaysOnTopHint);

  // this->setWindowFlag(Qt::FramelessWindowHint);

  rubberBand = nullptr;
  mousePressPos = mouseReleasePos = QPoint(0, 0);
}

ScreenShoot::~ScreenShoot() {}

// QString ScreenShoot::getScreentShootImage() {
//   this->show();
//   return NULL;
// }

void ScreenShoot::grabScreen() {
  int wid = abs(mousePressPos.x() - mouseReleasePos.x());
  int height = abs(mousePressPos.y() - mouseReleasePos.y());
  int x = mousePressPos.x() < mouseReleasePos.x() ? mousePressPos.x()
                                                  : mouseReleasePos.x();
  int y = mousePressPos.y() < mouseReleasePos.y() ? mousePressPos.y()
                                                  : mouseReleasePos.y();

  QScreen *screen = QGuiApplication::primaryScreen();
  QPixmap pic = screen->grabWindow(0, x, y, wid, height);
  if (!pic.save("/home/yydcnjjw/workspace/code/project/translation/screen_shoot.png", "png")) {
    qDebug() << "could not be saved!";
  }
  emit hided();
}

void ScreenShoot::mouseReleaseEvent(QMouseEvent *e) {
  if (rubberBand) {
    mouseReleasePos = e->pos();
    rubberBand->hide();
    grabScreen();
    this->hide();
  }
}

void ScreenShoot::mousePressEvent(QMouseEvent *e) {
  if (!rubberBand) {
    rubberBand = new QRubberBand(QRubberBand::Rectangle);
  }
  mousePressPos = e->pos();
  rubberBand->setGeometry(QRect(mousePressPos, QSize(0, 0)));
  rubberBand->show();
}

void ScreenShoot::mouseMoveEvent(QMouseEvent *e) {
  if (rubberBand) {
    rubberBand->setGeometry(QRect(mousePressPos, e->pos()).normalized());
  }
}
