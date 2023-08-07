#ifndef DATEEVENTDIALOG_H
#define DATEEVENTDIALOG_H

#include <QDialog>
#include <QtWidgets>

#include "ui_dateeventdialog.h"
#include "datelabel.h"

class DateEventDialog : public QDialog, public Ui::DateEventDialog {
    Q_OBJECT
public:

    explicit DateEventDialog(DateLabel* label, QWidget* parent = nullptr);

private slots:

    void setChanges();
    void popupTextColorDialog();
    void popupOutlineColorDialog();

    void validateText(QString str);
private:
    void setupColors();
    DateLabel* dateLabel;

};

#endif // DATEEVENTDIALOG_H
