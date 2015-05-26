#include <cmath>
#include "draw.h"
#include "screen.h"
#include "camera.h"
// SDL2_gfx by ferzkopp http://cms.ferzkopp.net/index.php/software/13-sdl-gfx
#include <SDL2/SDL2_gfxPrimitives.h>

DebugDraw::DebugDraw() : mToP(10.0f), scrn(nullptr), scrRendr(nullptr), camer(nullptr), alphaColor(255) {}

DebugDraw::DebugDraw(float MtoPix, Screen* screen, Camera* camz) : mToP(MtoPix), scrn(screen), camer(camz),
alphaColor(180), scrRendr(scrn->getRenderer()) {}

void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        Sint16* vx = new Sint16[vertexCount];
    Sint16* vy = new Sint16[vertexCount];
    for(size_t i = 0; i < vertexCount; ++i) {
        vx[i] = (mToP * vertices[i].x) + camer->getCamOffset().first;
        vy[i] = (mToP * vertices[i].y) + camer->getCamOffset().second;
    }
    polygonRGBA(scrRendr, vx, vy, vertexCount,
                static_cast<Uint8>(255 * color.r),
                static_cast<Uint8>(255 * color.g),
                static_cast<Uint8>(255 * color.b),
                alphaColor);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
    Sint16* vx = new Sint16[vertexCount];
    Sint16* vy = new Sint16[vertexCount];
    for(size_t i = 0; i < vertexCount; ++i) {
        vx[i] = (mToP * vertices[i].x) + camer->getCamOffset().first;
        vy[i] = (mToP * vertices[i].y) + camer->getCamOffset().second;
    }
    filledPolygonRGBA(scrRendr, vx, vy, vertexCount,
                      static_cast<Uint8>(255 * color.r),
                      static_cast<Uint8>(255 * color.g),
                      static_cast<Uint8>(255 * color.b),
                      alphaColor);
}

void DebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {
    circleRGBA(scrRendr,
               mToP * center.x + camer->getCamOffset().first,
               mToP * center.y + camer->getCamOffset().second,
               mToP * radius,
               static_cast<Uint8>(255 * color.r),
               static_cast<Uint8>(255 * color.g),
               static_cast<Uint8>(255 * color.b),
               alphaColor);
}

void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
    filledCircleRGBA(scrRendr,
                     mToP * center.x + camer->getCamOffset().first,
                     mToP * center.y + camer->getCamOffset().second,
                     mToP * radius,
                     static_cast<Uint8>(255 * color.r),
                     static_cast<Uint8>(255 * color.g),
                     static_cast<Uint8>(255 * color.b),
                     alphaColor);
}

// 	Draw a particle array.
void DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) {
    // b2Color has a range of 0-1.
    b2Color colr = {1, 1, 1};
    if(colors != nullptr) {
        colr = colors->GetColor();
    }
    for(int32 i = 0; i < count; ++i) {
        DrawSolidCircle(centers[i], radius, centers[i], colr);
    }
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
    SDL_SetRenderDrawColor(scrRendr,
                           static_cast<Uint8>(255 * color.r),
                           static_cast<Uint8>(255 * color.g),
                           static_cast<Uint8>(255 * color.b),
                           alphaColor);
    SDL_RenderDrawLine(scrRendr,
                       mToP * p1.x + camer->getCamOffset().first ,
                       mToP * p1.y + camer->getCamOffset().second,
                       mToP * p2.x + camer->getCamOffset().first,
                       mToP * p2.y + camer->getCamOffset().second);
}

void DebugDraw::DrawTransform(const b2Transform &xf){
    SDL_SetRenderDrawColor(scrRendr,
                           static_cast<Uint8>(0),
                           static_cast<Uint8>(0),
                           static_cast<Uint8>(0),
                           alphaColor);
    float ang = xf.q.GetAngle();
    float xAng = cosf(ang);
    float yAng = sinf(ang);
    b2Vec2 tmpVec;
    tmpVec.x = (mToP * (xf.p.x + xAng)) + camer->getCamOffset().first ;
    tmpVec.y = (mToP * (xf.p.y + yAng)) + camer->getCamOffset().second;
    SDL_RenderDrawLine(scrRendr,
                       (mToP * xf.p.x) + camer->getCamOffset().first,
                       (mToP * xf.p.y) + camer->getCamOffset().second,
                       tmpVec.x , tmpVec.y);
}
