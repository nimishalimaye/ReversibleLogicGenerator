// ReversibleLogicGenerator - generator of reversible logic circuits, based on permutation group theory.
// Copyright (C) 2015  <Dmitry Zakablukov>
// E-mail: dmitriy.zakablukov@gmail.com
// Web: https://github.com/dmitry-zakablukov/ReversibleLogicGenerator
// 
// This file is part of ReversibleLogicGenerator.
// 
// ReversibleLogicGenerator is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// ReversibleLogicGenerator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with ReversibleLogicGenerator.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

typedef vector<word> TruthTable;

// debug assert
#if defined(DEBUG) || defined(_DEBUG)
    #define assertd(condition, message)  if(!(condition)) throw AssertionError( move((message)) );
#else
    #define assertd(condition, message)
#endif  // DEBUG

// assert
#define assert(condition, message)  if(!(condition)) throw AssertionError( move((message)) );

// format assert
#define assertFormat(condition) if(!(condition)) throw InvalidFormatException();


#define forin(name, collection) for(auto (name) = (collection).begin(); (name) != (collection).end(); ++(name))
#define forcin(name, collection) for(auto (name) = (collection).cbegin(); (name) != (collection).cend(); ++(name))

// reversed
#define forrin(name, collection) for(auto (name) = (collection).rbegin(); (name) != (collection).rend(); ++(name))
#define forrcin(name, collection) for(auto (name) = (collection).crbegin(); (name) != (collection).crend(); ++(name))

uint countNonZeroBits(word value);
uint findPositiveBitPosition(word value, uint startPos = 0);
uint getSignificantBitCount(word value);

template< typename T >
deque<T> conjugate(deque<T> target, deque<T> conjugations, bool withReverse = false)
{
    deque<T> implementation;

    if(withReverse)
    {
        implementation.insert(implementation.end(), conjugations.crbegin(),
                              conjugations.crend());

        implementation.insert(implementation.end(), target.cbegin(),
                              target.cend());

        implementation.insert(implementation.end(), conjugations.cbegin(),
                              conjugations.cend());
    }
    else
    {
        implementation.insert(implementation.end(), conjugations.cbegin(),
                              conjugations.cend());

        implementation.insert(implementation.end(), target.cbegin(),
                              target.cend());

        implementation.insert(implementation.end(), conjugations.crbegin(),
                              conjugations.crend());
    }

    return move(implementation);
}

template<typename T>
bool found(const list<T>& container, T element)
{
    auto iter = find(container.cbegin(), container.cend(), element);
    bool result = (iter != container.cend());

    return result;
}

template<typename T>
bool found(const deque<T>& container, T element)
{
    auto iter = find(container.cbegin(), container.cend(), element);
    bool result = (iter != container.cend());

    return result;
}

template<typename T>
inline void bufferize(vector<T>& container, uint bufferSize = 1024)
{
    uint size = container.capacity();
    if(!(size & ~(bufferSize - 1)))
    {
        container.reserve(size + bufferSize);
    }
}

template<typename T>
inline void toVector(const list<T>& container, vector<T>* output)
{
    output->reserve(container.size());
    for (auto& element : container)
    {
        output->push_back(element);
    }
}

bool isWhiteSpacesOnly(const string& line);
string trim(const string& value);
string removeQuotes(const string& value);
vector<string> split(const string& value, char symbol);

string getFileName(const string& path);
string appendPath(const string& left, const string& right);

void debugLog(const string& context, function<void(ostream&)> logFunction);
void debugBehavior(const string& context, function<void()> debugFunction);
