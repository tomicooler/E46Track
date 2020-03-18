#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

class Converter : public QObject
{
  Q_OBJECT

  Q_PROPERTY(double converted READ converted WRITE doConvert NOTIFY convertedChanged);

public:
  explicit Converter(QObject *parent = nullptr);
  Q_INVOKABLE void set(double from1, double from2, double to1, double to2);

  double converted() const;

signals:
  void convertedChanged(double converted);

public slots:
  void doConvert(double from);

private:
  double to1{0.0};
  double to2{0.0};

  double zeroplus{0.0};
  double slope{0.0};

  double value{0.0};
  double m_converted;
};

#endif // CONVERTER_H
