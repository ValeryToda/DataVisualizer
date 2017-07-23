#ifndef ENTRY_H
#define ENTRY_H
/**
 * entry.h
 * @author Valery Toda
 **/

#include <iostream>
#include <qstring.h>
#include <string.h>

using namespace std;

class Entry{
    
    friend std::ostream& operator<<(std::ostream&, const Entry&);

    QString _imagePathLeft;
    QString _imagePathCenter;
    QString _imagePathRight;
    float _steeringAngle;

public:
    Entry();
    Entry(const QString& imagePathLeft,
                    const QString& imagePathCenter,
                    const QString& imagePathRight,
                    float steeringAngle);

    Entry(const Entry &);
    bool operator==(const Entry&);
    bool operator!=(const Entry& entry);
    QString imagePathLeft() const {return _imagePathLeft;}
    QString imagePathRight() const {return _imagePathRight;}
    QString imagePathCenter() const {return _imagePathCenter;}
    float steeringAngle() const {return _steeringAngle;}
    const QString& toString() const;
};

#endif // ENTRY_H
