#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

typedef std::unordered_map<std::wstring, std::vector<double>> modelData;

struct ExportedModel {
	std::vector<wchar_t> alphabet;
	std::vector<modelData> models;
};

class Model {
public:
	Model(const std::vector<std::wstring> &trainData,
		  const int order, double dPrior) :
		m_dPrior(dPrior), m_order(order), m_models(order)
	{
		srand (time(NULL));
		m_models.resize(m_order);
		p_train(trainData);
	}

	Model(ExportedModel &&model) :
		m_order(model.models.size() -1),
		m_alphabet(model.alphabet),
		m_models(model.models)
	{
		srand (time(NULL));
	}

	Model(const ExportedModel &model) :
		m_order(model.models.size() -1),
		m_alphabet(model.alphabet),
		m_models(model.models)
	{
		srand (time(NULL));
	}

	Model() : m_order(0)
	{
		srand (time(NULL));
	}
	
	ExportedModel exportData() const {
		ExportedModel res{m_alphabet, m_models};
		return res;
	}

	inline int order() const {
		return m_order;
	}

	inline bool isTrained() const {
		return !m_models.empty();
	}

	// Return the next wchar_t based on a context/word
	wchar_t generate(const std::wstring &context) const {
		wchar_t res = '#';
		if (!isTrained()) {
			return res;
		}
		int order = m_order;

		for (int i = m_order; i > 0; i--) {
			std::wstring s = context.substr(context.size() - i, i);
			const modelData &model = getModel(order);
			auto it = model.find(s);

			if (it != model.cend()) {
				res = m_alphabet[selectIndex((*it).second)];
				break;
			}
		}
		return res;
	}

	void train(const std::vector<std::wstring> &trainData,
			   const int order = 3, double dPrior = 0.0)
	{
		m_order = order;
		m_models.resize(m_order);
		for (auto &map: m_models) {
			map.clear();
		}
		m_dPrior = dPrior;
		p_train(trainData);
	}

private:
	double m_dPrior;
	int m_order;

	// List of letters in the model
	std::vector<wchar_t> m_alphabet;
	// Katz's back-off model with high order models.
	std::vector<modelData> m_models;

	inline void p_train(const std::vector<std::wstring> &trainData) {
		generateAlphabet(trainData);
		// build the chains of every order
		for (int i = 1; i <= m_order; i++) {
			buildChains(trainData, i);
		}
	}
	
	modelData& getModel(int order) {
		return m_models[order -1];
	}
	const modelData& getModel(int order) const {
		return m_models[order -1];
	}

	size_t selectIndex(const std::vector<double> &chain) const {
		double accumulator = 0.0;
		std::vector<double> totals;
		
		for (const double weight : chain) {
			accumulator += weight;
			totals.push_back(accumulator);
		}

		double randRes = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double random = randRes * accumulator;

		for (size_t i = 0; i < totals.size(); ++i) {
			if (random < totals[i]) {
				return i;
			}
		}
		return 0;
	}

	// generate the chain for a given order based on the training data,
	// the chain vector must be initialized before calling this function.
	void buildChains(const std::vector<std::wstring> &trainData,
					 const int order)
	{
		// Generate observations (wchar_ts after ecah groups of n=order wchar_ts)
		const std::wstring padding(order, '#');
		std::unordered_map<std::wstring, std::vector<wchar_t>> observations;

		for (std::wstring word : trainData) {
			word = padding + word;
			word += '#';

			for (int i = 0; i < word.length() - order; i++) {
				std::wstring key = word.substr(i, order);

				std::vector<wchar_t> &value = observations[key];
				value.push_back(word.at(i + order));
			}
		}
		// build the chain
		for (auto it : observations) {
			const std::wstring &key = it.first;
			const std::vector<wchar_t> &value = it.second;

			for (const wchar_t prediction : m_alphabet) {
				std::vector<double> &chain = getModel(order)[key];
				int count = 0;
				for (const wchar_t c : value) {
					if (prediction == c) {
						++count;
					}
				}
				chain.push_back(m_dPrior + count);
			}
		}
	}
	
	// Generate a list of all the wchar_ts in the training data
	void generateAlphabet(const std::vector<std::wstring> &trainData) {
		m_alphabet.resize(0);
		m_alphabet.push_back('#');
		for (const std::wstring &word : trainData) {

			for (const wchar_t c : word) {
				if (std::find(m_alphabet.begin(), m_alphabet.end(), c) == m_alphabet.end()) {
					m_alphabet.push_back(c);
				}
			}
		}
		std::sort(m_alphabet.begin(), m_alphabet.end());
	}
};

class WordGenerator {
public:

	WordGenerator() {
	}

	WordGenerator(ExportedModel &&model) :
		m_model(model)
	{
	}
	
	WordGenerator(const std::vector<std::wstring> &trainData,
		const int order, const double prior) :
		m_model(trainData, order, prior)
	{
	}

	void train(const std::vector<std::wstring> &trainData,
			   const int order = 3, double dPrior = 0.0)
	{
		m_model.train(trainData, order, dPrior);
	}

	inline bool isTrained() const {
		return m_model.isTrained();
	}

	std::wstring newWord(const int minLength, const int maxLength) const {
		std::wstring word;

		if (!isTrained()) {
			return word;
		}
		int i = 0;
        do {
			word = std::wstring(m_model.order(), '#');
			wchar_t letter = m_model.generate(word);

			while (letter != '#') {
				word += letter;
				letter = m_model.generate(word);
			}

			word.erase(std::remove(word.begin(), word.end(), '#'), word.end());
			i++;
		} while (i < 100 && (word.size() < minLength || word.size() > maxLength));

		return word;
	}

	std::vector<std::wstring> newWords(
		const size_t n,
		const int minLength,
		const int maxLength,
		bool repeat = false) const 
	{
		std::vector<std::wstring> words;

		if (!isTrained()) {
			return words;
		}

		words.reserve(n);

		while (words.size() < n) {
			std::wstring word = newWord(minLength, maxLength);
			words.push_back(word);
		}
		return words;
	}

	ExportedModel exportData() const {
		return m_model.exportData();
	}

private:
	Model m_model;
};
 
