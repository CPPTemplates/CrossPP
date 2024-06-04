#include "jsonReader.h"
#include "array/wstringFunctions.h"

jsonContainer readJson(std::wstring content)
{
	jsonContainer container = jsonContainer();
	content = replace(content, std::wstring(L"\\u0026"), std::wstring(L"&"));
	//{ = data with name
	//[ = data without name
	csize_t& indexQuote = find(content, 0, std::wstring(L"\""), std::wstring());
	csize_t& indexCurlyBracket = find(content, 0, std::wstring(L"{"), std::wstring());
	csize_t& indexBracket = find(content, 0, std::wstring(L"["), std::wstring());
	csize_t& nearestIndex = FindNearest(std::vector<size_t>({ indexQuote,indexCurlyBracket,indexBracket }));
	if (nearestIndex == std::wstring::npos)
	{
		container.value = trim_copy(content);
	}
	if (nearestIndex == 0)
	{
		//" first
						//read name
		csize_t& startPos = indexQuote + 1;
		csize_t& endPos = find(content, startPos, std::wstring(L"\""), std::wstring());
		container.value = content.substr(startPos, endPos - startPos);

		csize_t& indexColon = find(content, endPos + 1, std::wstring(L":"), std::wstring(L"{}[]()\"\""));
		if (indexCurlyBracket == std::wstring::npos && indexBracket == std::wstring::npos && indexColon != std::wstring::npos)
		{
			jsonContainer movingBlocks = jsonContainer();
			size_t indexQuote2 = find(content, endPos + 1, std::wstring(L"\""), std::wstring(L"{}[]"));
			if (indexQuote2 == std::wstring::npos)
			{
				movingBlocks.value = trim_copy(content.substr(indexColon + 1));
			}
			else
			{
				indexQuote2 += 1;
				csize_t& endIndexQuote2 = find(content, indexQuote2, std::wstring(L"\""), std::wstring());
				movingBlocks.value = content.substr(indexQuote2, endIndexQuote2 - indexQuote2);
			}
			container.children.push_back(movingBlocks);
			return container;
		}
	}
	csize_t& nearestBracketIndex = FindNearest(std::vector<size_t>({ indexCurlyBracket,indexBracket }));
	if (nearestBracketIndex == 0) {
		//{ first
		csize_t& endIndexCurlyBracket = find(content, indexCurlyBracket + 1, std::wstring(L"}"), std::wstring(L"[]\"\"{}"));
		//separate by commas
		wstringContainer arrayContainer = split_string(content.substr(indexCurlyBracket + 1, endIndexCurlyBracket - indexCurlyBracket - 1), std::wstring(L","), std::wstring(L"[]\"\"{}"));
		for (std::wstring str : arrayContainer)
		{
			container.children.push_back(readJson(str));
		}
	}
	else if (nearestBracketIndex == 1)
	{//[ first
		csize_t& endIndexBracket = find(content, indexBracket + 1, std::wstring(L"]"), std::wstring(L"{}\"\"[]"));
		//separate by commas
		wstringContainer arrayContainer = split_string(content.substr(indexBracket + 1, endIndexBracket - indexBracket - 1), std::wstring(L","), std::wstring(L"[](){}"));
		for (std::wstring str : arrayContainer)
		{
			container.children.push_back(readJson(str));
		}
	}
	return container;
}

size_t jsonContainer::getChildIndex(const std::wstring& value) const
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i].value == value)
		{
			return i;
		}
	}
	return std::wstring::npos;
}

jsonContainer jsonContainer::getChild(const std::wstring& value) const
{
	csize_t& index = getChildIndex(value);
	if (index == std::wstring::npos)
	{
		throw "no child found with this value";
	}
	return children[index];
}

jsonContainer jsonContainer::operator[](const std::wstring& name) const
{
	return getChild(name);
}
