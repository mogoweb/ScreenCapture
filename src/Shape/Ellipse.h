#pragma once
#include "include/core/SkSurface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkRect.h"
#include "include/core/SkPoint.h"
#include "include/core/SkPath.h"
#include "ShapeBase.h"
#include <vector>

class Ellipse : public ShapeBase
{
public:
    Ellipse(const int &x, const int &y);
    ~Ellipse();
    bool OnMouseDown(const int &x, const int &y) override;
    bool OnMouseUp(const int &x, const int &y) override;
    bool OnMouseMove(const int &x, const int &y) override;
    bool OnPaint(SkCanvas *canvas) override;
    bool OnMoseDrag(const int &x, const int &y) override;

private:
    void setCursor();
    void initParams();
    bool showDragger{false};
    std::vector<SkRect> draggers;
    SkRect rect;
    bool stroke{true};
    int strokeWidth{4};
    SkColor color{SkColorSetARGB(255, 207, 19, 34)};
};