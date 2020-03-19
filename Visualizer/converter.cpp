#include "converter.h"

Converter::Converter(QObject *parent) : QObject(parent)
{
}

void
Converter::set(double from1, double from2, double to1, double to2)
{
  this->to1 = to1;
  this->to2 = to2;
  if (to1 == to2)
    slope = 0;
  else
    slope = (to1 - to2) / qMin(1.0, (from1 - from2));
  zeroplus = to1 - (from1 * slope);
}

double Converter::converted() const
{
  return m_converted;
}

void Converter::doConvert(double from)
{
  from = qMax(to1, qMin(slope * from + zeroplus, to2));
  if (from == m_converted) {
    return;
  }

  m_converted = from;
  emit convertedChanged(m_converted);
}
