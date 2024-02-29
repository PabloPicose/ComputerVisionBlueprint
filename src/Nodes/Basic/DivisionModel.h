#pragma once

#include "MathOperationDataModel.hpp"
#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

class DivisionModel final : public MathOperationDataModel {
public:
    virtual ~DivisionModel() {
    }

public:
    QString caption() const override { return QStringLiteral("Division"); }

    bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override {
        Q_UNUSED(portType);
        Q_UNUSED(portIndex);
        return true;
    }

    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override {
        switch (portType) {
            case QtNodes::PortType::In:
                if (portIndex == 0)
                    return QStringLiteral("Dividend");
                else if (portIndex == 1)
                    return QStringLiteral("Divisor");

                break;

            case QtNodes::PortType::Out:
                return QStringLiteral("Result");

            default:
                break;
        }
        return QString();
    }

    QString name() const override { return QStringLiteral("Division"); }

private:
    const QMap<QMetaType::Type, QSet<QMetaType::Type>> _allowedTypes = {
        {QMetaType::Int, {QMetaType::Int, QMetaType::Double}},
        {QMetaType::Double, {QMetaType::Int, QMetaType::Double}},
        {QMetaType::Float, {QMetaType::Int, QMetaType::Double}},
        {QMetaType::QSize, {QMetaType::QSize, QMetaType::Int, QMetaType::Double, QMetaType::Float}},
    };

    std::shared_ptr<VariantData> compute() const override {
        const auto n1 = _number1.lock();
        const auto n2 = _number2.lock();
        if (!n1 || !n2) {
            return std::make_shared<VariantData>();
        }
        // check if allowed
        if (!_allowedTypes.contains(n1->metaType()) || !_allowedTypes[n1->metaType()].contains(n2->metaType())) {
            return std::make_shared<VariantData>();
        }
        // switch on the type of the dividend
        switch (n1->metaType()) {
            case QMetaType::Int: {
                const int dividend = n1->variant().toInt();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int divisor = n2->variant().toInt();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    case QMetaType::Double: {
                        const double divisor = n2->variant().toDouble();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            case QMetaType::Double: {
                const double dividend = n1->variant().toDouble();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int divisor = n2->variant().toInt();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    case QMetaType::Double: {
                        const double divisor = n2->variant().toDouble();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            case QMetaType::Float: {
                const float dividend = n1->variant().toFloat();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int divisor = n2->variant().toInt();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    case QMetaType::Double: {
                        const double divisor = n2->variant().toDouble();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
                break;
            }
            case QMetaType::QSize: {
                const QSize dividend = n1->variant().toSize();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int divisor = n2->variant().toInt();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    case QMetaType::Double: {
                        const double divisor = n2->variant().toDouble();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    case QMetaType::Float: {
                        const float divisor = n2->variant().toFloat();
                        if (divisor == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(dividend / divisor);
                    }
                    case QMetaType::QSize: {
                        const QSize divisor = n2->variant().toSize();
                        if (divisor.width() == 0 || divisor.height() == 0) {
                            return std::make_shared<VariantData>();
                        }
                        return std::make_shared<VariantData>(QSize(dividend.width() / divisor.width(), dividend.height() / divisor.height()));
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            default:
                qCritical() << "DivisionModel::compute: unknown type" << n1->metaType();
                return std::make_shared<VariantData>();
        }
    }
};
