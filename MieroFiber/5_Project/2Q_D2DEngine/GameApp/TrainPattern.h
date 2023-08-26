#pragma once

enum class TRAIN_PATTERN_TYPE
{
    NONE,
    PATTERN_1,
    PATTERN_2,
    PATTERN_3,

    END
};

class TrainPattern
{
public:
    TRAIN_PATTERN_TYPE m_PatternType = TRAIN_PATTERN_TYPE::NONE;

};