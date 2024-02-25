#pragma once

struct BaseFilter;

struct FilterComponent 
{
	// Applied by increasing orders. If twice is the same, first defined goes first. (maybe)
	int order = 0; 
	bool enabled = false;
	BaseFilter* filter = nullptr; // find something smarter
};