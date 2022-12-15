#include <iostream>
#include <string>
#include <cstdlib> // Used for rand
#include <ctime> // Used to seed rand with time
#include <iomanip>

using namespace std;

const int NUMS[13] = { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 }; // Power of the cards
const string RANKS[13] = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };  // Ranks of the cards
const string SUITS[4] = { "Clubs", "Diamonds", "Hearts", "Spades" };    // Suits of the cards

// Class that is used to create cards for play
class card {
public:
    int num = 0;
    string suit;
    string rank;
};

// Stores user information
class user {
public:
    string name;
    double wager = 0;
    card playerCards[13];
    int handValue = 0;
    int aces = 0;
};


// Stores the cards in an array and uses it to simulate a deck
class deck {
public:
    // Construsctor for the starting deck
    deck() {
        srand(time(0));
        int slot = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 13; j++) {
                cards[slot].suit = SUITS[i];
                cards[slot].num = NUMS[j];
                cards[slot].rank = RANKS[j];
                slot++;
            }
        }
    }

    // picks random cards from the deck to put in the user's starting hand
    card draw() {
        card tmpcard;
        tmpcard = cards[rand() % 52];
        return tmpcard;
    }
private:
    card cards[52];
};

class game {
private:
    user players[2];    // Dealer and user
public:
    game(string name, double wager) {
        players[0].name = "Dealer";
        players[1].name = name;
        players[0].wager = 100;
        players[1].wager = wager;
    }

    // Prints the menu to the player
    void Menu(deck deck) {
        int menuDecision;   // Holds the player's menu decision
        cout << "Welcome to Black Jack\n"
            << "1) Play\n"
            << "2) Quit\n";

        // Player enters menu decision
        cin >> menuDecision;

        // Error checks if menu option is valid
        while (menuDecision < 0 && menuDecision > 2) {
            cout << "Please select a valid option. ";
            cin >> menuDecision;
        }

        if (menuDecision == 1) {
            startRound(deck);
        }
        else
            cout << "Thank you for playing.";
    }

