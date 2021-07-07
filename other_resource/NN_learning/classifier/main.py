from classifier import *

groups1 = generate_half_moon_data(dx = 6, dy = 6)
groups2 = generate_circle_data()
groups1.extend(groups2)

classifier = Classifier2D()
classifier.set_data(groups1)
# classifier.show_points()
classifier.train()
# classifier.load()
# classifier.show_edge()
# print(classifier.classify(10, 4))