#include "NumberSourceDataModel.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>

NumberSourceDataModel::NumberSourceDataModel()
    : m_number(std::make_shared<VariantData>(0))
      , _lineEdit{nullptr} {
}

QJsonObject NumberSourceDataModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();

    modelJson["number"] = QString::number(m_number->variant().toDouble());

    return modelJson;
}

void NumberSourceDataModel::load(QJsonObject const& p) {
    QJsonValue v = p["number"];

    if (!v.isUndefined()) {
        QString strNum = v.toString();

        bool ok;
        double d = strNum.toDouble(&ok);
        if (ok) {
            m_number = std::make_shared<VariantData>(d);

            if (_lineEdit)
                _lineEdit->setText(strNum);
        }
    }
}

unsigned int NumberSourceDataModel::nPorts(PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case PortType::In:
            result = 0;
            break;

        case PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

void NumberSourceDataModel::onTextEdited(QString const& str) {
    bool ok = false;
    // based on the str determine if the number is a double or an int
    QVariant number;
    if (str.contains('.') || str.contains('e') || str.contains('E')){
        number = str.toDouble(&ok);
    } else {
        number = str.toInt(&ok);
    }

    if (ok) {
        m_number = std::make_shared<VariantData>(number);
        Q_EMIT dataUpdated(0);
    } else {
        Q_EMIT dataInvalidated(0);
    }
}

NodeDataType NumberSourceDataModel::dataType(PortType, PortIndex) const {
    return m_number->type();
}

std::shared_ptr<NodeData> NumberSourceDataModel::outData(PortIndex) {
    return m_number;
}

QWidget* NumberSourceDataModel::embeddedWidget() {
    if (!_lineEdit) {
        _lineEdit = new QLineEdit();

        _lineEdit->setValidator(new QDoubleValidator());
        _lineEdit->setMaximumSize(_lineEdit->sizeHint());

        connect(_lineEdit, &QLineEdit::textChanged, this, &NumberSourceDataModel::onTextEdited);

        _lineEdit->setText(QString::number(m_number->variant().toDouble()));
    }

    return _lineEdit;
}
