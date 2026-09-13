#pragma once

#include <any>
#include <functional>
#include <string>
#include <utility>
#include <vector>

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

    class PropertyGroup {
    public:
        PropertyGroup(std::string name, std::vector<IProperty> properties)
            : m_Name(std::move(name)),
              m_Properties(std::move(properties)) {}

        [[nodiscard]] const std::string &getName() const {
            return m_Name;
        }

        [[nodiscard]] const std::vector<IProperty> &getProperties() const {
            return m_Properties;
        }

    private:
        std::string m_Name;
        std::vector<IProperty> m_Properties;
    };

    class PropertyTabGroup {
    public:
        using TabChangedCallback = std::function<void(const std::string &)>;
        using CurrentTabGetter = std::function<std::string()>;

        PropertyTabGroup(
            std::string name,
            std::vector<std::string> tabs,
            TabChangedCallback onTabChanged,
            CurrentTabGetter getCurrentTab)
            : m_Name(std::move(name)),
              m_Tabs(std::move(tabs)),
              m_OnTabChanged(std::move(onTabChanged)),
              m_GetCurrentTab(std::move(getCurrentTab)) {}

        [[nodiscard]] const std::string &getName() const {
            return m_Name;
        }

        [[nodiscard]] const std::vector<std::string> &getTabs() const {
            return m_Tabs;
        }

        [[nodiscard]] std::string getCurrentTab() const {
            if (m_GetCurrentTab) {
                return m_GetCurrentTab();
            }
            return {};
        }

        void selectTab(const std::string &tab) const {
            if (m_OnTabChanged) {
                m_OnTabChanged(tab);
            }
        }

    private:
        std::string m_Name;
        std::vector<std::string> m_Tabs;
        TabChangedCallback m_OnTabChanged;
        CurrentTabGetter m_GetCurrentTab;
    };
}
