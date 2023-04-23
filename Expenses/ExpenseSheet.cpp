#include "ExpenseSheet.h"

void ExpenseSheet::Entry::Serialize(std::ostream& out) const
{
    out.write(label.c_str(), label.length() + 1);
    out.write((char*)&value, sizeof(double));
}

void ExpenseSheet::Entry::Deserialize(std::istream& in)
{
    std::stringstream ss;
    char c;
    do
    {
        in.read(&c, 1);
        if(c != '\0')
            ss << c;
    } while (c != '\0');

    label = ss.str();
    in.read((char*)&value, sizeof(double));
}

bool ExpenseSheet::Open(const std::filesystem::path& dataFile)
{
    auto path = dataFile;
    path.replace_extension(".fexc");
    std::ifstream fileIn(path, std::ios::in | std::ios::binary);
    if (fileIn.is_open())
    {
        m_path = dataFile;

        size_t elementCount = 0;
        fileIn.read((char*)&elementCount, sizeof(size_t));

        m_entrys.clear();

        for (size_t i = 0; i < elementCount; i++)
        {
            Entry e;
            e.Deserialize(fileIn);
            m_entrys.push_back(std::move(e));
        }

        return true;
    }
    return false;
}

bool ExpenseSheet::Save(const std::filesystem::path& dataFile) const
{
    auto xpath = dataFile;

    if (dataFile.empty())
    {
        if (m_path.empty())
        {
            return false;
        }
        else
        {
            xpath = m_path;
        }
    }

    auto path = xpath;
    path.remove_filename();
    if(!path.empty())
        std::filesystem::create_directories(path);

    xpath.replace_extension(".fexc");

    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc | std::ios::binary);
    if (fileOut.is_open())
    {
        size_t elementCount = m_entrys.size();
        fileOut.write((char*)&elementCount, sizeof(size_t));
        for (const Entry& e : m_entrys)
        {
            e.Serialize(fileOut);
        }
        return true;
    }
    return false;
}

void ExpenseSheet::New()
{
    m_path = "";
    m_entrys.clear();
}

bool ExpenseSheet::Add(std::string_view label, double value)
{
    Entry e;
    e.label = label;
    e.value = value;

    bool canInsert = std::find(m_entrys.begin(), m_entrys.end(), e) == m_entrys.end();
    if (canInsert)
    {
        m_entrys.push_back(std::move(e));
    }

    return canInsert;
}

bool ExpenseSheet::Del(std::string_view label)
{
    Entry e;
    e.label = label;
    auto element = std::find(m_entrys.begin(), m_entrys.end(), e);
    if (element != m_entrys.end())
    {
        m_entrys.erase(element);
        return true;
    }

    return false;
}

void ExpenseSheet::List(std::ostream& os) const
{
    size_t idx = 0;
    for (const Entry& e : m_entrys)
    {
        os << "(" << idx++ << ") " << e.label << " " << e.value << std::endl;
    }
}

double ExpenseSheet::Eval() const
{
    double value = 0.;
    for (const Entry& e : m_entrys)
    {
        value += e.value;
    }
    return value;
}
