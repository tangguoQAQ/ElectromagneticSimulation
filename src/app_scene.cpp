#include <iostream>
#include <utility>
#include <cmath>
#include <map>
#include <string>

#include "app_scene.h"
#include "scene.h"
#include "barrier.h"
#include "particle.h"
#include "field.h"
#include "rect.h"
#include "vector3d.h"
#include <memory>
#include "d2d_resource.h"

namespace simulation_app
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    SceneManager sceneManager{scene};
    double U{3000},
            B{std::sqrt((2.0 * U * 46 * constant::u) / constant::e) / 2.0};

    bool hasInitialized = false;

    void generateFieldAndBarrier();
    void generateIons();
    void onParticleFixed(std::shared_ptr<Particle> particle);

    void initializeScene()
    {
        if(hasInitialized)
        {
            return;
        }
        hasInitialized = true;
        
        generateFieldAndBarrier();
        generateIons();
        
        sceneManager.start();
    }

    void generateFieldAndBarrier()
    {
        // 加速电场
        scene->addField(std::make_shared<ElectricField>(
                Rect{3.0, 2.0, 5.0, 2.5}, Vector3d{0.0, U / 0.5, 0.0}));
        
        // 偏转磁场
        scene->addField(std::make_shared<MagneticField>(
                Rect{0.0, 2.7, 6.0, 8.0}, Vector3d{0.0, 0.0, -B}));
        
        scene->addBarrier(std::make_shared<Barrier>(
                Vector3d{0.0, 2.65, 0.0}, Vector3d{3.95, 2.65, 0.0}, onParticleFixed));
    }

    void generateIons()
    {
        const struct {
            double massToChargeRatio, relativeAbundance;
        } ionsDatas[] = {
            {14.0, 1.4},
            {15.0, 3.4},
            {19.0, 2.3},
            {26.0, 4.9},
            {27.0, 17.7},
            {28.0, 4.2},
            {29.0, 12.0},
            {30.0, 5.0},
            {31.0, 100.0},
            {32.0, 1.4},
            {41.0, 1.0},
            {42.0, 3.4},
            {43.0, 9.9},
            {44.0, 1.0},
            {45.0, 57.3},
            {46.0, 24.6}
        };

        for(const auto& ion : ionsDatas)
        {
            for(int i = 0; i < static_cast<int>(ion.relativeAbundance * 10); ++i)
            {
                scene->addParticle(std::make_shared<Particle>(
                        Vector3d{4.0, 2.0, 0.0}, Vector3d{}, ion.massToChargeRatio));
            }
        }
    }

    std::map<double, int> result{};
    std::map<double, double> relativeAbundanceMap{};

    void calcRelativeAbundance();

    void onParticleFixed(std::shared_ptr<Particle> particle)
    {
        const double r = (4.0 - particle->getPos().x) / 2.0;
        const double massToChargeRatio = (r*r * B*B) / (2*U) / constant::u * constant::e;
        result[massToChargeRatio]++;

        calcRelativeAbundance();
    }

    void calcRelativeAbundance()
    {
        int maxCount = 0;
        for(const auto& [massToChargeRatio, count] : result)
        {
            if(count > maxCount)
            {
                maxCount = count;
            }
        }
        
        for(const auto& [massToChargeRatio, count] : result)
        {
            relativeAbundanceMap[massToChargeRatio] = static_cast<double>(count) / maxCount;
        }
    }

    void uninitializeScene()
    {
        sceneManager.stop();
        hasInitialized = false;
    }

    void renderRelativeAbundanceChart();

    void renderScene()
    {
        // scene->dump();
        sceneManager.render();
        renderRelativeAbundanceChart();
    }

    void renderRelativeAbundanceChart()
    {
        // 绘制图表框
        Rect rect{7, 2, 12, 5};
        const double chartWidth = 12 - 7 - 0.3, chartHeight = 5 - 2 - 0.1;
        pRenderTarget->DrawRectangle(rect.toD2D(), graphics::pBlackBrush, 0.01f);

        // 绘制标尺
        pRenderTarget->DrawTextW(L"m/z", 3, graphics::pDefaultTextFormat,
                D2D1::RectF(rect.right - 0.2, rect.bottom + 0.03, rect.right + 0.1, rect.bottom + 0.03), graphics::pBlackBrush);
        const double xSrart = 10, xEnd = 50, xStep = 5, xRange = xEnd - xSrart;
        for(double x = xSrart; x <= xEnd; x += xStep)
        {
            const float xPos = rect.left + (x - xSrart)/xRange * chartWidth;
            pRenderTarget->DrawLine(D2D1::Point2F(xPos, rect.bottom + 0.03), D2D1::Point2F(xPos, rect.bottom),
                    graphics::pBlackBrush, 0.01f);
            const std::wstring xText = std::to_wstring(static_cast<int>(x));
            pRenderTarget->DrawTextW(xText.c_str(), xText.size(), graphics::pDefaultTextFormat,
                    D2D1::RectF(xPos - 0.1, rect.bottom + 0.08, xPos + 0.1, rect.bottom + 0.08), graphics::pBlackBrush);
        }
        
        for(float y = 0.0f; y <= 1.0f; y += 0.2f)
        {
            const float yPos = rect.bottom - y * chartHeight;
            pRenderTarget->DrawLine(D2D1::Point2F(rect.left - 0.03, yPos), D2D1::Point2F(rect.left, yPos),
                    graphics::pBlackBrush, 0.01f);
            const std::wstring yText = std::to_wstring(static_cast<int>(y * 100));
            pRenderTarget->DrawTextW(yText.c_str(), yText.size(), graphics::pDefaultTextFormat,
                    D2D1::RectF(rect.left - 0.25, yPos - 0.1, rect.left, yPos + 0.1), graphics::pBlackBrush);
        }

        // 绘制数据
        for(const auto& [massToChargeRatio, relativeAbundance] : relativeAbundanceMap)
        {
            pRenderTarget->DrawLine(
                D2D1::Point2F(
                    rect.left + (massToChargeRatio - xSrart)/xRange * chartWidth,
                    rect.bottom - relativeAbundance * chartHeight),
                D2D1::Point2F(rect.left + (massToChargeRatio - xSrart)/xRange * chartWidth, rect.bottom),
                graphics::pBlackBrush, 0.01f);
        }
    }
} // namespace simulation_app
