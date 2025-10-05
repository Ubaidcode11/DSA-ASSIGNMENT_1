#include "uno.h"
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <unordered_map>
using namespace std;

struct CardInfo {
    string color;
    string value;
};

struct Impl {
    int numPlayers;
    vector<CardInfo> deck;
    vector<vector<CardInfo>> hands;
    vector<CardInfo> discard;
    int currentPlayer;
    int direction;
    mt19937 rng;
    int winnerIndex;
    bool stalemate;
    int turnsSinceLastPlay;

    Impl(int n) : numPlayers(n), currentPlayer(0), direction(1),
                  rng(1234), winnerIndex(-1), stalemate(false), turnsSinceLastPlay(0) {}

    int nextIndex(int from, int steps = 1) const {
        int idx = (from + steps * direction) % numPlayers;
        if (idx < 0) idx += numPlayers;
        return idx;
    }
};

static unordered_map<UNOGame*, Impl*> impls;

static bool cardPlayable(const CardInfo &card, const CardInfo &top) {
    return (card.color == top.color) || (card.value == top.value);
}

static void makeDeck(vector<CardInfo> &deck) {
    deck.clear();
    vector<string> colors = {"Red", "Green", "Blue", "Yellow"};
    vector<string> values = {"0","1","2","3","4","5","6","7","8","9"};
    vector<string> actions = {"Skip", "Reverse", "Draw Two"};
    for (auto &c : colors) {
        for (auto &v : values) deck.push_back({c, v}); // 10
        for (auto &a : actions)
            for (int i = 0; i < 3; ++i)
                deck.push_back({c, a}); // 9 more → 19 per color
    }
}

UNOGame::UNOGame(int numPlayers) {
    impls[this] = new Impl(numPlayers);
}

void UNOGame::initialize() {
    Impl* p = impls[this];
    p->deck.clear(); p->hands.assign(p->numPlayers, {});
    p->discard.clear(); p->winnerIndex = -1;
    p->stalemate = false; p->turnsSinceLastPlay = 0;
    p->rng.seed(1234); p->direction = 1; p->currentPlayer = 0;

    makeDeck(p->deck);
    shuffle(p->deck.begin(), p->deck.end(), p->rng);

    for (int i = 0; i < p->numPlayers * 7; ++i)
        p->hands[i % p->numPlayers].push_back(p->deck.back()), p->deck.pop_back();

    p->discard.push_back(p->deck.back());
    p->deck.pop_back();
}

bool UNOGame::isGameOver() const {
    Impl* p = impls.at(const_cast<UNOGame*>(this));
    return (p->winnerIndex != -1) || p->stalemate;
}

int UNOGame::getWinner() const {
    Impl* p = impls.at(const_cast<UNOGame*>(this));
    return p->winnerIndex;
}

string UNOGame::getState() const {
    Impl* p = impls.at(const_cast<UNOGame*>(this));
    ostringstream out;
    out << "Player " << p->currentPlayer << "'s turn, Direction: "
        << (p->direction == 1 ? "Clockwise" : "Counter-clockwise")
        << ", Top: " << p->discard.back().color << " " << p->discard.back().value
        << ", Players cards: ";
    for (int i = 0; i < p->numPlayers; ++i) {
        out << "P" << i << ":" << p->hands[i].size();
        if (i != p->numPlayers - 1) out << ", ";
    }
    return out.str();
}

void UNOGame::playTurn() {
    Impl* p = impls[this];
    if (isGameOver()) return;

    vector<CardInfo> &hand = p->hands[p->currentPlayer];
    CardInfo top = p->discard.back();
    int playIndex = -1;

    for (int i = 0; i < (int)hand.size(); ++i)
        if (hand[i].color == top.color) { playIndex = i; break; }
    if (playIndex == -1)
        for (int i = 0; i < (int)hand.size(); ++i)
            if (hand[i].value == top.value) { playIndex = i; break; }

    if (playIndex == -1) {
        if (!p->deck.empty()) {
            CardInfo drawn = p->deck.back(); p->deck.pop_back();
            if (cardPlayable(drawn, top)) {
                p->discard.push_back(drawn);
                if (drawn.value == "Reverse") p->direction *= -1;
                else if (drawn.value == "Skip") p->currentPlayer = p->nextIndex(p->currentPlayer, 2);
                else if (drawn.value == "Draw Two") {
                    int target = p->nextIndex(p->currentPlayer, 1);
                    for (int d = 0; d < 2 && !p->deck.empty(); ++d)
                        p->hands[target].push_back(p->deck.back()), p->deck.pop_back();
                    p->currentPlayer = p->nextIndex(p->currentPlayer, 2);
                } else p->currentPlayer = p->nextIndex(p->currentPlayer, 1);
                p->turnsSinceLastPlay = 0;
                return;
            } else hand.push_back(drawn);
        } else {
            p->turnsSinceLastPlay++;
            if (p->turnsSinceLastPlay >= p->numPlayers) { p->stalemate = true; return; }
            p->currentPlayer = p->nextIndex(p->currentPlayer, 1);
            return;
        }
    }

    if (playIndex != -1) {
        CardInfo played = hand[playIndex];
        hand.erase(hand.begin() + playIndex);
        p->discard.push_back(played);
        if (hand.size() == 1) cout << "UNO" << endl;
        if (hand.empty()) { p->winnerIndex = p->currentPlayer; return; }

        if (played.value == "Reverse") {
            p->direction *= -1;
            p->currentPlayer = p->nextIndex(p->currentPlayer, 1);
        } else if (played.value == "Skip") {
            p->currentPlayer = p->nextIndex(p->currentPlayer, 2);
        } else if (played.value == "Draw Two") {
            int target = p->nextIndex(p->currentPlayer, 1);
            for (int d = 0; d < 2 && !p->deck.empty(); ++d)
                p->hands[target].push_back(p->deck.back()), p->deck.pop_back();
            p->currentPlayer = p->nextIndex(p->currentPlayer, 2);
        } else p->currentPlayer = p->nextIndex(p->currentPlayer, 1);

        p->turnsSinceLastPlay = 0;
    }

    if (p->deck.empty()) {
        p->turnsSinceLastPlay++;
        if (p->turnsSinceLastPlay >= p->numPlayers) p->stalemate = true;
    }
}
