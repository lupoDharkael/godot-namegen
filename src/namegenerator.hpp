#include <Godot.hpp>
#include <Array.hpp>
#include <Reference.hpp>
#include "markovnames.hpp"

class NameGenerator : public godot::GodotScript<godot::Reference> {
	GODOT_CLASS(NameGenerator)

public:
	static void _register_methods();
	
	void _init();

	void train(const godot::PoolStringArray arr);

	bool is_trained() const;

	godot::String new_word(int min , int max) const;

	godot::PoolStringArray new_words(int n, int min, int max) const;

private:
	WordGenerator m_generator;
};
