#include <memory>
#include <vector>
#include <cmath>
#include <bitset>

#include "field.h"
#include "rect.h"
#include "particle.h"
#include "d2d_resource.h"

namespace simulation_app
{
    Field::Field(const Rect& range) : range_(range)
    {
    }
    
    bool Field::contains(Vector3d position) const
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

    // 定义区域码的4个位：左、右、下、上
    const int LEFT = 0;   // 0001
    const int RIGHT = 1;  // 0010
    const int BOTTOM = 2; // 0100
    const int TOP = 3;    // 1000

    // 计算点的区域码
    std::bitset<4> ComputeOutCode(const D2D1_RECT_F& area, float x, float y) {
        std::bitset<4> code;
        
        // 设置区域码的各位
        if (x < area.left) 
            code.set(LEFT);    // 点在左边界左侧
        else if (x > area.right) 
            code.set(RIGHT);   // 点在右边界右侧
        
        if (y < area.top) 
            code.set(TOP);     // 点在上边界上方
        else if (y > area.bottom) 
            code.set(BOTTOM);  // 点在下边界下方
        
        return code;
    }

    // 使用Cohen-Sutherland算法裁剪线段
    bool CohenSutherlandLineClip(D2D1_RECT_F area, D2D1_POINT_2F& p1, D2D1_POINT_2F& p2) {
        std::bitset<4> code1 = ComputeOutCode(area, p1.x, p1.y);
        std::bitset<4> code2 = ComputeOutCode(area, p2.x, p2.y);
        bool accept = false;

        while (true) {
            // 情况1: 线段完全在裁剪区域内 (区域码都为0000)
            if ((code1 | code2).none()) {
                accept = true;
                break;
            }
            
            // 情况2: 线段完全在裁剪区域外 (区域码有相同位被设置)
            if ((code1 & code2).any()) {
                break;
            }
            
            // 情况3: 线段部分在裁剪区域内，需要裁剪
            std::bitset<4> code = code1.any() ? code1 : code2;
            D2D1_POINT_2F p;
            
            // 计算交点坐标
            if (code.test(LEFT)) { // 与左边界相交
                p.y = p1.y + (p2.y - p1.y) * (area.left - p1.x) / (p2.x - p1.x);
                p.x = area.left;
            } 
            else if (code.test(RIGHT)) { // 与右边界相交
                p.y = p1.y + (p2.y - p1.y) * (area.right - p1.x) / (p2.x - p1.x);
                p.x = area.right;
            } 
            else if (code.test(BOTTOM)) { // 与下边界相交
                p.x = p1.x + (p2.x - p1.x) * (area.bottom - p1.y) / (p2.y - p1.y);
                p.y = area.bottom;
            } 
            else if (code.test(TOP)) { // 与上边界相交
                p.x = p1.x + (p2.x - p1.x) * (area.top - p1.y) / (p2.y - p1.y);
                p.y = area.top;
            }
            
            // 用交点替换外部点
            if (code == code1) {
                p1 = p;
                code1 = ComputeOutCode(area, p1.x, p1.y);
            } else {
                p2 = p;
                code2 = ComputeOutCode(area, p2.x, p2.y);
            }
        }
        
        return accept;
    }

