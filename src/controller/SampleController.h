#pragma once
// encoding: UTF-8 with BOM
#include "service/SampleService.h"

class SampleController
{
public:
    void run(SampleService& sampleService);

private:
    static constexpr int PAGE_SIZE = 5;
    void handleRegister(SampleService& sampleService);
    void handleList    (SampleService& sampleService);
    void handleSearch  (SampleService& sampleService);
};