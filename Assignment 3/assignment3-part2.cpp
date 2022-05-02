// Comp 322 Assignment 3
// Name: Kimi Zhao
// Student ID: 260989483
// No collaborators

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Creating Rank
enum Rank {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, 
    EIGHT, NINE, TEN, JACK, QUEEN, KING
};
// Creating Type
enum Type {CLUBS, DIAMONDS, HEARTS, SPADES};

class Card
{
    private: 
        Rank rank;
        Type type;
    
    public:
        friend class BlackJackGame;
        friend class Hand;
        // Constructor
        Card(Rank rank, Type type) {
            this->rank = rank;
            this->type = type;
        }
        // Helper function: returns true if rank value = value
        bool sameValue(int value) {
            if (value == 11) return this->rank == ACE;
            return getValue() == value;
        }
        // Returns the numerical value
        int getValue(){
            if (rank == JACK) return 10;
            if (rank == QUEEN) return 10;
            if (rank == KING) return 10;
            return this->rank;
        }
        // Display the card info to the screen
        void displayCard() {
            // Displaying the rank
            if (rank == JACK) cout << "J";
            else if (rank == QUEEN) cout << "Q";
            else if (rank == KING) cout << "K";
            else cout << rank;

            // Displaying the type
            switch(type) {
                case CLUBS: cout << "C "; break;
                case DIAMONDS: cout << "D "; break;
                case HEARTS: cout << "H "; break;
                case SPADES: cout << "S "; break;
            }
        }
};

class Hand
{
    private: 
        vector<Card> hand;
        int total;
        bool has_ace;
    public: 
        friend class BlackJackGame;
        friend class ComputerPlayer;
        friend class HumanPlayer;
        // Constructor
        Hand() {
            total = 0;
            has_ace = false;
        }

        // Adding card to hand, and updating total
        void add(Card card) {
            hand.push_back(card);
            total += card.getValue();
            if (card.rank == ACE) has_ace = true;
        } 
        // Resetting
        void clear() {
            hand.clear();
            total = 0;
            has_ace = false;
        }
        // Return total value with regard to whether the hand contains ace
        int getTotal() {
            if (has_ace == true && total + 10 <= 21) return total + 10;
            return total;
        }
};

class Deck
{
    private:
         vector<Card> deck;
    public:
        friend class BlackJackGame;
        friend class ComputerPlayer;        
        // Populate the deck at initialization
        Deck() {
            Populate();
        }
        // Initialize deck with all card combinations
        void Populate() {
            Rank all_ranks[13] = {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
            Type all_type[4] = {CLUBS, DIAMONDS, HEARTS, SPADES};
            for (Rank rank : all_ranks) for (Type type : all_type) {
                Card new_card = Card(rank, type);
                deck.push_back(new_card);
            }
        }
        // Shuffling the deck with randomly generated seed
        void shuffle() {
            // Seed the time
            srand(time(0));
            // random number between 0 and 200
            int seed = rand() % 201;
            std::shuffle(deck.begin(), deck.end(), default_random_engine(seed));
        }
        // Return card to deal, and removing it from the deck
        Card deal() {
            Card card_to_deal = deck.front();
            deck.erase(deck.begin());
            return card_to_deal;
        }
};

class AbstractPlayer
{
    protected: 
        Hand hand;
    public:
        friend class BlackJackGame;
        virtual bool isDrawing() = 0;
        // Return true if greater than 21
        bool isBusted() {
            return hand.getTotal() > 21;
        }
        // Draw card (inherited by ComputerPlayer and HumanPlayer)
        void draw(Card card) {
            hand.add(card);
        }
};

class HumanPlayer: public AbstractPlayer
{
    private: 
        char to_draw_or_not_to_draw;
        int ID;
    public:
        friend class BlackJackGame;
        // Default to drawing a card
        HumanPlayer() { to_draw_or_not_to_draw = 'y'; }

        // To differentiate between different hands
        void assignID(int id) {
            ID = id;
        }

        // Checks for player's input
        bool isDrawing() {
            return (to_draw_or_not_to_draw == 'y' || to_draw_or_not_to_draw == 'Y');
        }

        // Prints whether player loses, or if they want to draw another card
        void announce() {
            if (isBusted()) cout << "Player hand " << ID <<  " busts." << endl;
            else if (hand.getTotal() == 21) return;
            else {
                cout << "Do you want to draw? (y/n): ";
                cin >> to_draw_or_not_to_draw;
            }
        }

        void displayHand() {
            // Displaying Player hand
                cout << "Player hand " << ID << ": ";
                for (Card card : hand.hand) {
                    card.displayCard();
                }
                cout << "[" << hand.getTotal() << "]" << endl;
        }

        void clear() {
            hand.clear();
            to_draw_or_not_to_draw = 'y';
        }
};

class ComputerPlayer: public AbstractPlayer
{
    private: 
        int total_games;
        int games_won;
        bool rigged;
    public:
        friend class BlackJackGame;
        ComputerPlayer() {
            // Total games don't include draws between player and computer
            total_games = 0;
            games_won = 0;
            rigged = false;
        }
        
        bool isDrawing() {
            // Casino wins more than 55% of the time
            if (total_games > 0 && (float)games_won / (float)total_games < 0.55) {
                rigged = true;
                return true;
            } 
            return this->hand.getTotal() <= 16;
        }

