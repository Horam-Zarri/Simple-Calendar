#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include "datelabel.h"

#include <QWidget>
#include <QVector>
#include <QDate>

class QGridLayout;
class QLabel;
class DateToken;

class CalendarWidget : public QWidget
{
    Q_OBJECT
public:

    explicit CalendarWidget(QWidget *parent = nullptr);

    QDate getDate();
    QList<DateToken> fetchMonthTokens();

    void insertDateToken(DateToken token);
    void eraseDateToken(DateToken token);

    const QVector<DateToken>& retrieveTokens();
    void clearTokens();

    void updateCalendarGrid();

signals:
    void dateChanged(QDate newDate);

public slots:
    void setDate(QDate date);
    void incrementMonth();
    void decrementMonth();

private:
    void initCalendarGrid();



    static const int WEEK_DAYS = 7;
    static const int MAX_DAYS_IN_MONTH = 31;

    QGridLayout* gridLayout;
    DateLabel* dayLabels[MAX_DAYS_IN_MONTH];

    QVector<DateToken> dateTokens;

    int currentDay;
    QDate date;
};

class DateToken {
public:
    explicit DateToken(const QDate& date, const QString& text, QColor textColor, QColor outlineColor) {

        this->_date = date;

        token = "";
        token.append(itos(date.month())).append('/').append(itos(date.day())).append
            ('/').append(itos(date.year())).append('/').append(text);

        text_color = textColor;
        outline_color = outlineColor;
    };

    QString accessToken() const {return token;}
    QColor textColor() const {return text_color;}
    QColor outlineColor() const {return outline_color;}
    QDate date() const {return _date;}

    static QString itos(int x) {
        return QString::fromStdString(std::to_string(x));
    }
private:

    QString token;
    QDate _date;

    QColor text_color;
    QColor outline_color;

};

#endif // CALENDARWIDGET_H
