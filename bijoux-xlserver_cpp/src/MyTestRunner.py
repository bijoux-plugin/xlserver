#!/usr/bin/python

#echo "PASS: foo.sh" >> MyTestRunner.trs
#echo "PASS: zardoz.tap 1 - Daemon started" >> MyTestRunner.trs
#echo "PASS: zardoz.tap 2 - Daemon responding" >> MyTestRunner.trs
#echo "SKIP: zardoz.tap 3 - Daemon uses /proc # SKIP /proc is not mounted" >> MyTestRunner.trs
#echo "PASS: zardoz.tap 4 - Daemon stopped" >> MyTestRunner.trs
#echo "SKIP: bar.sh" >> MyTestRunner.trs
#echo "PASS: mu.tap 1" >> MyTestRunner.trs
#echo "XFAIL: mu.tap 2 # TODO frobnication not yet implemented" >> MyTestRunner.trs

#cp MyTestRunner.trs FunctionLoaderTests.trs
#cp MyTestRunner.log FunctionLoaderTests.log
#echo $@ >> MyTestRunner.args
#exit 0

import sys, os

#f = open ( "my_log.out", "w" )
#f.write ( "Current Working Directory: " + os.getcwd() + "\n"  )
#f.write ( "\n".join ( sys.argv ) )
#f.write ( "all arguments joined in one line:\n" )
#f.write ( " ".join ( sys.argv ) )
#f.close ( )

import argparse

parser = argparse.ArgumentParser ( description='Custom Test Runner' )
parser.add_argument ( "--test-name", metavar="test_name", type=str)
parser.add_argument ( "--log-file", metavar="log_file", type=str)
parser.add_argument ( "--trs-file", metavar="trs_file", type=str)
parser.add_argument ( "--color-tests", metavar="color_tests", type=str)
parser.add_argument ( "--enable-hard-errors", metavar="enabled_hard_errors", type=str)
parser.add_argument ( "--expect-failure", metavar="expect_failure", type=str)
parser.add_argument ( "positional", action="store" )

args = vars ( parser.parse_args ( ) )

# Using the following arguments:
# args['test_name'] - name of test
# args['log_file'] - name of log file that gets output
# args['trs_file'] - trs file that automake uses to collect our results
# args['positional'] - Program to execute to generate output

os.system ( "" + args['positional'] + "&>" + args['log_file'] )

fin = open ( args['log_file'], 'r' )
file_content = fin.read ()
fin.close()

def PrintResult ( success, f_handle ):
	if success :
		result_text = "PASS"
	else:
		result_text = "FAIL"
	f_handle.write ( ":test-result: " + result_text + "\n" )
	f_handle.write ( ":global-test-result: " + result_text + "\n" )
	f_handle.write ( ":recheck: no\n" )
	f_handle.write ( ":copy-in-global-log: no\n" )

import re
# Get total number of tests
num_tests = 0
m = re.search ( '^Running ([0-9]+) test cases...', file_content )
if not m == None:
	num_tests = int ( m.group ( 1 ) )

# Get # of errors
num_failures = 0
m = re.search ( '([0-9]+) failure(s)* detected', file_content )
if not m == None:
	num_failures = int ( m.group ( 1 ) )


# Create .trs log file
fout = open ( args['trs_file'], 'w' )
for i in range( num_failures ):
	PrintResult ( False, fout )
for i in range ( num_tests - num_failures ):
	PrintResult ( True, fout )
fout.close ( )

sys.exit ( 0 )
