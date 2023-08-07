#include "datelabel.h"
#include "dateeventdialog.h"
#include "calendarwidget.h"

#include <QMouseEvent>

DateLabel::DateLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel{parent, f}
{
    initLabel();
}

DateLabel::DateLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel{text,parent,f}
{
    initLabel();
}


void DateLabel::initLabel()
{
    text_color = Qt::white;
    currentDay = false;

    resetAction = new QAction("&Reset");
    resetAction->setShortcut(QKeyCombination(Qt::CTRL, Qt::Key_R));
    connect(resetAction, SIGNAL(triggered(bool)), this, SLOT(reInit()));

    addAction(resetAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void DateLabel::setLabelText(const QString &text)
{
    QString txt = QString::fromStdString(std::to_string(_day));
    if (text != "")
        txt = txt.append('\n').append(text);
    this->text = text;

    QFont f(font());
    f.setPointSize(FONT_POINT_SIZE);
    QFontMetrics metrics(f);


    QRect rect = metrics.boundingRect(txt);
    rect.adjust(-FONT_POINT_SIZE / 3,-FONT_POINT_SIZE / 3 * 2 - 10, FONT_POINT_SIZE / 3, FONT_POINT_SIZE / 3 * 2 + 10);

    const QSize size = rect.size();
    QPixmap pixmap(size);

    pixmap.fill(outline_color);

    QPainter painter(&pixmap);

    painter.setPen(QPen(text_color));


    if (currentDay)
        f.setUnderline(true);

    painter.setFont(f);

    painter.drawText(pixmap.rect(), Qt::AlignCenter, txt);
    setPixmap(pixmap);

    updateGeometry();
    update();
}

QColor DateLabel::textColor()
{
    return text_color;
}

QColor DateLabel::outlineColor()
{
    return outline_color;
}

void DateLabel::setTextColor(QColor color)
{
    text_color = color;
}

void DateLabel::setOutlineColor(QColor color)
{
    outline_color = color;
}

void DateLabel::setCurrent(bool isCurr)
{
    currentDay = isCurr;
}

void DateLabel::reset()
{
    currentDay = false;
    _day = 0;
    text = "";
    QColor white = Qt::white;
    text_color = white;
    QColor back = palette().window().color();
    outline_color = back;


    updateGeometry();
    update();
}


// The following two functions are dependant on CalendarWidget (#include "calendarwidget.h"). Will need reimplementation if used with another parent widget

void DateLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        DateEventDialog dialog(this);

        if (dialog.exec()) {

            CalendarWidget* cw= qobject_cast<CalendarWidget*>(this->parentWidget());

            if (cw) {
                DateToken dateToken(QDate(cw->getDate().year(), cw->getDate().month(), this->day()), text, text_color, outline_color);
                cw->eraseDateToken(dateToken);
                cw->insertDateToken(dateToken);
            }
        }
    }

    QLabel::mouseDoubleClickEvent(e);
}

void DateLabel::reInit()
{

    CalendarWidget* cw= qobject_cast<CalendarWidget*>(this->parentWidget());

    if (cw) {
        DateToken dateToken(QDate(cw->getDate().year(), cw->getDate().month(), this->day()), text, text_color, outline_color);
        cw->eraseDateToken(dateToken);

        cw->updateCalendarGrid();
    }


}
