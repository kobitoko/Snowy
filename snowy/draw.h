#pragma once
#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

class Camera;
class Screen;

class DebugDraw : public b2Draw {
public:
    DebugDraw();

    /// DebugDraw needs MtoPix value to convert metrics to pixel, the Screen pointer for rendering to it,
    /// and the Camera for camera offset values so it renders at the correct place.
    DebugDraw(float MtoPix, Screen* screen, Camera* camz);

    ~DebugDraw(){}

    /// Draw a closed polygon provided in CCW order.
    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);

    /// Draw a solid closed polygon provided in CCW order.
    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);

    void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color);

    void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color);

    /// Draw a particle array.
    void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count);

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);

    /// A transform contains translation and rotation. It is used to represent the position and orientation of rigid frames.
    void DrawTransform(const b2Transform &xf);

private:
    void rendTargetSet();
    float mToP;
    Uint8 alphaColor;
    Screen* scrn;
    SDL_Renderer*  scrRendr;
    Camera* camer;
};

