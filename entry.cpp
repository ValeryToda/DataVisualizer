#include "entry.h"
/**
 * entry.cpp
 * @author Valery Toda
 **/

Entry::Entry(){
    this->_imagePathLeft = "";
    this->_imagePathCenter = "";
    this->_imagePathRight = "";
    this->_steeringAngle = 0.0;
}

Entry::Entry(const QString& imagePathLeft, const QString& imagePathCenter,
                    const QString& imagePathRight, float steeringAngle)
{
    this->_imagePathLeft = imagePathLeft;
    this->_imagePathCenter = imagePathCenter;
    this->_imagePathRight = imagePathRight;
    this->_steeringAngle = steeringAngle;
}

bool Entry::operator == (const Entry& entry){
    return (_imagePathLeft==entry._imagePathLeft &&
            _imagePathCenter==entry._imagePathCenter &&
            _imagePathRight==entry._imagePathRight &&
            _steeringAngle==entry._steeringAngle);
}

bool Entry::operator != (const Entry& entry){
    return !((*this)==entry);
}

const QString& Entry::toString() const{
   QString *result = new QString(this->_imagePathLeft+", "+this->_imagePathCenter+", "+\
                    this->_imagePathRight+", "+ QString("%1").arg(this->_steeringAngle)+ "\n");
   return *result;
}

std::ostream& operator<<(std::ostream& os, const Entry& entry){
    QString result = entry._imagePathLeft+", "+entry._imagePathCenter+", "+ entry._imagePathRight+", "+ QString("%1").arg(entry._steeringAngle)+ "\n";
    return os <<  result.toStdString();
}

Entry::Entry(const Entry & entry){
    this->_imagePathCenter = entry._imagePathCenter;
    this->_imagePathLeft = entry._imagePathLeft;
    this->_imagePathRight = entry._imagePathRight;
    this->_steeringAngle = entry._steeringAngle;
}
