#!/usr/bin/python

import random, sys, argparse


def main():
    usage_msg = """shuf [OPTION]... FILE
Write a random permutation of the input lines to standard output. 
            
With no FILE, or when FILE is -, read standard input."""
    parser = argparse.ArgumentParser(usage=usage_msg)
    parser.add_argument("-e", "--echo", action='store_true', default=False,
                        help="treat each ARG as an input line")
    parser.add_argument("-i", "--input-range", metavar="LO-HI", dest="inputRange", nargs=1,
                        help="treat each number LO through HI as an input line")
    parser.add_argument("-n", "--head-count", dest="count", nargs=1, type=str,
                        help="output at most COUNT lines")
    parser.add_argument("-r", "--repeat", action='store_true', default=False,
                        help="output lines can be repeated")
    parser.add_argument("infile", nargs='?', default=sys.stdin, help=argparse.SUPPRESS)
    args, unknown = parser.parse_known_args()

    # variables that need to be specified for a valid output
    # inputLines are the final lines that need to be shuffled and printed
    inputLines = []
    # read file flag would be set false if -e or -i is invoked
    readFile = True
    # numlines is the lines to be printed (would be specified by -n)
    numlines = -1

    # if the command line input includes the -e or -i command, ignore any possible read-in file
    if args.echo or args.inputRange:
        readFile = False
        if sys.stdin != args.infile:
            unknown.append(args.infile)

    # -e and -i options both specify the input, so they cannot be invoked together
    if args.echo and args.inputRange:
        sys.stderr.write('shuf: cannot combine -e and -i options\n'
                         'Try \'shuf --help\' for more information.\n')
        return

    # regard the inputLines as the remaining ARGs
    if args.echo:
        try:
            for char in unknown:
                inputLines.append('{0}\n'.format(char))
        except IOError as err:
            sys.stderr.write("I/O error: {0}".format(err))
            return

    # regard the argument for -i option as the inputRange
    if args.inputRange:
        if len(args.inputRange) < 1:
            sys.stderr.write('shuf: option requires an argument -- \'i\'\n'
                             'Try \'shuf --help\' for more information.\n')
            return
        else:
            # parse the inputRange string to a valid input range if possible
            index = 0
            while index < len(args.inputRange[0]):
                if args.inputRange[0][index] != '-':
                    index += 1
                else:
                    break

            start = args.inputRange[0][0:index]
            end = args.inputRange[0][index + 1:]
            try:
                start = int(start)
            except:
                sys.stderr.write("invalid input range: '{0}'\n".format(start))
                return
            if start < 0:
                sys.stderr.write("invalid input range: '{0}'\n".format(start))
                return
            try:
                end = int(end)
            except:
                sys.stderr.write("invalid input range: '{0}'\n".format(end))
                return
            if end < 0:
                sys.stderr.write("invalid input range: '{0}'\n".format(end))
                return
            if start > end:
                sys.stderr.write("invalid input range: '{0}'\n".format(args.inputRange[0]))
                return
            # store the processed range of integers to the inputLines
            for i in range(start, end + 1):
                inputLines.append('{0}\n'.format(i))

    # if -e option is not invoked and there are extra arguments, those arguments would be extra operand
    if unknown and not args.echo:
        sys.stderr.write("shuf: extra operand '{0}' \nTry 'shuf --help' for more information.\n".format(unknown[0]))
        return

    # specify the number of lines needed to be printed
    if args.count:
        try:
            numlines = int(args.count[0])
        except:
            sys.stderr.write("invalid line count: '{0}'\n".format(args.count[0]))
            return
        if numlines < 0:
            sys.stderr.write("invalid line count: '{0}'\n".format(args.count[0]))
            return

    # if the readFile flag is set true, read the file from the command line or standard input
    if readFile:
        try:
            if args.infile == '-' or args.infile == sys.stdin:
                f = sys.stdin
            else:
                f = open(args.infile, 'r')
            inputLines = f.readlines()
            f.close()
        except IOError as err:
            sys.stderr.write('I/O error: {0}\n'.format(err))
            return

    # if -n option requires more lines than possible, output all lines in inputLines
    # but if allow repeats, such action is not needed
    iteration = 0
    if numlines != -1:
        if args.repeat:
            iteration = numlines
        else:
            iteration = numlines if numlines < len(inputLines) else len(inputLines)
    else:
        iteration = len(inputLines)

    # shuffle the inputLines and write them to standard output line by line
    while iteration:
        try:
            line = random.choice(inputLines)
            sys.stdout.write("{0}".format(line))
            if not args.repeat:
                inputLines.remove(line)
            if not args.repeat or numlines != -1:
                iteration -= 1
        except IOError as err:
            sys.stderr.write("I/O error: {0}\n".format(err))


if __name__ == "__main__":
    main()
