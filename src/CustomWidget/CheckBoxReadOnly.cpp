//
// Created by pablo on 2/29/24.
//

#include "CheckBoxReadOnly.h"
#include <QEvent>

CheckBoxReadOnly::CheckBoxReadOnly(QWidget* parent) : QCheckBox(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);
}