    void DrawUniformElectricField(
        ID2D1RenderTarget* pRenderTarget,
        ID2D1SolidColorBrush* pBrush,
        const D2D1_RECT_F& area,
        const Vector3d& fieldDirection
    )
    {
        constexpr float lineSpacing = 0.5f;
        constexpr float arrowSize = 0.1f;
        constexpr float strokeWidth = 0.02f;

        // 1. 计算电场方向单位向量
        const Vector3d dir = fieldDirection.normalize();
        
        // 2. 计算垂直于电场方向的单位向量 (逆时针旋转90度)
        const Vector3d perp{-dir.y, dir.x, 0.0};
        
        // 3. 计算矩形区域尺寸和中心点
        float width = area.right - area.left;
        float height = area.bottom - area.top;
        D2D1_POINT_2F center = {
            (area.left + area.right) * 0.5f,
            (area.top + area.bottom) * 0.5f
        };
        
        // 4. 计算电场线数量 (基于垂直方向投影)
        float spanX = std::abs(perp.x * width);
        float spanY = std::abs(perp.y * height);
        float totalSpan = spanX + spanY;
        int lineCount = static_cast<int>(totalSpan / lineSpacing) + 1;
        
        // 5. 绘制电场线
        for(int i = -lineCount/2; i <= lineCount/2; ++i)
        {
            // 计算当前电场线偏移量
            float offset = i * lineSpacing;
            
            // 计算电场线起点和终点 (沿垂直方向偏移)
            D2D1_POINT_2F start = {
                center.x + perp.x * offset - dir.x * width,
                center.y + perp.y * offset - dir.y * height
            };
            D2D1_POINT_2F end = {
                center.x + perp.x * offset + dir.x * width,
                center.y + perp.y * offset + dir.y * height
            };
            
            if(CohenSutherlandLineClip(area, start, end))
            {
                // 绘制电场线
                pRenderTarget->DrawLine(start, end, pBrush, strokeWidth);
                
                // 6. 计算箭头位置 (线段中点)
                D2D1_POINT_2F mid = {
                    (start.x + end.x) * 0.5f,
                    (start.y + end.y) * 0.5f
                };
                
                // 7. 计算箭头分支 (30度角)
                const float angle = 0.5236f; // 30度弧度值
                float cosA = std::cos(angle);
                float sinA = std::sin(angle);
                
                // 箭头分支1 (旋转+30度)
                D2D1_POINT_2F arrow1 = {
                    arrowSize * (-dir.x * cosA + dir.y * sinA),
                    arrowSize * (-dir.y * cosA - dir.x * sinA)
                };
                
                // 箭头分支2 (旋转-30度)
                D2D1_POINT_2F arrow2 = {
                    arrowSize * (-dir.x * cosA - dir.y * sinA),
                    arrowSize * (-dir.y * cosA + dir.x * sinA)
                };
                
                // 绘制箭头
                pRenderTarget->DrawLine(mid, D2D1::Point2F(mid.x + arrow1.x, mid.y + arrow1.y), 
                        pBrush, strokeWidth);
                pRenderTarget->DrawLine(mid, D2D1::Point2F(mid.x + arrow2.x, mid.y + arrow2.y), 
                        pBrush, strokeWidth);
            }
        }
    }

    void renderElectricFieldLinesWithArrows(const Rect& rect, const Vector3d& E)
    {
        // 计算电场方向角度（忽略Z轴）
        double angle = atan2(E.y, E.x);
        constexpr float arrowSize = 0.16f;
        
        // 计算电场线间距（均匀分布）
        constexpr float lineSpacing = 0.5f; // 可以调整这个值改变线密度 
        int lineCount = static_cast<int>((rect.right - rect.left - lineSpacing/2)  / lineSpacing);
        
        // 根据电场方向决定是水平还是垂直分布电场线 
        bool isHorizontal = (fabs(E.x) > fabs(E.y));
        
        // 绘制每条电场线 
        for (int i = 0; i <= lineCount; ++i)
        {
            D2D1_POINT_2F start, end;
            
            if (isHorizontal) {
                // 水平分布电场线（电场主要是X方向）
                start = D2D1::Point2F(rect.left, rect.top + i * lineSpacing + lineSpacing/2);
                end = D2D1::Point2F(rect.right, rect.top + i * lineSpacing + lineSpacing/2);
            } else {
                // 垂直分布电场线（电场主要是Y方向）
                start = D2D1::Point2F(rect.left + i * lineSpacing + lineSpacing/2, rect.top); 
                end = D2D1::Point2F(rect.left + i * lineSpacing + lineSpacing/2, rect.bottom); 
            }
            
            // 绘制电场线 
            pRenderTarget->DrawLine(start, end, graphics::pFieldLineBrush, 0.02f);
            
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
            pRenderTarget->DrawLine(arrowPos, arrow1, graphics::pFieldLineBrush, 0.02f);
            pRenderTarget->DrawLine(arrowPos, arrow2, graphics::pFieldLineBrush, 0.02f);
        }
    }

    void ElectricField::render() const
    {
        graphics::pFieldFillBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow, 0.06f));
        pRenderTarget->FillRectangle(range_.toD2D(), graphics::pFieldFillBrush);
        DrawUniformElectricField(pRenderTarget, graphics::pFieldLineBrush, range_.toD2D(), E_);
        // renderElectricFieldLinesWithArrows(range_, E_);
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
        const bool isCross = (B.z > 0.0f);

        constexpr float cellSize = 0.6f;
        constexpr float symbolSize = 0.06f;
        constexpr float strokeWidth = 0.02f;

        for (float y = rect.top + cellSize / 2; y < rect.bottom; y += cellSize)
        {
            for (float x = rect.left + cellSize / 2; x < rect.right; x += cellSize)
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
                    // 绘制 ·（点）：
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
