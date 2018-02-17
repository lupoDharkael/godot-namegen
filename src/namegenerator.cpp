#include "namegenerator.hpp"
#include <codecvt>
#include <locale>
#include <iostream>
#include <string>

void NameGenerator::_init() {
}


void NameGenerator::train(const godot::PoolStringArray arr) {
	std::vector<std::wstring> data;
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	godot::Godot::print(std::to_string(arr.size()).c_str());
	{ // locked scope
		auto read = arr.read();
		for (int i = 0; i < arr.size(); i++) {
			//std::wstring s = converter.from_bytes(read[i].utf8().get_data());
			std::wstring s(read[i].unicode_str());
			data.push_back(s);
		}
	} // locked scope
	m_generator.train(data);
}

bool NameGenerator::is_trained() const {
	return m_generator.isTrained();
}

void NameGenerator::_register_methods() {
	godot::register_method("new_word", &NameGenerator::new_word);
	godot::register_method("new_words", &NameGenerator::new_words);
	godot::register_method("train", &NameGenerator::train);
	godot::register_method("is_trained", &NameGenerator::is_trained);
}


godot::String NameGenerator::new_word(int min, int max) const {
	return godot::String(m_generator.newWord(min, max).c_str());
}

godot::PoolStringArray NameGenerator::new_words(int n, int min, int max) const {
	godot::PoolStringArray arr;
	auto words = m_generator.newWords(n, min, max);
	for (const std::wstring &s : words) {
		arr.push_back(godot::String(s.c_str()));
	}
	return arr;
}
