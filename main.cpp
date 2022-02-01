#include <bits/stdc++.h>
using namespace std;

enum cards_enum
{
    Two = 2,
    Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace
};
enum combination
{
    SeniorCard, Pair, TwoPairs, Set, Straight, Flush, FullHouse, Kare, StraightFlush, RoyalFlush
};
vector<int> combinationDefinition(vector<string> &cards);
cards_enum seniorCard(vector<string> &cards);
vector<int> pair_twoPairs_set(vector<string> &cards);
vector<int> straight(vector<string> &cards);
vector<int> flush(vector<string> &cards);
vector<int> fullHouse(vector<string> &cards);
vector<int> kare(vector<string> &cards); // four of a kind
vector<int> straightFlush(vector<string> &cards);
bool royalFlush(vector<string> &cards);
string writeCombination(vector<int> res);
string cardTranslator(cards_enum &card);
map<char, cards_enum> charToEnum = {{'2', Two}, {'3', Three}, {'4', Four}, {'5', Five}, {'6', Six},
                                    {'7', Seven}, {'8', Eight}, {'9', Nine}, {'T', Ten},
                                    {'J', Jack}, {'Q', Queen}, {'K', King}, {'A', Ace}};

int main()
{
    setlocale(0, "");

    vector<string> v1 = {"3D", "KD", "QS", "JH", "4C", "7C", "2H"};
    vector<string> v2 = {"3D", "AD", "JS", "JH", "4C", "7C", "2H"};
    vector<string> v3 = {"3D", "AD", "JS", "JH", "3C", "7C", "2H"};
    vector<string> v4 = {"3D", "AD", "JS", "3H", "3C", "7C", "2H"};
    vector<string> v5 = {"5D", "AD", "2S", "3H", "4C", "7C", "2H"};
    vector<string> v6 = {"KD", "9D", "8S", "7H", "QC", "JC", "TH"};
    vector<string> v7 = {"5D", "4D", "2S", "3D", "4C", "7D", "AD"};
    vector<string> v8 = {"5D", "AD", "5S", "3H", "5C", "7C", "7H"};
    vector<string> v9 = {"5D", "7D", "5S", "5H", "5C", "7C", "7H"};
    vector<string> v10 = {"5D", "AD", "8D", "3H", "9D", "7D", "6D"};
    vector<string> v11 = {"KS", "AS", "2S", "3S", "QS", "JS", "TS"};

    cout << "\n\t1) " << writeCombination(combinationDefinition(v1));
    cout << "\n\t2) " << writeCombination(combinationDefinition(v2));
    cout << "\n\t3) " << writeCombination(combinationDefinition(v3));
    cout << "\n\t4) " << writeCombination(combinationDefinition(v4));
    cout << "\n\t5) " << writeCombination(combinationDefinition(v5));
    cout << "\n\t6) " << writeCombination(combinationDefinition(v6));
    cout << "\n\t7) " << writeCombination(combinationDefinition(v7));
    cout << "\n\t8) " << writeCombination(combinationDefinition(v8));
    cout << "\n\t9) " << writeCombination(combinationDefinition(v9));
    cout << "\n\t10) " << writeCombination(combinationDefinition(v10));
    cout << "\n\t11) " << writeCombination(combinationDefinition(v11)) << "\n";

    cout << boolalpha << "\n\t" << (combinationDefinition(v7) > combinationDefinition(v6));
    cout << boolalpha << "\n\t" << (combinationDefinition(v6) > combinationDefinition(v5));
    cout << boolalpha << "\n\t" << (combinationDefinition(v1) > combinationDefinition(v11)) << "\n";

    return 0;
}

vector<int> combinationDefinition(vector<string> &cards) // {combination, high_card}
{
    if(royalFlush(cards)) return {RoyalFlush, 2};
    else if(straightFlush(cards)[0]) return {StraightFlush, straightFlush(cards)[2]};
    else if(kare(cards)[0]) return {Kare, kare(cards)[2]};
    else if(fullHouse(cards)[0]) return {FullHouse, fullHouse(cards)[2]};
    else if(flush(cards)[0]) return {Flush, flush(cards)[2]};
    else if(straight(cards)[0]) return {Straight, straight(cards)[2]};
    else if(pair_twoPairs_set(cards)[0]) return {pair_twoPairs_set(cards)[1], pair_twoPairs_set(cards)[2]};
    else return {SeniorCard, seniorCard(cards)};
}

cards_enum seniorCard(vector<string> &cards)
{
    vector<cards_enum> values(7);
    for(int i = 0; i < 7; ++i) values[i] = charToEnum.find(cards[i][0])->second;
    return *max_element(values.begin(), values.end());
}

vector<int> pair_twoPairs_set(vector<string> &cards) // {1/0, combination, high_card}
{
    vector<cards_enum> values(7);
    for(int i = 0; i < 7; ++i) values[i] = charToEnum.find(cards[i][0])->second;
    map<cards_enum, int> m;
    for(const auto &x : values) m[x]++;

    vector<int> combo_finder(2); // {1/0, high_card}
    for(auto it = m.begin(); it != m.end(); ++it)
        if(it->second == 3 && it->first > combo_finder[1]) {
            combo_finder[0] = 1;
            combo_finder[1] = it->first;
        }                                  // нашли старший сет (либо установили его отсутствие)
    if(combo_finder[0]) return {1, Set, combo_finder[1]};

    combo_finder.clear();
    for(auto it = m.begin(); it != m.end(); ++it)
        if(it->second >= 2) combo_finder.push_back(it->first);
    sort(combo_finder.rbegin(), combo_finder.rend());
    if(combo_finder.size() == 1) return {1, Pair, combo_finder[0]};
    else if(combo_finder.size() >= 2) return {1, TwoPairs, combo_finder[0]};
    else return {0, 0, 2};
}

