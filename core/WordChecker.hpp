#ifndef WORDCHECKER_HPP
#define WORDCHECKER_HPP

#include <string>
#include <vector>
#include "Set.hpp"



class WordChecker
{
public:
    WordChecker(const Set<std::string>& words);

    // wordExists() returns true if the given word is spelled correctly,
    // false otherwise.
    bool wordExists(const std::string& word) const;


    // findSuggestions() returns a vector containing suggested alternative
    // spellings for the given word.
    std::vector<std::string> findSuggestions(const std::string& word) const;


private:
    const Set<std::string>& words;
	void swapAdjacent(const std::string& word, std::vector<std::string>& suggest) const;
	void addChars(const std::string& word, std::vector<std::string>& suggest) const;
	void delEach(const std::string& word, std::vector<std::string>& suggest) const;
	void repChar(const std::string& word, std::vector<std::string>& suggest) const;
	void splitWord(const std::string& word, std::vector<std::string>& suggest) const;
};



#endif // WORDCHECKER_HPP

