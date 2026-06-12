#pragma once
// encoding: UTF-8 with BOM
#include <string>
#include <vector>
#include "model/Sample.h"

class SampleView
{
public:
    void showMenu();
    void showSampleTable(const std::vector<Sample>& samples,
                         int page, int totalCount);
    void showRegisterSuccess(const std::wstring& sampleId);
    void showSearchResult(const std::vector<Sample>& results);
    int  promptChoice();
};