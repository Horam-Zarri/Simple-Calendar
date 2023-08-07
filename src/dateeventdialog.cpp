#include "dateeventdialog.h"

#include <QDialogButtonBox>

#include <QFontMetricsF>

DateEventDialog::DateEventDialog(DateLabel *label, QWidget *parent)
    : QDialog(parent) {

    setupUi(this);
    dateLabel = label;

    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    setupColors();

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(setChanges()));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(validateText(QString)));

    connect(textColorPushButton, SIGNAL(clicked(bool)), this, SLOT(popupTextColorDialog()));
    connect(outlineColorPushButton, SIGNAL(clicked(bool)), this, SLOT(popupOutlineColorDialog()));

}

void DateEventDialog::setChanges()
{

    dateLabel->setLabelText(lineEdit->text());

    QDialog::accept();
}

void DateEventDialog::popupTextColorDialog()
{
    QColor color = QColorDialog::getColor(dateLabel->textColor(), this, "Select Color: ");
    dateLabel->setTextColor(color);
    setupColors();
}

void DateEventDialog::popupOutlineColorDialog()
{
    QColor color = QColorDialog::getColor(dateLabel->outlineColor(), this, "Select Color: ");
    dateLabel->setOutlineColor(color);
    setupColors();
}



void DateEventDialog::validateText(QString str)
{
    if (!str.isEmpty())
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void DateEventDialog::setupColors()
{
    QPixmap pixmap(textColorLabel->size());
    pixmap.fill(dateLabel->textColor());
    textColorLabel->setPixmap(pixmap);

    pixmap = QPixmap(outlineColorLabel->size());
    pixmap.fill(dateLabel->outlineColor());
    outlineColorLabel->setPixmap(pixmap);
}
