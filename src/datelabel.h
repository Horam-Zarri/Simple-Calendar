#ifndef DATELABEL_H
#define DATELABEL_H

#include <QLabel>
#include <QObject>

class DateLabel : public QLabel
{
    Q_OBJECT
public:

    static const int FONT_POINT_SIZE = 30;

    explicit DateLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit DateLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void initLabel();

    void reset();

    void setLabelText(const QString& text);
    QColor textColor();
    QColor outlineColor();
    void setTextColor(QColor color);
    void setOutlineColor(QColor color);

    void setDay(int x) {_day = x;}
    int day() {return _day;}

    void setCurrent(bool isCurr);

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private slots:
    void reInit();
private:

    int _day = 0;
    bool currentDay;
    QColor text_color;
    QColor outline_color = palette().window().color();
    QString text;

    QAction* resetAction;
};

#endif // DATELABEL_H
