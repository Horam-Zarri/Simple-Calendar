#include "calendarwidget.h"
#include <QtWidgets>
#include <algorithm>

CalendarWidget::CalendarWidget(QWidget *parent)
    : QWidget{parent}
{
    gridLayout = new QGridLayout();
    setLayout(gridLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setFont(QFont());
    date = QDate::currentDate();
    currentDay = date.day();
    initCalendarGrid();
    updateCalendarGrid();

}

QDate CalendarWidget::getDate()
{
    return date;
}

void CalendarWidget::insertDateToken(DateToken token)
{
    dateTokens.push_back(token);
}


//very broken function
void CalendarWidget::eraseDateToken(DateToken token)
{
    for (auto p = dateTokens.begin(); p != dateTokens.end(); p++) {

        auto str = p->accessToken().split('/');
        auto tar = token.accessToken().split('/');

        if (str.at(0) == tar.at(0) && str.at(1) == tar.at(1) && str.at(2) == tar.at(2)) {
            dateTokens.erase(p);
            break;
        }
    }
}

const QVector<DateToken> &CalendarWidget::retrieveTokens()
{
    return dateTokens;
}

void CalendarWidget::clearTokens()
{
    dateTokens.clear();
}

void CalendarWidget::setDate(QDate date)
{

    this->date = date;
    updateCalendarGrid();
    emit dateChanged(this->date);
}

void CalendarWidget::incrementMonth()
{
    setDate(date.addMonths(1));
}

void CalendarWidget::decrementMonth()
{
    setDate(date.addMonths(-1));
}



const std::array<QChar, 7> daysAbbs = {'S', 'M', 'T', 'W', 'T', 'F', 'S'};

void CalendarWidget::initCalendarGrid()
{

    QLabel* weekDays[7];

    for (int i = 0; i < 7; i++) {
        weekDays[i] = new QLabel((QString)daysAbbs.at(i));
        weekDays[i]->setFont(QFont("Arial", DateLabel::FONT_POINT_SIZE - 5));
        gridLayout->addWidget(weekDays[i], 0, i, 1, 1, Qt::AlignCenter);
    }

    for (int i = 0; i < MAX_DAYS_IN_MONTH; i++) {
        dayLabels[i] = new DateLabel("");
    }
}

void CalendarWidget::updateCalendarGrid()
{
    int day = date.dayOfWeek();
    if (day == 7)
        day = 0;

    QList<DateToken> monthSet = fetchMonthTokens();

    for (int i = 1; i <= date.daysInMonth(); i++) {

        dayLabels[i - 1]->reset();

        QString txt = "";

        QListIterator<DateToken> tokenIter(monthSet);

        while (tokenIter.hasNext()) {
            DateToken next = tokenIter.next();
            QList<QString> list = next.accessToken().split('/');


            if (list.at(1) == DateToken::itos(i)) {

                txt = list.at(3);

                dayLabels[i - 1]->setTextColor(next.textColor());
                dayLabels[i - 1]->setOutlineColor(next.outlineColor());

                break;
            }

        }

        if (i == QDate::currentDate().day() && date.month() == QDate::currentDate().month()
            && date.year() == QDate::currentDate().year())
            dayLabels[i - 1]->setCurrent(true);

        dayLabels[i - 1]->setDay(i);
        dayLabels[i - 1]->setLabelText(txt);

        gridLayout->addWidget(dayLabels[i - 1], (i + day - 1) / 7 + 1, (i + day - 1) % 7, 1,1, Qt::AlignCenter);
    }

    update();
}

QList<DateToken> CalendarWidget::fetchMonthTokens()
{
    QVectorIterator<DateToken> iter(dateTokens);
    QList<DateToken> res;

    while(iter.hasNext()) {

        const DateToken next = iter.next();
        QString tok = next.accessToken();

        QList<QString> list = tok.split('/');

        if (list.at(0) == DateToken::itos(date.month()) && list.at(2) == DateToken::itos(date.year()))
            res.push_back(next);
    }

    return res;
}


