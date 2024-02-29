//
// Created by pablo on 2/29/24.
//

#ifndef CHECKBOXREADONLY_H
#define CHECKBOXREADONLY_H

#include <QCheckBox>

class CheckBoxReadOnly final: public QCheckBox {
    Q_OBJECT

public:
    explicit CheckBoxReadOnly(QWidget* parent = nullptr);

    ~CheckBoxReadOnly() override = default;
};


#endif //CHECKBOXREADONLY_H
