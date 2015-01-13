#include <gtest/gtest.h>
#include "../src/BilinearInterpolator.h"

class WikipediaDataSourceStub : public Source<QColor>
{
    public:
        virtual QColor GetValue(int x, int y) const override
        { 
            /*if (x == 14)
                return y == 20 ? QColor(91, 91, 91, 91) : QColor(162, 162, 162, 162);
            else
                return y == 20 ? QColor(210, 210, 210, 210) : QColor(95, 95, 95, 95);*/
            if (x == 14)
            {
                if (y == 20)
                    return QColor(91,91,91,91);
                else
                    return QColor(162, 162,162,162);
            }
            else
            {
                if (y == 20)
                    return QColor(210,210,210,210);
                else
                    return QColor(95,95,95,95);
            }
        }
};

TEST(BilinearInterpolatorTests, TestBilinearInterpolatorWithDataFromWikipedia)
{
    BilinearInterpolator<QColor> interpolator;
    WikipediaDataSourceStub source;
    int expectedColorAlpha = 146;
    int expectedColorRed = 146;
    int expectedColorGreen = 146;
    int expectedColorBlue = 146;

    QColor result = interpolator.Interpolate(source, 14.5, 20.2);

    EXPECT_EQ(expectedColorRed, result.red());
    EXPECT_EQ(expectedColorBlue, result.blue());
    EXPECT_EQ(expectedColorGreen, result.green());
    EXPECT_EQ(expectedColorAlpha, result.alpha());
}

