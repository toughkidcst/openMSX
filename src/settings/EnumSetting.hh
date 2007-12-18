// $Id$

#ifndef ENUMSETTING_HH
#define ENUMSETTING_HH

#include "SettingPolicy.hh"
#include "SettingImpl.hh"
#include "TclObject.hh"
#include "CommandException.hh"
#include "Completer.hh"
#include "StringOp.hh"
#include <map>
#include <set>
#include <cassert>

namespace openmsx {

template <typename T> class EnumSettingPolicy : public SettingPolicy<T>
{
public:
	typedef std::map<std::string, T, StringOp::caseless> Map;

	void getPossibleValues(std::set<std::string>& result) const;

protected:
	EnumSettingPolicy(CommandController& commandController,
	                  const Map& map_);
	virtual ~EnumSettingPolicy();

	std::string toString(T value) const;
	T fromString(const std::string& str) const;
	virtual void checkSetValue(T& value) const;
	void tabCompletion(std::vector<std::string>& tokens) const;
	std::string getTypeString() const;
	void additionalInfo(TclObject& result) const;

private:
	Map enumMap;
};

template <typename T> class EnumSetting : public SettingImpl<EnumSettingPolicy<T> >
{
public:
	EnumSetting(CommandController& commandController, const std::string& name,
	            const std::string& description, T initialValue,
	            const typename EnumSettingPolicy<T>::Map& map_,
	            Setting::SaveSetting save = Setting::SAVE);
};


//-------------

template <typename T>
EnumSettingPolicy<T>::EnumSettingPolicy(
		CommandController& commandController, const Map& map_)
	: SettingPolicy<T>(commandController)
	, enumMap(map_)
{
}

template <typename T>
EnumSettingPolicy<T>::~EnumSettingPolicy()
{
}

template<typename T>
void EnumSettingPolicy<T>::getPossibleValues(std::set<std::string>& result) const
{
	for (typename Map::const_iterator it = enumMap.begin();
	     it != enumMap.end(); ++it) {
		try {
			T val = it->second;
			checkSetValue(val);
			result.insert(it->first);
		} catch (MSXException& e) {
			// ignore
		}
	}
}

template<typename T>
std::string EnumSettingPolicy<T>::toString(T value) const
{
	for (typename Map::const_iterator it = enumMap.begin();
	     it != enumMap.end() ; ++it) {
		if (it->second == value) {
			return it->first;
		}
	}
	assert(false);
	return "";	// avoid warning
}

template<typename T>
T EnumSettingPolicy<T>::fromString(const std::string& str) const
{
	typename Map::const_iterator it = enumMap.find(str);
	if (it == enumMap.end()) {
		throw CommandException("not a valid value: " + str);
	}
	return it->second;
}

template<typename T>
void EnumSettingPolicy<T>::checkSetValue(T& /*value*/) const
{
}

template<typename T>
void EnumSettingPolicy<T>::tabCompletion(std::vector<std::string>& tokens) const
{
	std::set<std::string> stringSet;
	getPossibleValues(stringSet);
	Completer::completeString(tokens, stringSet, false); // case insensitive
}

template<typename T>
std::string EnumSettingPolicy<T>::getTypeString() const
{
	return "enumeration";
}

template <typename T>
void EnumSettingPolicy<T>::additionalInfo(TclObject& result) const
{
	TclObject valueList(result.getInterpreter());
	std::set<std::string> values;
	this->getPossibleValues(values);
	valueList.addListElements(values.begin(), values.end());
	result.addListElement(valueList);
}


template <typename T>
EnumSetting<T>::EnumSetting(
		CommandController& commandController, const std::string& name,
		const std::string& description, T initialValue,
		const typename EnumSettingPolicy<T>::Map& map_,
		Setting::SaveSetting save)
	: SettingImpl<EnumSettingPolicy<T> >(
		commandController, name, description, initialValue, save, map_)
{
}

} // namespace openmsx

#endif
