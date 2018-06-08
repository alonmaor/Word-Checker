
#include "WordChecker.hpp"
#include <iostream>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> suggest;
    swapAdjacent(word, suggest);
    addChars(word,suggest);
    delEach(word,suggest);
	repChar(word,suggest);
    splitWord(word, suggest);
    return suggest;
}

void WordChecker::swapAdjacent(const std::string& word, std::vector<std::string>& suggest) const
{
    for(int i = 0; i < word.size()-1; i++)
    {
        std::string s = word;
        std::swap(s[i],s[i+1]);
        if(words.contains(s))
            suggest.push_back(s);
    }
}


void WordChecker::addChars(const std::string& word, std::vector<std::string>& suggest) const
{
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYX";
    for(int i = 1; i < word.size(); i++)
	{
		std::string s = word;
        s.insert(i, "0");
        for(char j = 0; j < alphabet.size(); j++)
        {   
			s[i] = alphabet[j];
            if(words.contains(s))
                suggest.push_back(s);
        }
	}
}


void WordChecker::delEach(const std::string& word, std::vector<std::string>& suggest) const
{
    for(int i = 0; i < word.size(); i++)
    {
		if(word[i] == word[i+1] && i < word.size()-1)
        	continue;
        std::string s = word;
        s.erase(s.begin()+i);
        if(words.contains(s))
            suggest.push_back(s);
    }
}

void WordChecker::repChar(const std::string& word, std::vector<std::string>& suggest) const
{
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYX";
	for(int i = 0; i < word.size(); i++)
    {
		std::string s = word;
        for(char j = 0; j < alphabet.size(); j++)
        {
           	s[i] = alphabet[j]; 
            if(words.contains(s))
                suggest.push_back(s);
        }
    }
}

void WordChecker::splitWord(const std::string& word, std::vector<std::string>& suggest) const
{
	for(int i = 1; i < word.size(); i++)
	{
		if(words.contains(word.substr(0,i)))
			if(words.contains(word.substr(i)))
			{
                std::string s = word;
                s.insert(i, " ");
                suggest.push_back(s);
			}
	}
}
