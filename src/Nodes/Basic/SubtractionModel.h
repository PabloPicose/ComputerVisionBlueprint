#pragma once

#include "MathOperationDataModel.hpp"


/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class SubtractionModel final : public MathOperationDataModel {
public:
    virtual ~SubtractionModel() override {
    }

public:
    QString caption() const override { return QStringLiteral("Subtraction"); }

    virtual bool portCaptionVisible(PortType portType, PortIndex portIndex) const override {
        Q_UNUSED(portType);
        Q_UNUSED(portIndex);
        return true;
    }

    virtual QString portCaption(PortType portType, PortIndex portIndex) const override {
        switch (portType) {
            case PortType::In:
                if (portIndex == 0)
                    return QStringLiteral("Minuend");
                else if (portIndex == 1)
                    return QStringLiteral("Subtrahend");

                break;

            case PortType::Out:
                return QStringLiteral("Result");

            default:
                break;
        }
        return QString();
    }

    QString name() const override { return QStringLiteral("Subtraction"); }

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
                const int first = n1->variant().toInt();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int second = n2->variant().toInt();
                        return std::make_shared<VariantData>(first - second);
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first - second);
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            case QMetaType::Double: {
                const double first = n1->variant().toDouble();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int second = n2->variant().toInt();
                        return std::make_shared<VariantData>(first - second);
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first - second);
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            case QMetaType::Float: {
                const float first = n1->variant().toFloat();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::Int: {
                        const int second = n2->variant().toInt();
                        return std::make_shared<VariantData>(first - second);
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first - second);
                    }
                    case QMetaType::Float: {
                        const float second = n2->variant().toFloat();
                        return std::make_shared<VariantData>(first - second);
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            case QMetaType::QSize: {
                const QSize first = n1->variant().toSize();
                // switch on the type of the divisor
                switch (n2->metaType()) {
                    case QMetaType::QSize: {
                        const QSize second = n2->variant().toSize();
                        return std::make_shared<VariantData>(first - second);
                    }
                    case QMetaType::Int: {
                        const int second = n2->variant().toInt();
                        return std::make_shared<VariantData>(first - QSize(second, second));
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first - QSize(second, second));
                    }
                    case QMetaType::Float: {
                        const float second = n2->variant().toFloat();
                        return std::make_shared<VariantData>(first - QSize(second, second));
                    }
                    default:
                        return std::make_shared<VariantData>();
                }
            }
            default:
                qCritical() << "SubtractionModel::compute: unknown type";
                return std::make_shared<VariantData>();
        }
    }
};
