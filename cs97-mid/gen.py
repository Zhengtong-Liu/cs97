#!/usr/bin/python
import random, sys, argparse

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

def test():
    parser = argparse.ArgumentParser()

    options, args = parser.parse_known_args()
    li = []
    good_num = args[0]
    bad_num = args[1]
    bad_words = args[2]
    dic = '/usr/share/dict/linux.words'
    try:
        generator1 = randline(dic)
        for index in range(good_num):
            li.append(generator1.chooseline())
    except:
        sys.stderr.write("Cannot open the file '{0}'.\n".format(dic))
    try:
        generator2 = randline(bad_words)
        for index in range(bad_num):
            print(li)
            li.append(generator2.chooseline())
    except:
        sys.stderr.write("Cannot open the file '{0}'.\n".format(bad_words))
    for word in li:
        sys.stdout.write("{0} ".format(word))