        void displayHand() {
            // Displaying Casino hand
            cout << "Casino: ";
            for (Card card : hand.hand) {
                card.displayCard();
            }
            cout << "[" << hand.getTotal() << "]" << endl;
        }
};

class BlackJackGame
{
    private:
        Deck m_deck;
        ComputerPlayer m_casino;
        HumanPlayer m_player_1;
        HumanPlayer m_player_2;
        HumanPlayer m_player_3;
        int total_hands;
        char add_hand;
        // Trust me
        void TotallyNotRiggingTheGame() {
            int to_win = 21 - m_casino.hand.getTotal();
            int index = 0;
            // Traversing the deck to find the winning card
            for (Card card: m_deck.deck) {
                if (card.sameValue(to_win)) {
                    // Deal card to casino, and remove it from the deck
                    Card card_to_deal = card;
                    m_deck.deck.erase(m_deck.deck.begin() + index);
                    m_casino.draw(card_to_deal);
                    break;
                }
                index++;
            }
            // If no card satisfies the condition, there's nothing I can do... It's rng...
        }

    public:
        // Initializing the deck
        BlackJackGame() { 
            m_deck = Deck(); 
            total_hands = 1; 
            add_hand = 'y'; 
            m_player_1.assignID(1);
            m_player_2.assignID(2);
            m_player_3.assignID(3);
        }
        void reset() {
            m_casino.hand.clear();
            m_player_1.clear();
            m_player_2.clear();
            m_player_3.clear();
            m_deck = Deck();
            total_hands = 1;
        }

        void addHand() {
            cout << "Would you like to add another hand? (3 max) (y/n): ";
            cin >> add_hand;
            if ((add_hand == 'y' || add_hand == 'Y') && total_hands <= 3) {
                total_hands++;
                // Drawing card for the appropriate hand
                switch (total_hands) {
                    case 2: 
                        m_player_2.draw(m_deck.deal()); 
                        player_draw_card(m_player_2, total_hands); 
                        break;
                    case 3: 
                        m_player_3.draw(m_deck.deal());
                        player_draw_card(m_player_3, total_hands); 
                        break;
                }
            }
        }

        void player_draw_card(HumanPlayer player, int hand) {
            do {
                // Player draws one more card
                player.draw(m_deck.deal());
                // Displaying Player hand
                player.displayHand();

                player.announce();
                if (player.isBusted()) {
                    break;
                }
                if (player.hand.getTotal() == 21) break;
            }
            while (player.isDrawing());
            // Assigning to the right hand
            switch (hand) {
                case 1: m_player_1 = player; break;
                case 2: m_player_2 = player; break;
                case 3: m_player_3 = player; break;
            }
            while (total_hands < 3 && (add_hand == 'y' || add_hand == 'Y')) {
                addHand();
            }
        }
        void casino_draw_card() {
            while (m_casino.isDrawing()) {
                if (m_casino.rigged) {
                    // Drawing winning card for casino
                    TotallyNotRiggingTheGame();
                    // Displaying Casino hand
                    m_casino.displayHand();
                    break;
                } else {
                    // Casino draws one more card
                    m_casino.draw(m_deck.deal());
                    // Displaying Casino hand
                    m_casino.displayHand();
                }
            }
        }
        void who_wins() {
            bool busted = false;
            // Determine if player busts depending on their number of hands
            switch(total_hands) {
                case 1: 
                    if (m_player_1.isBusted()) busted = true;
                    break;
                case 2:
                    if (m_player_1.isBusted() && m_player_2.isBusted()) busted = true;
                    break;
                case 3: 
                    if (m_player_1.isBusted() &&  m_player_2.isBusted() && m_player_3.isBusted()) busted = true;
                    break;
            }
            // If all hands are busted, casino wins
            if (busted) {
                cout << "All player's hands busted." << endl << "Casino wins." << endl;
                m_casino.games_won++;
                return;
            }

            // Finding the best hand for the player
            int player_winning_hand = 0;
            if (!m_player_1.isBusted() && m_player_1.hand.getTotal() > player_winning_hand) {
                player_winning_hand = m_player_1.hand.getTotal();
            }
            if (!m_player_2.isBusted() && m_player_2.hand.getTotal() > player_winning_hand) {
                player_winning_hand = m_player_2.hand.getTotal();
            }
            if (!m_player_3.isBusted() && m_player_3.hand.getTotal() > player_winning_hand) {
                player_winning_hand = m_player_3.hand.getTotal();
            }

            // Find out who wins
            if (m_casino.isBusted()) {
                cout << "Casino busts." << endl << "Player wins." << endl;
            }
            else if (m_casino.hand.getTotal() == player_winning_hand) {
                cout << "No one wins." << endl;
                m_casino.total_games--;
            }
            else if (m_casino.hand.getTotal() > player_winning_hand) {
                m_casino.games_won++;
                cout << "Casino wins." << endl;
            }
            else cout << "Player wins." << endl;

        }
        void play() {
            m_deck.shuffle();

            // Displaying Casino hand
            m_casino.draw(m_deck.deal());
            m_casino.displayHand();
            
            // Player drawing cards
            m_player_1.draw(m_deck.deal());
            player_draw_card(m_player_1, 1);

            // iF player has not busted, casino draws cards
            if (!m_player_1.isBusted() || !m_player_2.isBusted() || !m_player_3.isBusted()) {
                m_casino.draw(m_deck.deal());
                m_casino.displayHand();
                casino_draw_card();
            }
            who_wins();
            m_casino.total_games++;
            // Reset for new round
            reset();
        }
};

int main() 
{
    cout << "\tWelcome to the COMP322 Blackjack game!" << endl << endl;

    BlackJackGame game;

    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();

        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
    }

    cout << "Game over!";
    return 0;
}