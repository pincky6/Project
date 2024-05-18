#ifndef SETTINGS_MODEL_H
#define SETTINGS_MODEL_H

#include <limits>

struct SettingsModel
{
    constexpr static int DEFAULT_RESOLUTION_X = 100;
    constexpr static int DEFAULT_RESOLUTION_Y = 100;
    constexpr static int DEFAULT_RESOLUTION_Z = 100;

    constexpr static int MODEL_WIDTH = 100;
    constexpr static int MODEL_LENGTH = 100;
    constexpr static int MODEL_HEIGHT= 100;

    constexpr static int DEFAULT_MODEL_SIZE = 5;
    constexpr static int DEFAULT_RECORDS_COUNT = std::numeric_limits<int>::max();

public:
    SettingsModel(int resolutionX_, int resolutionY_, int resolutionZ_,
                  int modelWidth_, int modelLength_, int modelHeight_,
                   int model2DRecordsCount_, int model3DRecordsCount_):
        resolutionX(resolutionX_), resolutionY(resolutionY_), resolutionZ(resolutionZ_),
        modelWidth(modelWidth_), modelLength(modelLength_), modelHeight(modelHeight_),
        model2DRecordsCount(model2DRecordsCount_), model3DRecordsCount(model3DRecordsCount_)
    {}
    SettingsModel(): resolutionX(DEFAULT_RESOLUTION_X), resolutionY(MODEL_LENGTH), resolutionZ(MODEL_HEIGHT),
                     modelWidth(DEFAULT_MODEL_SIZE), modelLength(DEFAULT_MODEL_SIZE), modelHeight(DEFAULT_MODEL_SIZE),
                     model2DRecordsCount(DEFAULT_RECORDS_COUNT), model3DRecordsCount(DEFAULT_RECORDS_COUNT)
    {}

public:
    int resolutionX;
    int resolutionY;
    int resolutionZ;

    int modelWidth;
    int modelLength;
    int modelHeight;

    int model2DRecordsCount;
    int model3DRecordsCount;
};

#endif
