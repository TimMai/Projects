import random

class Game:
    def __init__(self, number, name):
        # Create players
        self.human = Player(name)
        self.robots = []
        number -= 1
        while number > 0:
            self.robots.append(Player("Robot" + str(number)))
            number -= 1

        # Set up cards and board
        self.unused_cards = []
        self.used_cards = []
        self.board = []
        self.pot = 0

        for suit in range(1, 5):
            for number in range(2, 15):
                if suit == 1:
                    new_card = Card(number, "♠")
                elif suit == 2:
                    new_card = Card(number, "♣")
                elif suit == 3:
                    new_card = Card(number, "♦")
                else:
                    new_card = Card(number, "♥")
                self.unused_cards.append(new_card)

    # Return list of players (human and robots)
    def get_players(self):
        players =[self.human]
        for robot in self.robots:
            players.append(robot)
        return players

    # Generates one card
    def generate_card(self):
        drawn_card = random.choice(self.unused_cards)
        self.used_cards.append(drawn_card)
        self.unused_cards.remove(drawn_card)
        return drawn_card

    # Generates a hand for each player
    def get_hand(self):
        card1 = self.generate_card()
        card2 = self.generate_card()
        self.human.hand.append(card1)
        self.human.hand.append(card2)

        for robot in self.robots:
            card1 = self.generate_card()
            card2 = self.generate_card()
            robot.hand.append(card1)
            robot.hand.append(card2)

    # Flop
    def board_flop(self):
        card1 = self.generate_card()
        card2 = self.generate_card()
        card3 = self.generate_card()
        self.board.append(card1)
        self.board.append(card2)
        self.board.append(card3)

    # Turn
    def turn(self):
        card4 = self.generate_card()
        self.board.append(card4)

    # River
    def river(self):
        card5 = self.generate_card()
        self.board.append(card5)




class Player:
    def __init__(self, name):
        self.name = name
        self.money = 100
        self.hand = []

class Card:
    def __init__(self, number, suit):
        self.number = number
        self.suit = suit


def welcome():
    print("========================")
    print("Welcome to Texas Hold'em")
    print("========================")


def menu():
    print("========================")
    print("Choices")
    print("1: View hand")
    print("2: View board")
    print("3: View current money and pot")
    print("4: Bet/raise")
    print("5: Fold")
    print("6: Quit")



def game():
    name = input("Enter your name:")
    human = Player(name)

    while True:
        menu()
        choice = int(input())
        if choice == 6:
            break



def takeFirst(elem):
    return elem.number


def score(player, game):
    cards = player.hand + game.board
    sorted_cards = sorted(cards, key=takeFirst)
    for card in sorted_cards:
        print(str(card.number) + "" + card.suit)


    # Pair


    # High card
    print(sorted_cards[6].number)
    print(float("1." + str(sorted_cards[6].number)))
    return float("1." + str(sorted_cards[6].number))


game = Game(3, "Bob")
game.get_hand()
players = game.get_players()
game.board_flop()
game.turn()
game.river()
for player in players:
    print(player.name)
    for card in player.hand:
        print(card.number, card.suit)

print("Board")
for card in game.board:
    print(card.number, card.suit)

print("Score")
score(game.human, game)
