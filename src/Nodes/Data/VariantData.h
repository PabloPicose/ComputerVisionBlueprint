//
// Created by pablo on 2/29/24.
//

#ifndef VARIANTDATA_H
#define VARIANTDATA_H

#include <QtNodes/NodeData>
#include <QVariant>

class VariantData final : public QtNodes::NodeData {
public:
    VariantData() = default;

    explicit VariantData(const QVariant& variant) : m_variant(variant) {
    }

    QtNodes::NodeDataType type() const override {
        QString typeName = m_variant.typeName();
        // if the typename starts with Q or q, remove it
        if (typeName.startsWith('Q') || typeName.startsWith('q')) {
            typeName = typeName.mid(1);
        }
        // the string should have at least 10 characters, if not, fill with spaces
        typeName = typeName.rightJustified(10, ' ');
        return QtNodes::NodeDataType{
            "variant",
            typeName
        };
    }

    QtNodes::NodeDataType typeIn() const {
        QString typeName = m_variant.typeName();
        if (typeName.startsWith('Q') || typeName.startsWith('q')) {
            typeName = typeName.mid(1);
        }
        typeName = typeName.leftJustified(10, ' ');
        return QtNodes::NodeDataType{
            "variant",
            typeName
        };
    }

    bool isType(QMetaType::Type const type) const {
        return m_variant.metaType().id() == type;
    }

    QMetaType::Type metaType() const {
        return static_cast<QMetaType::Type>(m_variant.metaType().id());
    }

    QVariant variant() const {
        return m_variant;
    }

    bool isValid() const {
        return m_variant.isValid();
    }

    QString toString() const {
        QString output;
        switch (metaType()) {
            case QMetaType::QSize: {
                const QSize size = m_variant.toSize();
                output = QString("%1x%2").arg(size.width()).arg(size.height());
                break;
            }
            case QMetaType::Bool: {
                output = m_variant.toBool() ? "true" : "false";
                break;
            }
            default : {
                output = m_variant.toString();
                break;
            }
        }

        return output;
    }

private:
    QVariant m_variant;
};
#endif //VARIANTDATA_H
