#!/usr/bin/env python

import sys
import getopt
import logging

def log_setting(log_file):
    logging.basicConfig(level=logging.DEBUG,
            format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
            datafmt='%a, %d %b %Y %H:%M:%S',
            filename=log_file,
            filemode='a')
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
    console.setFormatter(formatter)
    logging.getLogger('').addHandler(console)

def test_getopt(argv):
    opts, args = getopt.getopt(argv[1:], "hi:o:")
    logging.info(opts)
    logging.info("---------")
    logging.info(args)
    i_file = ""
    o_file = ""
    for op, value in opts:
        if op == "-i":
            i_file = value
        elif op == "-o":
            o_file = value
        elif op == "-h":
            usage()
            sys.exit()
    logging.error("input file %s | output file %s" % (i_file, o_file))

def test_var_scope():
    print a
    try:
        raise Exception()
        abc = "abc"
    finally:
        print abc

def usage():
    print "usage:..."

if __name__=="__main__":
    i = 0
    while i < len(sys.argv):
        print "%d: %s" %(i, sys.argv[i])
        i += 1
    log_setting("test.log")
    a = 1
    test_var_scope()
    print "after test_var_scope a=%d" % a
    # test_getopt(sys.argv)

