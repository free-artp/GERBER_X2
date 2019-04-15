#include "exfile.h"

using namespace Excellon;

Excellon::DrillFile::DrillFile()
    : m_format(this)
{
}

Format DrillFile::format() const
{
    return m_format;
}

void DrillFile::setFormat(const Format& value)
{
    m_format = value;
    //    m_format.zeroMode = value.zeroMode;
    //    m_format.unitMode = value.unitMode;
    //    m_format.decimal = value.decimal;
    //    m_format.integer = value.integer;
    //    m_format.offsetPos = value.offsetPos;
    for (Hole& hole : *this) {
        hole.state.updatePos();
        hole.item->updateHole();
    }
}

void DrillFile::setFormatForFile(const Format& value)
{
    QList<QString> lines;
    QFile file(fileName());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }
}

double DrillFile::tool(int t) const
{
    double tool = 0.0;
    if (m_tools.contains(t)) {
        tool = m_tools[t];
        if (m_format.unitMode == Inches)
            tool *= 25.4;
    }
    return tool;
}

QMap<int, double> DrillFile::tools() const
{
    QMap<int, double> tools(m_tools);
    QMap<int, double>::iterator toolIt;
    if (m_format.unitMode == Inches)
        for (toolIt = tools.begin(); toolIt != tools.end(); ++toolIt)
            toolIt.value() *= 25.4;
    return tools;
}

Paths Excellon::DrillFile::merge() const
{
    for (GraphicsItem* item : *m_itemGroup.data())
        m_mergedPaths.append(item->paths());
    return m_mergedPaths;
}
