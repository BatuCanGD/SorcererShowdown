#pragma once
#include "code/header/CursedTools/CursedTool.h"

class InvertedSpearofHeaven final : public CursedTool {
public:
	InvertedSpearofHeaven();
	std::unique_ptr<CursedTool> Clone() const override;
};