vector<int> straight(vector<string> &cards)
{
    vector<cards_enum> values(7);
    for(int i = 0; i < 7; ++i) values[i] = charToEnum.find(cards[i][0])->second;
    sort(values.begin(), values.end());
    values.resize(distance(values.begin(), unique(values.begin(), values.end())));
    int k = 1, high_card = 0;
    for(int i = 1; i < values.size(); ++i) {
        if(values[i] - values[i-1] == 1) k++;
        else k = 1;
        if(k == 5) {
            k = 4;
            high_card = values[i];
        }
    }
    if(high_card) return {1, Straight, high_card};
    else if(values.back() == Ace && values[0] == Two && values[1] == Three && values[2] == Four && values[3] == Five)
        return {1, Straight, Five};
    else return {0, 0, 2};
}

vector<int> flush(vector<string> &cards)
{
    map<char, int> m;
    for(int i = 0; i < 7; ++i) m[cards[i][1]]++;
    char suit = '*';
    for(auto it = m.begin(); it != m.end(); ++it) {
        if(it->second >= 5) suit = it->first;
    }
    if(suit == '*') return {0, 0, 2};
    else {
        int high_card = 0;
        for(int i = 0; i < 7; ++i) {
            if(suit == cards[i][1] && charToEnum.find(cards[i][0])->second > high_card)
                high_card = charToEnum.find(cards[i][0])->second;
        }
        return {1, Flush, high_card};
    }
}

vector<int> fullHouse(vector<string> &cards)
{
    vector<cards_enum> values(7);
    for(int i = 0; i < 7; ++i) values[i] = charToEnum.find(cards[i][0])->second;
    map<cards_enum, int> m;
    for(const auto &x : values) m[x]++;
    vector<int> set_finder, pair_finder;
    for(auto it = m.begin(); it != m.end(); ++it) {
        if(it->second == 3) set_finder.push_back(it->first);
        else if(it->second == 2) pair_finder.push_back(it->first);
    }
    if(!set_finder.size() || set_finder.size() == 1 && !pair_finder.size()) return {0, 0, 2};
    if(!pair_finder.size()) return {1, FullHouse, max(set_finder[0], set_finder[1])};
    else return {1, FullHouse, set_finder[0]};
}

vector<int> kare(vector<string> &cards)
{
    vector<cards_enum> values(7);
    for(int i = 0; i < 7; ++i) values[i] = charToEnum.find(cards[i][0])->second;
    map<cards_enum, int> m;
    for(const auto &x : values) m[x]++;
    for(auto it = m.begin(); it != m.end(); ++it)
        if(it->second == 4) return {1, Kare, it->first};
    return {0, 0, 2};
}

vector<int> straightFlush(vector<string> &cards)
{
    if(!flush(cards)[0]) return {0, 0, 2};
    map<char, int> m;
    for(int i = 0; i < 7; ++i) m[cards[i][1]]++;
    vector<int> flush_values;
    char suit;
    for(auto it = m.begin(); it != m.end(); ++it) {
        if(it->second >= 5) suit = it->first;
    }
    if(suit == ' ') cout << "\n\tsome error in straightFLush() function\n";
    for(int i = 0; i < 7; ++i) {
        if(cards[i][1] == suit)
            flush_values.push_back(charToEnum.find(cards[i][0])->second);
    }
    sort(flush_values.begin(), flush_values.end());
    int k = 1, high_card = 0;
    for(int i = 1; i < flush_values.size(); ++i) {
        if(flush_values[i] - flush_values[i-1] == 1) k++;
        else k = 1;
        if(k == 5) {
            k = 4;
            high_card = flush_values[i];
        }
    }
    if(high_card) return {1, StraightFlush, high_card};
    else if(flush_values.back() == Ace && flush_values[0] == Two && flush_values[1] == Three &&
            flush_values[2] == Four && flush_values[3] == Five) return {1, StraightFlush, Five};
    else return {0, 0, 2};
}

bool royalFlush(vector<string> &cards)
{
    vector<int> v = straightFlush(cards);
    if(v[0] && v[2] == Ace) return 1;
    return 0;
}

string writeCombination(vector<int> res)
{
    cards_enum x = (cards_enum) res[1];
    string s = cardTranslator(x);
    switch(res[0])
    {
        case 0: return "Старшая карта (" + s + ")"; // SeniorCard
        case 1: return "Пара (" + s + ")"; // Pair
        case 2: return "Две пары (старшая из которых - " + s + ")"; // TwoPairs
        case 3: return "Сет (" + s + ")"; // Set
        case 4: return "Стрит (" + s + ")"; // Straight
        case 5: return "Флеш (старшая - " + s + ")"; // Flush
        case 6: return "Фулл хаус (" + s + ")"; // FullHouse
        case 7: return "Каре (" + s + ")"; // Kare (four of a kind)
        case 8: return "Стрит флеш (" + s + ")"; // StraightFlush
        case 9: return "Роял флеш"; // RoyalFlush
        default: return "Несуществующая комбинация";
    }
}

string cardTranslator(cards_enum &card)
{
    switch(card)
    {
        case Two: return "двойка";
        case Three: return "тройка";
        case Four: return "четверка";
        case Five: return "пятерка";
        case Six: return "шестерка";
        case Seven: return "семерка";
        case Eight: return "восьмерка";
        case Nine: return "девятка";
        case Ten: return "десятка";
        case Jack: return "валет";
        case Queen: return "дама";
        case King: return "король";
        case Ace: return "туз";
        default: return "some mistake";
    }
}
