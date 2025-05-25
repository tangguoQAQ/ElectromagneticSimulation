#include <memory>
#include <vector>
#include <cmath>

#include "field.h"
#include "rect.h"
#include "particle.h"
#include "d2d_resource.h"

namespace simulation_app
{
    Field::Field(const Rect& range) : range_(range)
    {
    }
    
    bool Field::contains(Vector3d position)
    {
        return range_.contains(position.x, position.y);
    }


    ElectricField::ElectricField(const Rect& range, Vector3d E) : Field(range), E_(E)
    {
    }

    void ElectricField::addToArgs(FieldArgs& args) const
    {
        args.E += E_;
    }

    void renderElectricFieldLinesWithArrows(const Rect& rect, const Vector3d& E)
    {
        // 计算电场方向角度（忽略Z轴）
        float angle = atan2f(E.y, E.x);
        constexpr float arrowSize = 0.8f;
        
        // 计算电场线间距（均匀分布）
        constexpr float lineSpacing = 3.0f; // 可以调整这个值改变线密度 
        int lineCount = static_cast<int>((rect.right - rect.left - 1.5f)  / lineSpacing);
        
        // 根据电场方向决定是水平还是垂直分布电场线 
        bool isHorizontal = (fabsf(E.x) > fabsf(E.y));
        
        // 绘制每条电场线 
        for (int i = 0; i <= lineCount; ++i)
        {
            D2D1_POINT_2F start, end;
            
            if (isHorizontal) {
                // 水平分布电场线（电场主要是X方向）
                start = D2D1::Point2F(rect.left, rect.top + i * lineSpacing + 1.5f);
                end = D2D1::Point2F(rect.right, rect.top + i * lineSpacing + 1.5f);
            } else {
                // 垂直分布电场线（电场主要是Y方向）
                start = D2D1::Point2F(rect.left + i * lineSpacing + 1.5f, rect.top); 
                end = D2D1::Point2F(rect.left + i * lineSpacing + 1.5f, rect.bottom); 
            }
            
            // 绘制电场线 
            pRenderTarget->DrawLine(start, end, graphics::pFieldLineBrush, 0.1f);
            
            // 计算箭头位置（线段中点）
            D2D1_POINT_2F arrowPos = D2D1::Point2F(
                (start.x + end.x) * 0.5f,
                (start.y + end.y) * 0.5f);
            
            // 计算箭头两个点的位置（形成三角形）
            D2D1_POINT_2F arrow1 = D2D1::Point2F(
                arrowPos.x - arrowSize * cosf(angle + 0.3f),  // 0.3弧度≈17度 
                arrowPos.y - arrowSize * sinf(angle + 0.3f));
            
            D2D1_POINT_2F arrow2 = D2D1::Point2F(
                arrowPos.x - arrowSize * cosf(angle - 0.3f),
                arrowPos.y - arrowSize * sinf(angle - 0.3f));
            
            // 绘制箭头（两条线形成三角形）
            pRenderTarget->DrawLine(arrowPos, arrow1, graphics::pFieldLineBrush, 0.1f);
            pRenderTarget->DrawLine(arrowPos, arrow2, graphics::pFieldLineBrush, 0.1f);
        }
    }

    void ElectricField::render() const
    {
        graphics::pFieldFillBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow, 0.06f));
        pRenderTarget->FillRectangle(range_.toD2D(), graphics::pFieldFillBrush);
        renderElectricFieldLinesWithArrows(range_, E_);
    }

    
    MagneticField::MagneticField(const Rect& range, const Vector3d& B) : Field(range), B_(B)
    {
    }

    void MagneticField::addToArgs(FieldArgs& args) const
    {
        args.B += B_;
    }

    void renderMagneticField(const Rect& rect, const Vector3d& B)
    {
        bool isCross = (B.z > 0.0f);

        constexpr float cellSize = 3.0f;      // 单元格大小（控制符号间距）
        constexpr float symbolSize = 0.3f;     // 符号大小（线段长度的一半）
        constexpr float strokeWidth = 0.1f;    // 线段宽度

        for (float y = rect.top  + cellSize / 2; y < rect.bottom;  y += cellSize)
        {
            for (float x = rect.left  + cellSize / 2; x < rect.right;  x += cellSize)
            {
                if (isCross)
                {
                    // 绘制 ×（叉）：两条对角线
                    pRenderTarget->DrawLine(
                        D2D1::Point2F(x - symbolSize, y - symbolSize),
                        D2D1::Point2F(x + symbolSize, y + symbolSize),
                        graphics::pFieldLineBrush,
                        strokeWidth
                    );
                    pRenderTarget->DrawLine(
                        D2D1::Point2F(x + symbolSize, y - symbolSize),
                        D2D1::Point2F(x - symbolSize, y + symbolSize),
                        graphics::pFieldLineBrush,
                        strokeWidth
                    );
                }
                else
                {
                    // 绘制 ·（点）：十字线
                    pRenderTarget->FillEllipse( 
                        D2D1::Ellipse(D2D1::Point2F(x, y), symbolSize / 2.0f, symbolSize / 2.0f), 
                        graphics::pFieldLineBrush 
                    );
                }
            }
        }
    }

    void MagneticField::render() const
    {
        graphics::pFieldFillBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green, 0.06f));
        pRenderTarget->FillRectangle(range_.toD2D(), graphics::pFieldFillBrush);
        renderMagneticField(range_, B_);
    }

} // namespace simulation_app