    void startRound(deck deck) {
        int roundDecision;  // Holds decision for hit, hold, or fold.
        int count = 2;  // Holds player hand count
        bool dealerNatural = false, playerNatural = false;  // determines blackjack pressence
        bool turnOver = false;  // determines if the player's turn is over

        // Draw a fresh hand for the dealer
        players[0].playerCards[0] = deck.draw();
        players[0].playerCards[1] = deck.draw();
        players[0].handValue = players[0].playerCards[0].num + players[0].playerCards[1].num;

        // Checks dealer blackjack
        if (players[0].handValue == 21)
            dealerNatural = true;

        // Draw a fresh hand for the player now
        players[1].playerCards[0] = deck.draw();
        players[1].playerCards[1] = deck.draw();
        players[1].handValue = players[1].playerCards[0].num + players[1].playerCards[1].num;

        // checks player blackjack
        if (players[1].handValue == 21)
            playerNatural = true;

        // determines blackjack turnouts
        if (dealerNatural == true && playerNatural == false) {
            cout << "\nDealer has a blackjack\n";
            cout << "\n ----- DEALER WINS! ----- \n";
            cout << "You have lost $" << players[1].wager;
            return;
        }
        else if (dealerNatural == false && playerNatural == true) {
            cout << "\nYou have a blackjack\n";
            cout << "\n ----- " << players[0].name << " WINS!----- \n";
            cout << "You won $" << 2.5 * players[1].wager;
            return;
        }
        else if (dealerNatural == true && playerNatural == true) {
            cout << "\nBoth players have a blackjack\n";
            cout << "\n ----- TIE ----- ";
            cout << "$" << players[1].wager << " has been returned to you";
            return;
        }
            
        // Displays the dealer's current face up card
        cout << "\n" << players[0].name << " has a " << players[0].playerCards[0].rank << " of "
            << players[0].playerCards[0].suit << " face up.\n";

        // Displays an update on the current player's standing in the round
        cout << "\n ----- " << players[1].name << "'s Turn ----- "
            << "\nHand value: " << players[1].handValue
            << "\n    Card 1: " << players[1].playerCards[0].rank << " of " << players[1].playerCards[0].suit // Card #1
            << "\n    Card 2: " << players[1].playerCards[1].rank << " of " << players[1].playerCards[1].suit // Card #2
            << "\n";
        // simulate user turn
        do {
            // Show user their options
            cout << "\n1) Hit\n"
                << "2) Stand\n"
                << "3) Fold\n";

            cin >> roundDecision;

            // Error check improper decisions
            while (roundDecision < 1 || roundDecision > 3) {
                cout << "Please select a valid option. ";
                cin >> roundDecision;
            }

            // Draw another card if the user asks to hit
            if (roundDecision == 1) {
                players[1].playerCards[count++] = deck.draw();
                cout << "    Card " << count << ": " << players[1].playerCards[count - 1].rank << " of " << players[1].playerCards[count - 1].suit << "\n";
                players[1].handValue = players[1].handValue + players[1].playerCards[count - 1].num;
                if (players[1].handValue <= 21)
                    cout << "Hand value: " << players[1].handValue << "\n";
                // Checks if player has busted
                if (players[1].handValue > 21) {
                    // Look for how many aces the player has
                    for (int i = 0; i < 13; i++) {
                        if (players[1].playerCards[i].num == 11)
                            players[1].aces++;
                    }

                    // Reduce aces until below hand value 21 if possible
                    while (players[1].handValue > 21 && players[1].aces > 0) {
                        for (int i = 0; i < 13; i++) {
                            if (players[1].playerCards[i].num == 11) { // find an ace with num 11
                                players[1].playerCards[i].num = 1;  // make its num = 1
                                players[1].handValue -= 10;
                                players[1].aces--;
                                cout << "Hand value: " << players[1].handValue << "\n";
                                break;
                            }
                        }
                    }
                    if (players[1].handValue > 21) {
                        cout << "Hand value: " << players[1].handValue << "\n";
                        cout << " ----- BUST!!! ----- \n";
                        turnOver = true;
                    }
                }
            }
            // End turn if user stands
            if (roundDecision == 2) {
                cout << "\nYou stand and await the dealer's turn.\n\n";
                turnOver = true;
            }
            // End round if user folds
            if (roundDecision == 3) {
                cout << "Folding Round...\n\n"
                    << "You have lost $" << players[1].wager;
                return;
            }
        } while (turnOver != true);

        // Simulate the dealer's turn, but only if player hasnt busted
        if (players[1].handValue <= 21) {
            count = 2; // set card cound to 2, representing the dealer's starting card count
            // Show dealer hand

            cout << " --- The dealer reveals their hand --- \n";
            cout << "\n    Card 1: " << players[0].playerCards[0].rank << " of " << players[0].playerCards[0].suit
                << "\n    Card 2: " << players[0].playerCards[1].rank << " of " << players[0].playerCards[1].suit;

            // if dealer hand is less than 17, draw
            while (players[0].handValue < 17) {
                players[0].playerCards[count++] = deck.draw();
                cout << "\n    Card " << count << ": " << players[0].playerCards[count - 1].rank << " of " << players[0].playerCards[count - 1].suit << "\n";
                players[0].handValue = players[0].handValue + players[0].playerCards[count - 1].num;
                if (players[0].handValue <= 21)
                    cout << "Hand value: " << players[0].handValue << "\n";
                // Checks if player has busted
                if (players[0].handValue > 21) {
                    // Look for how many aces the player has
                    for (int i = 0; i < 13; i++) {
                        if (players[0].playerCards[i].num == 11)
                            players[0].aces++;
                    }

                    // Reduce aces until below hand value 21 if possible
                    while (players[0].handValue > 21 && players[0].aces > 0) {
                        for (int i = 0; i < 13; i++) {
                            if (players[0].playerCards[i].num == 11) { // find an ace with num 11
                                players[0].playerCards[i].num = 1;  // make its num = 1
                                players[0].handValue -= 10;
                                players[0].aces--;
                                cout << "Hand value: " << players[0].handValue << "\n";
                                break;
                            }
                        }
                    }
                    if (players[0].handValue > 21) {
                        cout << " ----- DEALER BUST!!! ----- \n"
                            << "You have won $" << 2 * players[1].wager;
                        return;
                    }
                }
            }
        }
        // Determine the winner if nobody has busted or achieved blackjack
        if (players[1].handValue > players[0].handValue && players[1].handValue <= 21 && players[0].handValue <= 21)
            cout << "\n ----- " << players[1].name << " WINS -----\n Winnings : $" << 2 * players[1].wager << "\n\n";
        else cout << "\n ----- " << players[0].name << " WINS ----- \n Losses: $" << players[1].wager << "\n\n";
    }
};

int main()
{
    string name;
    double wager = 0;
    deck playingDeck= deck();
    cout << "Please enter your name: ";
    cin >> name;
    cout << "Please enter your wager: $";
    cin >> wager;
    game newGame = game(name, wager);
    newGame.Menu(playingDeck);

    return 0; 
}