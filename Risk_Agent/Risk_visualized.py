import pygame
import json
import time

PLAYER_COLORS = ('cyan', 'red', 'green', 'purple', 'pink', 'yellow')
place_names = ('Afghanistan', 'Alaska', 'Alberta', 'Argentina', 'Brazil', 'Central America',
 'China', 'Congo', 'East Africa', 'Eastern Australia', 'Eastern United States',
 'Egypt', 'Great Britain', 'Greenland', 'Iceland', 'India', 'Indonesia',
 'Irkutsk', 'Japan', 'Kamchatka', 'Madagascar', 'Middle East', 'Mongolia',
 'New Guinea', 'North Africa', 'Northern Europe', 'Northwest Territory',
 'Ontario', 'Peru', 'Quebec', 'Scandinavia', 'Siam', 'Siberia', 'South Africa',
 'Southern Europe', 'Ukraine', 'Ural', 'Venezuela', 'Western Australia',
 'Western Europe', 'Western United States', 'Yakursk')
marker_locations = [(651, 232), (58, 128), (151, 192), (304, 513),
                    (354, 433), (206, 328), (760, 281), (518, 390),
                    (550, 350), (891, 486), (248, 272), (517, 302),
                    (429, 196), (384, 60), (418, 157), (681, 326),
                    (790, 391), (785, 182), (880, 267), (956, 127),
                    (590, 453), (585, 291), (793, 230), (876, 402),
                    (442, 331), (487, 206), (151, 128), (218, 193),
                    (284, 439), (298, 197), (500, 123), (753, 335),
                    (735, 111), (522, 458), (513, 247), (575, 171),
                    (674, 135), (296, 370), (822, 469), (434, 251), (169, 251), (841, 112)]
pygame.init()
myfont = pygame.font.SysFont("monospace", 15)
with open('random_wins.json', 'r') as file:
    game_data = json.load(file)[1:]

def display_board(turn_number):
    markers = {}
    territories = game_data[turn_number]
    for place_name, marker_location, territory in zip(place_names, marker_locations, territories):
        markers[place_name] = pygame.draw.circle(screen,
                            PLAYER_COLORS[territories[territory]['player']], marker_location, 20)
        label = myfont.render(str(territories[territory]['armies']), 1, 'black')
        screen.blit(label, marker_location)
    return markers

screen = pygame.display.set_mode((1022, 600))
done = False
image=pygame.image.load('risk_map.jpg')
screen.blit(image, (0, 0))
turn = 0
next_button = pygame.draw.rect(screen,'red',[10,550,40,40])
back_button = pygame.draw.rect(screen,'green',[60,550,40,40])


while not done:
    if (turn < len(game_data)):
        markers = display_board(turn)
        turn_number = pygame.draw.rect(screen, 'blue', [10, 500, 40, 40])
        label = myfont.render(str(turn), 1, 'black')
        screen.blit(label, (10, 500))
        time.sleep(2)
        pygame.display.flip()
        turn += 1

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
