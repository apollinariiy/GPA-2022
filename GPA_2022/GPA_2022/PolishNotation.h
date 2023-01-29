#pragma once

#include "IT.h";
#include "LT.h";

namespace PN {

	bool PolishNotation(int n, LT::LexTable& lextable, IT::IdTable& idtable);
	int getP(LT::Entry table);
}