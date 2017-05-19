from classes.cards import Game
from classes.cards import welcome, menu, takeFirst

welcome()
while True:
    menu()
    choice = int(input())
    if choice == 6:
        break

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

sorted_board_cards = sorted(game.board, key = takeFirst)
for card in sorted_board_cards:
    print(str(card.number) + "" + card.suit)