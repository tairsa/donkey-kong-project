#pragma once

#include <limits>
#include <fstream>
#include <list>

class Results {
public:
	enum ResultValue { died, finished, noResult };
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename,int score) const;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || results.back().first <= iteration;
	}
	size_t getNextDeathIteration() const;

	void clearResults() {
		results.clear();  // Clears the list of results
	}
};
