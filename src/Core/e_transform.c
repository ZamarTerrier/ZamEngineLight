#include "Core/e_transform.h"

#include "Tools/e_math.h"

#include "Objects/gameObject2D.h"
#include "Objects/gameObject3D.h"

#include "Data/e_resource_data.h"
#include "Data/e_resource_engine.h"

extern ZEngine engine;

void Transform2DInit(Transform2D* transform){

    memset(transform, 0, sizeof(Transform2D));

    transform->scale.x = 1;
    transform->scale.y = 1;
    
    transform->img.scale.x = 1;
    transform->img.scale.y = 1;

    transform->scaleOrig.x = engine.width;
    transform->scaleOrig.y = engine.height;

}

void Transform3DInit(Transform3D* transform){

    memset(transform, 0, sizeof(Transform3D));

    transform->scale.x = 1;
    transform->scale.y = 1;
    transform->scale.z = 1;

    transform->img.scale.x = 1;
    transform->img.scale.y = 1;

}

//2D Transforms
void Transform2DSetPosition(struct GameObject2D_T* go, float x, float y)
{

    GameObject2D *temp = (GameObject2D *)go;

    temp->transform.positionOrig.x = x;
    temp->transform.positionOrig.y = y;

    if(x != 0)
        x /= engine.width;

    if(y != 0)
        y /= engine.height;

    temp->transform.position.x = x;
    temp->transform.position.y = y;
}

void Transform2DReposition(struct GameObject2D_T* go)
{
    GameObject2D *temp = (GameObject2D *)go;
    temp->transform.position.x = temp->transform.positionOrig.x / (engine.width) / 2;
    temp->transform.position.y = temp->transform.positionOrig.y / (engine.height) / 2;
}

vec2 Transform2DGetPosition(struct GameObject2D_T* go)
{
    GameObject2D *temp = (GameObject2D *)go;

    vec2 pos = temp->transform.position;

    pos.x *= engine.width;
    pos.y *= engine.height;

    return pos;
}


void Transform2DSetImageOffset(struct GameObject2D_T* go, float x, float y)
{
    GameObject2D *temp = (GameObject2D *)go;

    temp->transform.img.offset.x = x / temp->image->imgWidth;
    temp->transform.img.offset.y = y / temp->image->imgHeight;
}

vec2 Transform2DGetImageOffset(struct GameObject2D_T* go)
{
    GameObject2D *temp = (GameObject2D *)go;

    vec2 offset;
    offset.x = temp->image->imgWidth * temp->transform.img.offset.x;
    offset.y = temp->image->imgHeight * temp->transform.img.offset.y;

    return offset;
}

void Transform2DSetScale(struct GameObject2D_T* go, float x, float y)
{
    GameObject2D *temp = (GameObject2D *)go;
    temp->transform.scaleOrig.x = x;
    temp->transform.scaleOrig.y = y;

    x /= engine.width;
    y /= engine.height;

    temp->transform.scale.x = x / 2;
    temp->transform.scale.y = y / 2;
}

void Transform2DRescale(struct GameObject2D_T* go)
{
    GameObject2D *temp = (GameObject2D *)go;
    temp->transform.scale.x = temp->transform.scaleOrig.x / (engine.width) / 2;
    temp->transform.scale.y = temp->transform.scaleOrig.y / (engine.height) / 2;
}

vec2 Transform2DGetScale(struct GameObject2D_T* go)
{
    GameObject2D *temp = (GameObject2D *)go;

    vec2 size;
    size.x = temp->transform.scale.x * (engine.width) * 2;
    size.y = temp->transform.scale.y * (engine.height) * 2;

    return size;
}

//3D Transforms
void Transform3DSetPositionT(Transform3D* transform, float x, float y, float z)
{
    transform->position.x = x;
    transform->position.y = y;
    transform->position.z = z;
}

void Transform3DSetPosition(struct GameObject3D_T* go, float x, float y, float z)
{
    GameObject3D *temp = (GameObject3D *)go;

    temp->transform.position.x = x;
    temp->transform.position.y = y;
    temp->transform.position.z = z;
}

vec3 Transform3DGetPosition(struct GameObject3D_T* go)
{
    GameObject3D *temp = (GameObject3D *)go;

    return temp->transform.position;
}

void Transform3DSetScaleT(Transform3D* transform, float x, float y, float z)
{
    transform->scale.x = x;
    transform->scale.y = y;
    transform->scale.z = z;
}

void Transform3DSetScale(struct GameObject3D_T* go, float x, float y, float z)
{
    GameObject3D *temp = (GameObject3D *)go;

    temp->transform.scale.x = x;
    temp->transform.scale.y = y;
    temp->transform.scale.z = z;
}

vec3 Transform3DGetScale(struct GameObject3D_T* go)
{
    GameObject3D *temp = (GameObject3D *)go;
    return temp->transform.scale;
}

void Transform3DSetRotateT(Transform3D* transform, float x, float y, float z)
{
    transform->rotation.x = x;
    transform->rotation.y = y;
    transform->rotation.z = z;
}

void Transform3DSetRotate(struct GameObject3D_T* go, float x, float y, float z){

    GameObject3D *temp = (GameObject3D *)go;

    x = x > 360 ? 0 : x;
    x = x < -360 ? 0 : x;

    y = y > 360 ? 0 : y;
    y = y < -360 ? 0 : y;

    z = z > 360 ? 0 : z;
    z = z < -360 ? 0 : z;

    temp->transform.rotation.x = x;
    temp->transform.rotation.y = y;
    temp->transform.rotation.z = z;
}

vec3 Transform3DGetRotate(struct GameObject3D_T* go){

    GameObject3D *temp = (GameObject3D *)go;

    return temp->transform.rotation;
}
