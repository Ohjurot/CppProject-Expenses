#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <string_view>

class ExpenseSheet
{
    public:
        struct Entry
        {
            // Binary format byte[...]\0byte[sizeof(double)]

            std::string label; 
            double value = 0.;

            void Serialize(std::ostream& out) const;
            void Deserialize(std::istream& in);

            inline bool operator==(const Entry& rhs) const
            {
                return label == rhs.label;
            }
        };

    public:
        ExpenseSheet() = default;
        ExpenseSheet(const ExpenseSheet&) = default;

        ExpenseSheet& operator=(const ExpenseSheet&) = default;

        void New();
        bool Open(const std::filesystem::path& dataFile);
        bool Save(const std::filesystem::path& dataFile = "") const;

        bool Add(std::string_view label, double value);
        bool Del(std::string_view label);
        void List(std::ostream& os, bool repl = false) const;
        double Eval() const;

        bool ExportCsv(const std::filesystem::path& file) const;
        bool ExportHtml(const std::filesystem::path& file) const;

    private:
        static inline std::string TextHTMLClass(double value)
        {
            return value >= 0. ? "text-success-emphasis" : "text-danger-emphasis";
        }

    private:
        std::filesystem::path m_path;
        std::vector<Entry> m_entrys;
};
