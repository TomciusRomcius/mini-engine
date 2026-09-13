#pragma once

#include <any>
#include <functional>
#include <string>
#include <utility>

namespace mini_engine {
    class IProperty {
    public:
        using Getter = std::function<std::any()>;
        using Setter = std::function<void(const std::any &)>;

        IProperty(std::string name, std::string type, Getter getter = {}, Setter setter = {})
            : m_Name(std::move(name)),
              m_Type(std::move(type)),
              m_Getter(std::move(getter)),
              m_Setter(std::move(setter)) {}

        [[nodiscard]] const std::string &getName() const {
            return m_Name;
        }

        [[nodiscard]] const std::string &getType() const {
            return m_Type;
        }

        [[nodiscard]] std::any getValue() const {
            if (m_Getter) {
                return m_Getter();
            }
            return m_Value;
        }

        void setValue(const std::any &value) const {
            m_Value = value;
            if (m_Setter) {
                m_Setter(value);
            }
        }

    private:
        std::string m_Name;
        std::string m_Type;
        mutable std::any m_Value;
        Getter m_Getter;
        Setter m_Setter;
    };
}
