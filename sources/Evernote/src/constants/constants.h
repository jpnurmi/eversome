#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

class Constants : public QObject
{
    Q_OBJECT
public:
    static const std::string EDAM_HOST;
    static const std::string EDAM_USER_ROOT;
    static const std::string EDAM_NOTE_ROOT;
private:
    explicit Constants(QObject *parent = 0);

signals:

public slots:

};

#endif // CONSTANTS_H
