#pragma once

#include "MathOperationDataModel.hpp"

class AdditionModel : public MathOperationDataModel
{
public:
    ~AdditionModel() override = default;

public:
    QString caption() const override { return QStringLiteral("Addition"); }

    QString name() const override { return QStringLiteral("Addition"); }

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
                        return std::make_shared<VariantData>(first + second);
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first + second);
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
                        return std::make_shared<VariantData>(first + second);
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first + second);
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
                        return std::make_shared<VariantData>(first + second);
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(first + second);
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
                        return std::make_shared<VariantData>(first + second);
                    }
                    case QMetaType::Int: {
                        const int second = n2->variant().toInt();
                        return std::make_shared<VariantData>(QSize(first.width() + second, first.height() + second));
                    }
                    case QMetaType::Double: {
                        const double second = n2->variant().toDouble();
                        return std::make_shared<VariantData>(QSize(first.width() + second, first.height() + second));
                    }
                    case QMetaType::Float: {
                        const float second = n2->variant().toFloat();
                        return std::make_shared<VariantData>(QSize(first.width() + second, first.height() + second));
                    }
                    default:
                        qCritical() << "AdditionModel::compute: invalid type";
                        return std::make_shared<VariantData>();
                }
            }
            default:
                qCritical() << "AdditionModel::compute: unknown type of dividend";
                return std::make_shared<VariantData>();
        }
    }
};
