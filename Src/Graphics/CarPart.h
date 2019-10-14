#ifndef CARPART_H_INCLUDED
#define CARPART_H_INCLUDED

#include "Shader.h"

class CarPart {
public:
    virtual ~CarPart()=default;
    
    virtual void setPosition(float x, float y, float z)=0;
    virtual void addPositionXZ(const Vector2f& vect)=0;
    virtual void setScale(float x, float y, float z)=0;
    virtual void addScaleOrigin(float sca)=0;
    virtual void addRotationX(float bruh)=0;
    virtual void addRotationY(float bruh)=0;
    virtual void addRotationOriginY(float bruh)=0;
    virtual void addRotationZ(float bruh)=0;

    virtual void setShader(Shader* shd)=0;
    
    virtual bool isWheel() { return false; }

    virtual void render()=0;
    virtual void render(const Vector3f& origin)=0;
};

#endif // CARPART_H_INCLUDED
