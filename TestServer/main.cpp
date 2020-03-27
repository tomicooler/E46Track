#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QTcpServer server;

  static const auto dsc_steering = QByteArray::fromHex("b829f1032201f5b5");
  static const auto dsc_brake = QByteArray::fromHex("b829f102210645");
  static const auto dsc_offsets = QByteArray::fromHex("b829f102210241");
  static const auto motronic_m3_a = QByteArray::fromHex("12050b031f");
  static const auto motronic_m3_b = QByteArray::fromHex("12050b130f");

  static const auto extraFramingHeader = QByteArray::fromHex("000212c02100003c00");

  QObject::connect(&server, &QTcpServer::newConnection, [&](){
    QTcpSocket *socket = server.nextPendingConnection();
    QObject::connect(socket, &QAbstractSocket::disconnected, socket, &QObject::deleteLater);
    QObject::connect(socket, &QTcpSocket::readyRead, [&, socket](){
      auto request = socket->readAll();
      if (request.startsWith(extraFramingHeader)) {
        request = request.mid(extraFramingHeader.size() + 1, request.size() - extraFramingHeader.size() - 1 - 1);
      }

      if (request == motronic_m3_a) {
        //socket->write(QByteArray::fromHex("1227a0000003b600000000000069858b65073a7b8181a60008000440043303e803f603e8635c73")); // DKP 100 Throttle 3.95
        //socket->write(QByteArray::fromHex("1227a0000003b600000000000069858b65073a7c8181a600080001e201dd0223022f0223635ce0")); // DKP 54.7 Throttle 2.17
        //socket->write(QByteArray::fromHex("1227a0035d03660054043d00df66828b660a3a898181a6003600000000000000000000005d5b3a")); // RPM 861
        //socket->write(QByteArray::fromHex("1227a00ceb036600b1024d007966808c6a0e3a888181a600810000000000000100000000635c35")); // RPM 3307
        socket->write(QByteArray::fromHex("1227a01fe7036606d1094d01e940848c5f2e3c888181a1087100043b043300d000c100d3505be7")); // RPM 8167
      } else if (request == motronic_m3_b) {
        socket->write(QByteArray::fromHex("125ea00037037000000f730f560f740f7f0f500f9e0f7a0f7a012a012a012a012a012a012a000000007f3c7f1881a200000000000200000000000000000000fb0500000000000000000000000a23127200007276011330000000000000dc")); // 55 km/h
      } else if (request == dsc_steering) {
        socket->write(QByteArray::fromHex("b8f1290c6201f5010110dd8180810000b7"));  // angle 184.365
        //socket->write(QByteArray::fromHex("b8f1290c6201f501728000008091000018")); // -5.13
      } else if (request == dsc_brake) {
        socket->write(QByteArray::fromHex("b8f1290f6106ff00c4dc228122acfe46fff8126f")); // brake 88.33
        //socket->write(QByteArray::fromHex("b8f1290f61061800c6dcffa5ff65fe46000c126c")); // brake 0.0
        //socket->write(QByteArray::fromHex("b8f1290f6106ff00c5dc0336030bfe460004137c")); // brake 8.22

        //socket->write(QByteArray::fromHex("b8f1290f6106ff00c3dcffa5ff6533d501c3131f")); // brake -0.91 yaw 37.816 latg 0.45
        //socket->write(QByteArray::fromHex("b8f1290f6106ff00c4dcffbaff65d296febb1322")); // brake -0.7 yaw -33.134 latg -0.325
      } else if (request == dsc_offsets) {
        socket->write(QByteArray::fromHex("b8f1290c6102fb62f680fbc007640000b8"));
      }

    });
  });

  server.listen(QHostAddress::Any, 35000);

  return a.exec();
}
